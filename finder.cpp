#include "finder.h"
#include "record.h"
#include "webpage.h"
#include "webview.h"

#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QIcon>

Parser::StringRef *Finder::s_authorIndex = nullptr;
Parser::StringRef *Finder::s_titleIndex = nullptr;
Parser::StringRef *Finder::s_keyIndex = nullptr;
quint32 Finder::s_authorIndexs = 0;
quint32 Finder::s_titleIndexs = 0;
quint32 Finder::s_keyIndexs = 0;
QFile *Finder::s_file = nullptr;

Finder::Finder(QObject *parent) : QObject(parent)
{
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    m_fileName = settings.value("lastOpenFileName").toString();
}

void Finder::find(const QString &word, const QString &type)
{
//    qDebug() << word << type;
    QString result;
    if(type == "author"){
        auto list = indexOfAuthor(word);
        result = getJson(list);
    }else if(type == "title"){
        auto list = indexOfTitle(word);
        result = getJson(list);
    }else if(type == "coauthor"){
        auto list = indexOfAuthor(word);
        QSet<QString> coauthors;
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Record record(Util::findRecord(m_fileName, pos));
            foreach(auto author, record.authors()){
                coauthors.insert(author);
            }
        }
        coauthors.remove(word);
        result = QJsonDocument(QJsonArray::fromStringList(coauthors.toList())).toJson();
    }
    emit ready(result);
}

void Finder::handleRequest(QUrl url)
{
    QWebEngineView *view = new QWebEngineView;
    view->setWindowIcon(qobject_cast<QWidget*>(parent())->windowIcon());
    view->setAttribute(Qt::WA_DeleteOnClose);
    QString key = url.path().remove(0, 1);
//    qDebug() << key;
    auto list = indexOfKey(key);
    Q_ASSERT(!list.isEmpty());
    auto pos = list.at(0);
    Record record(Util::findRecord(m_fileName, pos));
    auto html = Util::readFile(":/resources/detail.html");
    html.replace("{{title}}", record.title());
//    qDebug() << html;
    view->setHtml(html, QUrl("qrc:/resources/"));
    view->show();
}

bool Finder::parsed()
{
    return QFile("author.dat").exists() 
            && QFile("title.dat").exists()
            && QFile("key.dat").exists();
}

void Finder::clearIndex()
{
    delete s_authorIndex;
    delete s_titleIndex;
    delete s_keyIndex;
    s_authorIndex = nullptr;
    s_titleIndex = nullptr;
    s_keyIndex = nullptr;
}

QList<quint32> Finder::indexOfAuthor(const QString &author) const
{
    QList<quint32> list;
    if(s_authorIndex == nullptr) return list;
    auto range = equalRange(s_authorIndex, s_authorIndex + s_authorIndexs, author);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

QList<quint32> Finder::indexOfTitle(const QString &title) const
{
    QList<quint32> list;
    if(s_titleIndex == nullptr) return list;
    auto range = equalRange(s_titleIndex, s_titleIndex + s_titleIndexs, title);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

QList<quint32> Finder::indexOfKey(const QString &key) const
{
    QList<quint32> list;
    if(s_keyIndex == nullptr) return list;
    auto range = equalRange(s_keyIndex, s_keyIndex + s_keyIndexs, key);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

QString Finder::readText(const Parser::StringRef &ref)
{
    Q_ASSERT(s_file != nullptr);
    Q_ASSERT(s_file->isOpen());
    s_file->seek(ref.l);
    return s_file->read(ref.r - ref.l);
}

QString Finder::getJson(const QList<quint32> &posList)
{
    QJsonArray array;
    for(int i = 0; i < posList.size(); ++i){
        auto pos = posList.at(i);
        Record record(Util::findRecord(m_fileName, pos));
        QJsonObject o;
        o.insert("title", record.title());
        o.insert("year", record.year());
        o.insert("authors", QJsonValue::fromVariant(record.authors()));
        o.insert("mdate", record.mdate());
        o.insert("key", record.key());
        array.append(o);
    }
    return QJsonDocument(array).toJson();
}
void Finder::init()
{
    if(!parsed()) return ;
    QFile file("author.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    s_authorIndexs = static_cast<quint32>(file.size() >> 3);
    s_authorIndex = new Parser::StringRef[s_authorIndexs];
    for(quint32 i = 0; i < s_authorIndexs; ++i){
        stream >> s_authorIndex[i].l >> s_authorIndex[i].r;
    }
    file.close();
    file.setFileName("title.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    s_titleIndexs = static_cast<quint32>(file.size() >> 3);
    s_titleIndex = new Parser::StringRef[s_titleIndexs];
    for(quint32 i = 0; i < s_titleIndexs; ++i){
        stream >> s_titleIndex[i].l >> s_titleIndex[i].r;
    }
    file.close();
    file.setFileName("key.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    s_keyIndexs = static_cast<quint32>(file.size() >> 3);
    s_keyIndex = new Parser::StringRef[s_keyIndexs];
    for(quint32 i = 0; i < s_keyIndexs; ++i){
        stream >> s_keyIndex[i].l >> s_keyIndex[i].r;
    }
    file.close();
    if(s_file != nullptr){
        if(s_file->isOpen()) s_file->close();
        delete s_file;
        s_file = nullptr;
    }
    
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    QString fileName = settings.value("lastOpenFileName").toString();
    s_file = new QFile(fileName);
    s_file->open(QFile::ReadOnly);
}

QPair<const Parser::StringRef *, const Parser::StringRef *> 
Finder::equalRange(const Parser::StringRef *first, 
                   const Parser::StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        QString text = readText(*mid);
        int cmp = QString::compare(text, val);
        if(cmp < 0){
            first = mid;
            ++first;
            len = len - half - 1;
        }else if(cmp > 0){
            len = half;
        }else{
            const Parser::StringRef *left = lowerBound(first, mid, val);
            first += len;
            const Parser::StringRef *right = upperBound(++mid, first, val);
            return qMakePair(left, right);
        }
    }
    return qMakePair(first, first);
}

const Parser::StringRef *
Finder::lowerBound(const Parser::StringRef *first, 
                   const Parser::StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        QString text = readText(*mid);
        int ret = QString::compare(text, val);
        if(ret < 0){
            first = mid;
            ++first;
            len = len - half - 1;
        }else{
            len = half;
        }
    }
    return first;
}

const Parser::StringRef *
Finder::upperBound(const Parser::StringRef *first, 
                   const Parser::StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        QString text = readText(*mid);
        int ret = QString::compare(val, text);
        if(ret < 0){
            len = half;
        }else{
            first = mid;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}
