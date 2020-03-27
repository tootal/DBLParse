#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static const int BUF_SZ = 3000;
    static QString formatTime(int ms);
    static QString readAround(const QString &fileName, quint32 &pos);
    static QString findRecord(const QString &fileName, quint32 pos);
    static QString readFile(const QString &fileName);
    static QString getXmlFileName();
    static QString formatUrl(const QString &url);
    // render <!-- something_holder --> in html if flag is not empty
    static void htmlRender(QString &html, 
                              const QString &flag,
                              const QString &holder,
                              QString templ);
    static void htmlRender(QString &html, 
                              const QStringList &flags,
                              const QString &holder,
                              QString templ);
};

#endif // UTIL_H
