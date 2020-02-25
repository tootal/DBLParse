#include "parser.h"
#include "recordparser.h"

#include <QXmlStreamReader>
#include <QDebug>

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    
}

void Parser::clear()
{
    count_ = 0;
    abortFlag = false;
}

void Parser::run()
{
    parseTiming.start();
    while(!reader->atEnd()){
        reader->readNext();
        if(reader->isStartDocument()){
            setDocumentVersion(reader->documentVersion().toString());
            setDocumentEncoding(reader->documentEncoding().toString());
        }else if(reader->isDTD()){
//                qDebug()<<reader->text();
//                qDebug()<<reader->notationDeclarations().size();
//                qDebug()<<reader->entityDeclarations().size();
//                qDebug()<<reader->dtdName();
//                qDebug()<<reader->dtdPublicId();
//                qDebug()<<reader->dtdSystemId();
            setDtdName(reader->dtdName().toString());
            setDtdSystemId(reader->dtdSystemId().toString());
        }else if(reader->isStartElement()){
//                qDebug()<<reader->namespaceUri();
//                qDebug()<<reader->name();
//                qDebug()<<reader->attributes().size();
//                qDebug()<<reader->namespaceDeclarations().size();
            if(reader->name() == dtdName()){
                parseRecords();
            }
            break;
        }
    }
//    qDebug()<<"Parser: work()";
    parseCostMsecs_ = parseTiming.elapsed();
    emit done(this);
//    qDebug()<<authorIndex_.size();
}

QString Parser::documentVersion() const
{
    return documentVersion_;
}

void Parser::setDocumentVersion(QString s)
{
    documentVersion_ = s;
}

QString Parser::documentEncoding() const
{
    return documentEncoding_;
}

QString Parser::dtdName() const
{
    return dtdName_;
}

QString Parser::dtdSystemId() const
{
    return dtdSystemId_;
}

int Parser::count() const
{
    return count_;
}

int Parser::count(QString recordName) const
{
    return recordCount_[recordName];
}

QStringList Parser::authorNames() const
{
    return QStringList();
//    return authorIndex_.uniqueKeys();
}

int Parser::authorCount() const
{
//    return authorIndex_.uniqueKeys().size();
    return int();
}

QStringList Parser::recordNames() const
{
    return recordCount_.uniqueKeys();
}

QMap<QString, int> Parser::recordCount()
{
    return recordCount_;
}

void Parser::abortParse()
{
    abortFlag = true;
}

QTime Parser::parseCostTime()
{
    return QTime::fromMSecsSinceStartOfDay(parseCostMsecs_);
}

void Parser::setReader(QXmlStreamReader *r)
{
    reader = r;
}

void Parser::setDocumentEncoding(QString s)
{
    documentEncoding_ = s;
}

void Parser::setDtdName(QString s)
{
    dtdName_ = s;
}

void Parser::setDtdSystemId(QString s)
{
    dtdSystemId_ = s;
}

void Parser::parseRecords()
{
    while(!reader->atEnd()){
        if(abortFlag){
            emit done(this);
            break;
        }
        reader->readNext();
        if(reader->isEndElement()){
            if(reader->name() == dtdName()){
                break;
            }
        }else if(reader->isStartElement()){
//            qDebug()<<reader->namespaceUri();
//            qDebug()<<reader->name();
//            qDebug()<<reader->attributes().size();
//            qDebug()<<reader->namespaceDeclarations().size();
            recordCount_[reader->name().toString()]++;
            count_++;
            emit countChanged((double)reader->device()->pos()/reader->device()->size());
            reader->readElementText(QXmlStreamReader::SkipChildElements);
//            RecordParser recordParser(reader);
//            recordParser.setAuthorIndex(&authorIndex_);
        }
//        qDebug()<<"Parser: name = "<<reader->name();
//        qDebug()<<"Parser: token = "<<reader->tokenString();
    }
}
