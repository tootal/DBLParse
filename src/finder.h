#pragma once

#include <QObject>
#include <QJsonArray>
#include <QFile>

#include "parser.h"
#include "record.h"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);
    ~Finder();
    void handleRequest(const QUrl &url);
    void handleWordCloud(const QUrl &url);
    void clearIndex();
    
    void init();
    
    bool yearWordLoaded() const;
    void setYearWordLoaded();
    bool authorStacLoaded() const;
    void setAuthorStacLoaded();
    bool loaded() const;
    void setLoaded();
    
    Q_INVOKABLE void find(const QString &type, const QString &word);
    Q_INVOKABLE void image(const QString &img , const QString &filename);
    Q_INVOKABLE void saveWordCloud(const QString &img , const QString &filename);
    
signals:
    void ready(const QString &data);
    void notReady();
    void detailReady(const QString &data);
    void saveImg(const bool isOk);
    void saveWC(const bool isOk);
    void languageChanged(const QString &locale);
    
private:
    mutable QFile dataFile;
    YearWord yearWord;
    QVector<AuthorStac> authorStacs;
    QVector<Record> m_lastResult;
    
    bool m_loaded{};
    bool m_authorStacLoaded{};
    bool m_yearWordLoaded{};
    
    void getRecord(QVector<Record> &res, const QVector<quint32> &posList) const;
    void getRecord(QVector<Record> &res, const QList<quint32> &posList) const;
    QVector<quint32> indexOfAuthor(const QByteArray &author) const;
    QVector<quint32> indexOfTitle(const QByteArray &title) const;
    QSet<quint32> indexOfTitleWord(const QByteArray &keyword) const;
    QList<quint32> indexOfTitleWords(const QByteArray &keywords) const;
    QJsonArray cographBFS(const QString &node) const;
    
    friend class Loader;
    friend class MainWindow;
};
