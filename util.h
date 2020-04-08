#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QDebug>
#include <string>
#include <type_traits>

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
    static void initAvailableLanguage();
    static const QVector<QPair<QString, QString>> availableLanguages();
    static QString getLocale();
    
    static QString str(int v) {
        return QString::number(v);
    }
    
    template <typename T>
    static QString str(T v) {
        bool first = true;
        QString res = "{";
        for (const auto &x : v) {
            if (!first) res += ", ";
            first = false;
            res += str(x);
        }
        res += "}";
        return res;
    }
    
private:
    static QVector<QPair<QString, QString>> s_availableLanguages;
};


#endif // UTIL_H
