#ifndef FINDER_H
#define FINDER_H

#include <QObject>

#include "parser.h"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);
    Q_INVOKABLE void find(const QString &type, const QString &word);
    void handleRequest(QUrl url);
    static bool parsed();
    void clearIndex();
    static void init();
    QList<quint32> indexOfAuthor(const QString &author) const;
    QList<quint32> indexOfTitle(const QString &title) const;
    QList<quint32> indexOfKey(const QString &key) const;
    static QList<QPair<QString,int> > authorStac() {return s_authorStac;}
    
    bool authorStacLoaded() const;
    void setAuthorStacLoaded();
    bool authorLoaded() const;
    void setAuthorLoaded();
    bool titleLoaded() const;
    void setTitleLoaded();
    bool keyLoaded() const;
    void setKeyLoaded();
    bool loaded() const;
    void setLoaded();
    QJsonArray cographBFS(QString node);
    Q_INVOKABLE void image(const QString &img , const QString &filename);
signals:
    void ready(const QString &data);
    void notReady();
    void detailReady(const QString &data);
    void saveImg(const bool isOk);
    
private:
    QString getJson(const QList<quint32> &posList) ;
    bool m_loaded;
    bool m_authorLoaded;
    bool m_titleLoaded;
    bool m_keyLoaded;
    bool m_authorStacLoaded;

public:
    static QFile *s_file;
    static Parser::StringRef *s_authorIndex;
    static Parser::StringRef *s_titleIndex;
    static Parser::StringRef *s_keyIndex;
    static quint32 s_authorIndexs;
    static quint32 s_titleIndexs;
    static quint32 s_keyIndexs;
    static QList<QPair<QString,int> > s_authorStac;
    static QString readText(const Parser::StringRef &ref);
    // thanks to stl algo
    static QPair<const Parser::StringRef*, const Parser::StringRef*>
            equalRange(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
    static const Parser::StringRef*
            lowerBound(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
    static const Parser::StringRef*
            upperBound(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
};

#endif // FINDER_H
