#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDate>
#include <QSet>

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(const QString &s, QObject *parent = nullptr);
    QString capture(const QString &tag) const;
    QString title() const;
    QString mdate() const;
    QString key() const;
    QString name() const;
    QString year() const;
    QString journal() const;
    QString volume() const;
    QStringList authors() const;
    QString ee() const;
    QString url() const;
    QStringList editors() const;
private:
    QString m_src;
    QString m_name;
    QString m_year;
    QString m_key;
    QString m_mdate;
    QStringList m_authors;
    QString m_title;
    QString m_journal;
    QString m_volume;
    QString m_ee;
    QString m_url;
    QStringList m_editors;
};

#endif // RECORD_H
