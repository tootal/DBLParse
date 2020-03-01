#include "record.h"

#include <QXmlStreamReader>
#include <QDebug>

Record::Record(const QString &s, QObject *parent)
    : QObject(parent)
{
    QXmlStreamReader reader(s);
    while(!reader.isStartElement()) reader.readNext();
    m_name = reader.name().toString();
    auto attrs = reader.attributes();
    Q_ASSERT(attrs.hasAttribute("key"));
    m_key = attrs.value("key").toString();
    Q_ASSERT(attrs.hasAttribute("mdate"));
    m_mdate = attrs.value("mdate").toString();
    
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isEndElement() && reader.name() == m_name) break;
        if(reader.isStartElement()){
            if(reader.name() == "author"){
                QString author = reader.readElementText(QXmlStreamReader::IncludeChildElements);
                m_authors.append(author);
            }else if(reader.name() == "title"){
                m_title = reader.readElementText(QXmlStreamReader::IncludeChildElements);
            }
        }
    }
    // Ignore error
//    if(reader.hasError()){
//        qDebug() << reader.errorString();
//        qDebug() << reader.lineNumber();
//    }
//    Q_ASSERT(!reader.hasError());
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
