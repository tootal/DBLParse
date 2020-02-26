#ifndef RECORD_H
#define RECORD_H

#include <QObject>

class QFile;

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(QObject *parent = nullptr);
    static Record fromFile(QFile *file, qint64 pos);
signals:
    
};

#endif // RECORD_H
