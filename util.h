#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(QTime time);
};

#endif // UTIL_H
