#ifndef UTIL_H
#define UTIL_H

#include <QObject>

#include "stringref.h"

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(int ms);
    static QString findRecord(const QString &fileName, qint64 pos);
    static quint8 hash(const QString &s);
    // treat child element as text, after reading, from equal to the
    // last position of end element
    static StringRef readElementText(const StringRef &s, qint64 &from);
};

#endif // UTIL_H
