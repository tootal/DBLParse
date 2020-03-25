#include "record.h"

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
        m_coauthors << author;
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
}

QString Record::capture(const QString &tag) const
{
    QRegularExpression re;
    re.setPattern(QString(R"(<%1.*?>(.+)<\/%1>)").arg(tag));
    auto m = re.match(m_src);
    Q_ASSERT(m.hasMatch());
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

QStringList Record::authors() const
{
    return m_authors;
}

void Record::clearCoauthors()
{
    m_coauthors.clear();
}

QStringList Record::coauthors() const
{
    return m_coauthors;
}
