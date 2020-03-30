#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QJsonValue>

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(const QString &s, QObject *parent = nullptr);
    QString capture(const QString &tag) const;
    QVariant attr(const QString &tag) const;
    QJsonObject toJson() const;
    
private:
    QString m_src;
    QMap<QString, QVariant> m_attrs;
};

#endif // RECORD_H
