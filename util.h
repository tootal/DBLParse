#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(int ms);
    static QString findRecord(const QString &fileName, qint64 pos);
    static quint8 hash(const QString &s);
};

#endif // UTIL_H
