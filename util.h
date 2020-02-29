#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(QTime time);
    static QPair<qint64, qint64> findRecordRange(const QString &filePath, qint64 pos);
};

#endif // UTIL_H
