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
#include "hash.h"
#include "stemmer.h"

Finder::Finder(QObject *parent) : QObject(parent)
{
    clearIndex();
    init();
}

Finder::~Finder()
{
}

void Finder::find(const QString &type, const QString &word)
{
    QVector<Record> result;
    QJsonArray json;
    if (!Util::parsed()) goto not_ready;
    if (type == "author") {
        auto list = indexOfAuthor(word.toLatin1());
        QVector<Record> t_result;
        getRecord(t_result, list);
        for (const auto &i : t_result) {
            const auto &t = i.attr["author"];
            if (t.contains(word)) {
                result.append(i);
            }
        }
        /*std::sort(result.begin(), result.end(), [](const Record &x, const Record &y) {
            return x.property("year").toStringList()[0] < y.property("year").toStringList()[0]; 
        });*/
        for (const Record &record : result) {
            json.append(record.toJson(type));
        }
    } else if (type == "title") {
        auto list = indexOfTitle(word.toLatin1());
        QVector<Record> t_result;
        getRecord(t_result, list);
        for (const auto &i : t_result) {
            if (i.attr["title"][0] == word) {
                result.append(i);
            }
        }
        /*std::sort(result.begin(), result.end(), [](const Record &x, const Record &y) {
            return x.property("mdate").toStringList()[0] > y.property("mdate").toStringList()[0];
        });*/
        for (const Record &record : result) {
            json.append(record.toJson(type));
        }
    } else if (type == "coauthor") {
        auto list = indexOfAuthor(word.toLatin1());
        QSet<QString> coauthors;
        for (auto pos : list) {
            Record record(pos);
            for (auto &author : record.attr["author"]) {
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
        auto list = indexOfTitleWords(word.toLatin1());
        qInfo() << "Keywords results: " << list.size();
        getRecord(result, list);
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
     obj.insert("name", QString(it->word));
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
    m_authorStacLoaded = false;
    authorStacs.clear();
}

QVector<quint32> Finder::indexOfAuthor(const QByteArray &author) const
{
    QVector<quint32> list;
    auto hash = Hash::hash2(author);
    QFile file(QString("data/author/%1").arg(Hash::hash1(author)));
    if (!file.open(QFile::ReadOnly)) return list;
    QDataStream s(&file);
    while (!s.atEnd()) {
        HashIndex i;
        s >> i;
        if (i.hash == hash) list.append(i.pos);
    }
    file.close();
    return list;
}

QVector<quint32> Finder::indexOfTitle(const QByteArray &title) const
{
    QVector<quint32> list;
    auto hash = Hash::hash2(title);
    QFile file(QString("data/title/%1").arg(Hash::hash1(title)));
    if (!file.open(QFile::ReadOnly)) return list;
    QDataStream s(&file);
    while (!s.atEnd()) {
        HashIndex i;
        s >> i;
        if (i.hash == hash) list.append(i.pos);
    }
    file.close();
    return list;
}

QSet<quint32> Finder::indexOfTitleWord(const QByteArray &keyword) const
{
    QSet<quint32> set;
    auto hash = Hash::hash2(keyword);
    QFile file(QString("data/word/%1").arg(Hash::hash1(keyword)));
    if (!file.open(QFile::ReadOnly)) return set;
    QDataStream s(&file);
    while (!s.atEnd()) {
        HashIndex i;
        s >> i;
        if (i.hash == hash) set.insert(i.pos);
    }
    file.close();
    return set;
}

QList<quint32> Finder::indexOfTitleWords(const QByteArray &keywords) const
{
    auto words = Stemmer::stem(keywords);
    auto res = indexOfTitleWord(words[0]);
    for (int i = 1; i < words.size(); i++) {
        auto t = indexOfTitleWord(words[i]);
        res.intersect(t);
    }
    return res.values();
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

bool Finder::loaded() const
{
    return m_loaded;
}

void Finder::setLoaded()
{
    m_loaded = true;
}

void Finder::getRecord(QVector<Record> &res, const QVector<quint32> &posList) const
{
    auto size = std::min(posList.size(), (qsizetype)2000);
    res.resize(size);
    auto fileName = Util::getXmlFileName();
    for (int i = 0; i < size; i++) {
        res[i].get(posList.at(i));
    }
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
        auto list = indexOfAuthor(t.second.toLatin1());
        QSet<QString> coauthors;
        for (auto pos : list) {
            Record record(pos);
            for (auto &author : record.attr["author"]) {
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
    emit saveImg(isOk);

}

void Finder::saveWordCloud(const QString &img , const QString &filename)
{
    QPixmap image;
    image.loadFromData(QByteArray::fromBase64(img.section(",", 1).toLocal8Bit()));
    bool isOk=image.save("./"+filename+".png");
    emit saveWC(isOk);

}
