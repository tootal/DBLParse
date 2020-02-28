#include "record.h"
#include "dblp.h"

#include <QXmlStreamReader>

Record::Record(const QString &string)
{
    QXmlStreamReader reader(string);
    while(!reader.isStartElement()) reader.readNext();
    Q_ASSERT(Dblp::defaultRecordNames.contains(reader.name()));
    name_ = reader.name().toString();
    auto attrs = reader.attributes();
    Q_ASSERT(attrs.hasAttribute("key"));
    key_ = attrs.value("key").toString();
    Q_ASSERT(attrs.hasAttribute("mdate"));
    mdate_ = QDate::fromString(attrs.value("mdate").toString(), "yyyy-MM-dd");
    
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isEndElement() && reader.name() == name_) break;
        if(reader.isStartElement()){
            if(reader.name() == "author"){
                QString author = reader.readElementText();
                authors_.append(author);
            }else if(reader.name() == "title"){
                title_ = reader.readElementText();
            }
        }
    }
}
