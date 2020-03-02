#ifndef UTIL_H
#define UTIL_H

#include <QObject>

struct string{
    const char *s;
    int n;
};

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(int ms);
    static QString findRecord(const QString &fileName, qint64 pos);
    static quint8 hash(const QString &s);
    static bool startsWith(const char *s, const char *str, qint64 from = 0);
    static int indexOf(const char *s, const char *str, qint64 from = 0);
    // treat child element as text, after reading, from equal to the
    // last position of end element
    static string readElementText(const char *s, qint64 &from);
};

#endif // UTIL_H
