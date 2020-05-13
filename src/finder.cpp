#include "finder.h"

#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QIcon>
#include <QQueue>
#include <QBuffer>
#include <QElapsedTimer>

#include <algorithm>

#include "record.h"
#include "webpage.h"
#include "webview.h"
#include "loader.h"
#include "mainwindow.h"
#include "application.h"

Finder::Finder(QObject *parent) : QObject(parent)
{
    clearIndex();
    init();
}

void Finder::find(const QString &type, const QString &word)
{
    QVector<Record> result;
    QJsonArray json;
    if (!Util::parsed()) goto not_ready;
    if (type == "author") {
        if (!authorLoaded()) goto not_ready;
        auto list = indexOfAuthor(word);
        result = getRecord(list);
        std::sort(result.begin(), result.end(), [](const Record &x, const Record &y) {
            return x.attr("year").toString() < y.attr("year").toString(); 
        });
        for (const Record &record : result) {
            json.append(record.toJson(type));
        }
    } else if (type == "title") {
        if (!titleLoaded()) goto not_ready;
        auto list = indexOfTitle(word);
        result = getRecord(list);
        std::sort(result.begin(), result.end(), [](const Record &x, const Record &y) {
            return x.attr("mdate").toString() > y.attr("mdate").toString(); 
        });
        for (const Record &record : result) {
            json.append(record.toJson(type));
        }
    } else if (type == "coauthor") {
        if (!authorLoaded()) goto not_ready;
        auto list = indexOfAuthor(word);
        QSet<QString> coauthors;
        for (auto pos : list) {
            Record record(Util::findRecord(Util::getXmlFileName(), pos));
            for (auto &author : record.attr("authors").toStringList()){
                coauthors.insert(author);
            }
        }
        coauthors.remove(word);
        json = QJsonArray::fromStringList(coauthors.values());
    } else if(type == "cograph") {
        QJsonArray cograph;
        cograph=cographBFS(word);
        json = cograph;
    } else if (type == "keywords") {
        if (!titleWordLoaded()) goto not_ready;
        auto list = indexOfTitleWords(word.toLower());
        result = getRecord(list);
        for (int i = 0; i < result.size(); i++) {
            json.append(result[i].toJson(type));
        }
    }
    m_lastResult = result;
    emit ready(QJsonDocument(json).toJson());
    return ;
not_ready:
    emit notReady();
    emit ready("not_ready");
}

void Finder::handleRequest(const QUrl &url)
{
    int idx = url.path().remove(0, 1).toInt();
    auto *view = new WebView(App->window);
    view->setWindowFlag(Qt::Window);
    view->resize(800, 600);
    auto html = Util::readFile(":/web/detail.html");
    auto data = QJsonDocument(m_lastResult[idx].toJson()).toJson();
    // qDebug() << data;
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/web/"));
    view->show();
}

void Finder::handleWordCloud(const QUrl &url)
{
    int idx = url.path().remove(0, 1).toInt();
    auto *view = new WebView(App->window);
    view->setWindowFlag(Qt::Window);
    view->resize(800, 600);
    view->registerObject("finder", this);

    auto html = Util::readFile(":/web/wordCloud.html");
    QJsonArray json;
    QJsonObject year;
    year.insert("year",idx);
    json.append(year);
    auto it = yearWord.find(idx).value().begin();
    while(it != yearWord.find(idx).value().end()){
     QJsonObject obj;
     obj.insert("name", it->word);
     obj.insert("value", it->count);
     json.append(obj);
     it++;
    }
    auto data = QJsonDocument(json).toJson();
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/web/"));
    view->show();
}

void Finder::clearIndex()
{
    m_loaded = false;
    m_authorLoaded = false;
    m_titleLoaded = false;
    m_authorStacLoaded = false;
    authorIndexs.clear();
    titleIndexs.clear();
    authorStacs.clear();
    titleWords.clear();
}

QVector<quint32> Finder::indexOfAuthor(const QString &author) const
{
    QVector<quint32> list;
    AuthorIndex authorIndex{author.toLatin1(), 0, 0};
    auto range = std::equal_range(authorIndexs.begin(), 
                                  authorIndexs.end(), 
                                  authorIndex);
    for (auto i = range.first; i != range.second; ++i) {
        list.append(i->end);
    }
    return list;
}

