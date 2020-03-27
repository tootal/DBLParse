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
    QString booktitle() const;
    QString series() const;
    QString pages() const;
    QString note() const;
    QString address() const;
    QString number() const;
    QString month() const;
    QString cdrom() const;
    QStringList cites() const;
    QString publisher() const;
    QString crossref() const;
    QString isbn() const;
    QString school() const;
    QString chapter() const;
    QString publnr() const;
    
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
    QString m_booktitle;
    QString m_series;
    QString m_pages;
    QString m_note;
    QString m_address;
    QString m_number;
    QString m_month;
    QString m_cdrom;
    QStringList m_cites;
    QString m_publisher;
    QString m_crossref;
    QString m_isbn;
    QString m_school;
    QString m_chapter;
    QString m_publnr;
};

#endif // RECORD_H
