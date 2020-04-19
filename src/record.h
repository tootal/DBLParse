#ifndef RECORD_H
#define RECORD_H

#include <QDate>
#include <QMap>
#include <QJsonValue>

class Record
{
public:
    Record();
    Record(const QString &s);
    QString capture(const QString &tag) const;
    QVariant attr(const QString &tag) const;
    QJsonObject toJson() const;
    QJsonObject toJson(const QString &type) const;
    
private:
    QString m_src;
    QMap<QString, QVariant> m_attrs;
};

#endif // RECORD_H
