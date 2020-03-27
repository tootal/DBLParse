#include "record.h"
#include "util.h"

#include <QDebug>
#include <QRegularExpression>

Record::Record(const QString &s, QObject *parent)
    : QObject(parent)
{
    m_src = s;
    QRegularExpression re(R"(<author.*?>(.+)<\/author>)");
    auto i = re.globalMatch(s);
    while(i.hasNext()){
        QString author = i.next().captured(1);
        m_authors << author;
    }
    
    re.setPattern(R"(<editor.*?>(.+)<\/editor>)");
    i = re.globalMatch(s);
    while(i.hasNext()){
        QString editor = i.next().captured(1);
        m_editors << editor;
    }
    
    re.setPattern(R"(<cite.*?>(.+)<\/cite>)");
    i = re.globalMatch(s);
    while(i.hasNext()){
        QString cite = i.next().captured(1);
        m_cites << cite;
    }
    
    re.setPattern(R"#(mdate\s*=\s*"((?:\d{4})-(?:\d{2})-(?:\d{2}))")#");
    auto m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_mdate = m.captured(1);
//    qDebug() << m_mdate;
    
    re.setPattern(R"(<(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data))");
    m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_name = m.captured(1);
//    qDebug() << m_name;
    
    re.setPattern(R"#(key="(.*?)")#");
    m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_key = m.captured(1);
//    qDebug() << m_key;
    
    m_title = capture("title");
    m_year = capture("year");
    m_journal = capture("journal");
    m_volume = capture("volume");
    m_ee = Util::formatUrl(capture("ee"));
    m_url = Util::formatUrl(capture("url"));
    m_booktitle = capture("booktitle");
    m_series = capture("series");
    m_pages = capture("pages");
    m_note = capture("note");
    m_address = capture("address");
    m_number = capture("number");
    m_month = capture("month");
    m_cdrom = capture("cdrom");
    m_publisher = capture("publisher");
    m_crossref = capture("crossref");
    m_isbn = capture("isbn");
    m_school = capture("school");
    m_chapter = capture("chapter");
    m_publnr = capture("publnr");
}

QString Record::capture(const QString &tag) const
{
    QRegularExpression re;
    re.setPattern(QString(R"(<%1.*?>(.+)<\/%1>)").arg(tag));
    auto m = re.match(m_src);
    return m.captured(1);
}

QString Record::title() const
{
    return m_title;
}

QString Record::mdate() const
{
    return m_mdate;
}

QString Record::key() const
{
    return m_key;
}

QString Record::name() const
{
    return m_name;
}

QString Record::year() const
{
    return m_year;
}

QString Record::journal() const
{
    return m_journal;
}

QString Record::volume() const
{
    return m_volume;
}

QStringList Record::authors() const
{
    return m_authors;
}

QString Record::ee() const
{
    return m_ee;
}

QString Record::url() const
{
    return m_url;
}

QStringList Record::editors() const
{
    return m_editors;
}

QString Record::booktitle() const
{
    return m_booktitle;
}

QString Record::series() const
{
    return m_series;
}

QString Record::pages() const
{
    return m_pages;
}

QString Record::note() const
{
    return m_note;
}

QString Record::address() const
{
    return m_address;
}

QString Record::number() const
{
    return m_number;
}

QString Record::month() const
{
    return m_month;
}

QString Record::cdrom() const
{
    return m_cdrom;
}

QStringList Record::cites() const
{
    return m_cites;
}

QString Record::publisher() const
{
    return m_publisher;
}

QString Record::crossref() const
{
    return m_crossref;
}

QString Record::isbn() const
{
    return m_isbn;
}

QString Record::school() const
{
    return m_school;
}

QString Record::chapter() const
{
    return m_chapter;
}

QString Record::publnr() const
{
    return m_publnr;
}
