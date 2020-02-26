#include "recordparser.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QMultiMap>
#include <QThread>

RecordParser::RecordParser(QXmlStreamReader *r)
{
    reader = r;
    authorIndex_ = nullptr;
}

QString RecordParser::name() const
{
    return name_;
}

QString RecordParser::key() const
{
    return key_;
}

QDate RecordParser::mdate() const
{
    return mdate_;
}

QStringList RecordParser::authors() const
{
    return authors_;
}

void RecordParser::parse()
{
    if(reader->isStartElement()){
        name_ = reader->name().toString();
        auto attrs = reader->attributes();
        if(attrs.hasAttribute("key")){
            key_ = attrs.value("key").toString();
        }
        if(attrs.hasAttribute("mdate")){
            mdate_ = QDate::fromString(attrs.value("mdate").toString(),"yyyy-MM-dd");
        }
        parseContent();
    }
}

void RecordParser::setAuthorIndex(QMultiHash<QString, qint64> *index)
{
    authorIndex_ = index;
}

void RecordParser::parseContent()
{
    while(!reader->atEnd()){
        reader->readNext();
        if(reader->isEndElement() && reader->name()==name_){
            break;
        }
        if(reader->isStartElement() && reader->name()=="author"){
            QString author = reader->readElementText(QXmlStreamReader::ErrorOnUnexpectedElement);
            authors_.append(author);
//            qDebug()<<QThread::currentThread();
            if(authorIndex_ != nullptr){
                authorIndex_->insert(author, reader->characterOffset());
            }
        }
//        qDebug()<<reader->name();
    }
}