QVector<quint32> Finder::indexOfTitle(const QString &title) const
{
    QVector<quint32> list;
    TitleIndex titleIndex{title.toLatin1(), 0, 0};
    auto range = std::equal_range(titleIndexs.begin(), 
                                  titleIndexs.end(), 
                                  titleIndex);
    for(auto i = range.first; i != range.second; ++i)
        list.append(i->end);
    return list;
}

QSet<quint32> Finder::indexOfTitleWord(const QString &keyword) const
{
    QSet<quint32> set;
    auto range = std::equal_range(
                    titleWords.begin(), 
                    titleWords.end(),
                    WordPos(keyword));
    for (auto i = range.first; i != range.second; ++i) {
        set.insert(i->pos);
    }
    return set;
}

QVector<quint32> Finder::indexOfTitleWords(const QString &keywords) const
{
    auto words = keywords.split(' ');
    auto res = indexOfTitleWord(words[0]);
    for (int i = 1; i < words.size(); i++) {
        auto t = indexOfTitleWord(words[i]);
        res.intersect(t);
    }
    return QVector<quint32>::fromList(res.values());
}

bool Finder::yearWordLoaded() const
{
    return m_yearWordLoaded;
}

void Finder::setYearWordLoaded()
{
    m_yearWordLoaded = true;
}

bool Finder::authorStacLoaded() const
{
    return m_authorStacLoaded;
}

void Finder::setAuthorStacLoaded()
{
    m_authorStacLoaded = true;
}

bool Finder::authorLoaded() const
{
    return m_authorLoaded;
}

void Finder::setAuthorLoaded()
{
    m_authorLoaded = true;
}

bool Finder::titleLoaded() const
{
    return m_titleLoaded;
}

void Finder::setTitleLoaded()
{
    m_titleLoaded = true;
}

bool Finder::titleWordLoaded() const
{
    return m_titleWordLoaded;
}

void Finder::setTitleWordLoaded()
{
    m_titleWordLoaded = true;
}

bool Finder::loaded() const
{
    return m_loaded;
}

void Finder::setLoaded()
{
    m_loaded = true;
}

QString Finder::readText(const StringRef &ref) const
{
    dataFile.seek(ref.l);
    return dataFile.read(ref.r - ref.l);
}

QVector<Record> Finder::getRecord(const QVector<quint32> &posList) const
{
    QVector<Record> array;
    int size = std::min(posList.size(), 2000);
    auto fileName = Util::getXmlFileName();
    for (int i = 0; i < size; i++) {
        Record record(Util::findRecord(fileName, posList.at(i)));
        array.append(record);
    }
    auto ret = array;
    return ret;
}
void Finder::init()
{
    dataFile.close();
    dataFile.setFileName(Util::getXmlFileName());
    dataFile.open(QFile::ReadOnly);
}

QJsonArray Finder::cographBFS(const QString &node) const
{
    QQueue<QPair<QString,QString>> q;
    QJsonArray cograph;
    QPair<QString,QString> parent("",node);
    q.enqueue(parent);
    int num=0;
    while(num<2 && !q.empty()){
        QPair<QString,QString> t=q.head();
        q.dequeue();
        QJsonObject coNode;
        auto list = indexOfAuthor(t.second);
        QSet<QString> coauthors;
        for (auto pos : list) {
            Record record(Util::findRecord(Util::getXmlFileName(), pos));
            for (auto &author : record.attr("authors").toStringList()){
                coauthors.insert(author);
            }
        }
        coauthors.remove(t.second);

        if(!t.first.isEmpty()){
            coauthors.remove(t.first);
        }

        coNode.insert("parentNode",t.second);
        if (!coauthors.isEmpty()) {
            coNode.insert("childNodes",QJsonArray::fromStringList(coauthors.values()));
            for (const QString &value : coauthors){
                QPair<QString,QString> temp(t.second,value);
                q.enqueue(temp);
            }
            cograph.append(coNode);
            num++;
            coauthors.clear();
        }
    }
    q.clear();
    return cograph;
}

void Finder::image(const QString &img , const QString &filename)
{
    QPixmap image;    
    image.loadFromData(QByteArray::fromBase64(img.section(",", 1).toLocal8Bit()));
    bool isOk=image.save("./"+filename+".png");
//    qDebug()<<isOk;
    emit saveImg(isOk);

}

void Finder::saveWordCloud(const QString &img , const QString &filename)
{
    QPixmap image;
    image.loadFromData(QByteArray::fromBase64(img.section(",", 1).toLocal8Bit()));
    bool isOk=image.save("./"+filename+".png");
//    qDebug()<<isOk;
    emit saveWC(isOk);

}
