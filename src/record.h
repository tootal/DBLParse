#pragma once

#include <QDate>
#include <QMap>
#include <QJsonValue>

class Record
{
public:
    static constexpr int BUF_SZ = 1 << 12;
    Record();
    Record(quint32 pos);
    void get(quint32 pos);
    QJsonObject toJson() const;
    QJsonObject toJson(const QString &type) const;

    QMap<QByteArray, QStringList> attr;
};
