#pragma once

#include <QObject>
#include <QJsonArray>

#include "parser.h"
#include "stringref.h"
#include "record.h"

class Finder : public QObject
{
    Q_OBJECT
public:
    typedef Parser::YearWord YearWord;
    typedef Parser::WordPos WordPos;
    
    explicit Finder(QObject *parent = nullptr);
    void handleRequest(const QUrl &url);
    void handleWordCloud(const QUrl &url);
    void clearIndex();
    QVector<quint32> indexOfAuthor(const QString &author) const;
    QVector<quint32> indexOfTitle(const QString &title) const;
    QSet<quint32> indexOfTitleWord(const QString &keyword) const;
    QVector<quint32> indexOfTitleWords(const QString &keywords) const;
    QJsonArray cographBFS(const QString &node);
    
    static void init();
    
    bool yearWordLoaded() const;
    void setYearWordLoaded();
    bool authorStacLoaded() const;
    void setAuthorStacLoaded();
    bool authorLoaded() const;
    void setAuthorLoaded();
    bool titleLoaded() const;
    void setTitleLoaded();
    bool titleWordLoaded() const;
    void setTitleWordLoaded();
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
    QVector<Record> getRecord(const QVector<quint32> &posList);
    bool m_loaded{};
    bool m_authorLoaded{};
    bool m_titleLoaded{};
    bool m_authorStacLoaded{};
    bool m_yearWordLoaded{};
    bool m_titleWordLoaded{};
    QVector<Record> m_lastResult;

public:
    static QFile *dataFile;
    static QVector<StringRef> authorIndexs;
    static QVector<StringRef> titleIndexs;
    static QVector<QPair<QString,int>> authorStacs;
    static QVector<QPair<QString, quint32>> titleWords;
    static YearWord yearWord;
    static QString readText(const StringRef &ref);
    // thanks to stl algo
    static QPair<const StringRef*, const StringRef*>
            equalRange(const StringRef *first,
                         const StringRef *last,
                         const QString &val);
    static const StringRef*
            lowerBound(const StringRef *first,
                         const StringRef *last,
                         const QString &val);
    static const StringRef*
            upperBound(const StringRef *first,
                         const StringRef *last,
                         const QString &val);
};
