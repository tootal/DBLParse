#include "record.h"

#include <QXmlStreamReader>

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
    m_mdate = QDate::fromString(attrs.value("mdate").toString(), "yyyy-MM-dd");
    
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
    Q_ASSERT(!reader.hasError());
}
