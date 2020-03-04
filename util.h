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
};

#endif // UTIL_H
