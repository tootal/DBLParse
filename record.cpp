#include "record.h"

#include <QXmlStreamReader>
#include <QDebug>

Record::Record(const QString &s, QObject *parent)
    : QObject(parent)
{
    
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

QStringList Record::authors() const
{
    return m_authors;
}

void Record::clearCoauthors()
{
    coauthors_.clear();
}

QStringList Record::coauthors() const
{
    return coauthors_;
}
