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
    
    static void clearIndexs();
    
    static bool parsed();
    
    static bool canLoad();
    
    static QString str(int v) {
        return QString::number(v);
    }
    
    static QString str(const QString &s) {
        return s;
    }
    
    template <typename A, typename B>
    static QString str(QPair<A, B> p) {
        return QString("(%1, %2)").arg(str(p.first)).arg(str(p.second));
    }
    
    template <typename A, typename B>
    static QString str(const QMap<A, B> &p) {
        QStringList res;
        for (auto i = p.begin(); i != p.end(); ++i) {
            res.append(str(i.key()) + ": " + str(i.value()));
        }
        return "{" + res.join(", ") + "}";
    }
    
    template <typename A>
    static QString str(A v) {
        QStringList res;
        for (const auto &x : v) {
            res.append(str(x));
        }
        return "{" + res.join(", ") + "}";
    }
    
    
private:
    static QVector<QPair<QString, QString>> s_availableLanguages;
    
    // make sure they all exist before loading
    static QStringList s_loadedFiles;
    
    // these files will be clean
    static QStringList s_parsedFiles;
};


#define STR_RAW(x) #x
#define STR(x) STR_RAW(x)
#define VERSION STR(VERSION_NUMBER)

#endif // UTIL_H
