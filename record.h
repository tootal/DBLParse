#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDate>

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(const QString &s, QObject *parent = nullptr);
    
private:
    QString m_name;
    QString m_key;
    QDate m_mdate;
    QStringList m_authors;
    QString m_title;
};

#endif // RECORD_H
