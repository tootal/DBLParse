#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(QTime time);
    static QString findRecord(const QString &fileName, qint64 pos);
};

#endif // UTIL_H
