#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDate>

class Record : public QObject
{
    Q_OBJECT
public:
    Record(const QString &string);
    
private:
    QString name_;
    QString string_;
    QString key_;
    QDate mdate_;
    QStringList authors_;
    QString title_;
};

#endif // RECORD_H
