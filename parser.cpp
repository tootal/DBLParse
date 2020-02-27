#include "parser.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    clear();
}

void Parser::clear()
{
    count_ = 0;
    abortFlag = false;
    reader = nullptr;
    parsed_ = false;
    recordCount_.clear();
    authorIndex_.clear();
    parseCostMsecs_ = 0;
}

bool Parser::parsed() const
{
    return parsed_;
}

void Parser::setParsed(bool p)
{
    parsed_ = p;
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
//    qDebug()<<"author index: "<<authorIndex_;
    parseCostMsecs_ = parseTiming.elapsed();
    emit done(this);
    parsed_ = true;
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
    return recordCount_[recordName].toInt();
}

void Parser::setCount(int cnt)
{
    count_ = cnt;
}

QStringList Parser::recordNames() const
{
    return recordCount_.uniqueKeys();
}

QMap<QString, QVariant> Parser::recordCount() const
{
    return recordCount_;
}

void Parser::setRecordCount(QMap<QString, QVariant> x)
{
    recordCount_ = x;
}

QMap<QString, QVariant> Parser::authorIndex() const
{
    return authorIndex_;
}

void Parser::setAuthorIndex(QMap<QString, QVariant> x)
{
    authorIndex_ = x;
}

void Parser::addRecordCount(QString recordName, int value)
{
    int oldValue = recordCount_[recordName].toInt();
    recordCount_[recordName] = oldValue + value;
}

void Parser::abortParse()
{
    abortFlag = true;
}

int Parser::parseCostMsecs() const
{
    return parseCostMsecs_;
}

void Parser::setParseCostMsecs(int x)
{
    parseCostMsecs_ = x;
}

QTime Parser::parseCostTime() const
{
    return QTime::fromMSecsSinceStartOfDay(parseCostMsecs_);
}

QList<QVariant> Parser::getOffsetsByAuthorName(QString authorName) const
{
//    QList<qint64> list;
//    auto &v = authorIndex_;
//    auto i = v.find(authorName);
//    while(i != v.end() && i.key() == authorName){
//        list.append(i.value());
//        ++i;
//    }
//    return list;
    return authorIndex_.values(authorName);
}

void Parser::setReader(QXmlStreamReader *r)
{
    reader = r;
}

bool Parser::hasReader() const
{
    return reader!=nullptr;
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
            addRecordCount(reader->name().toString());
            count_++;
            emit countChanged((double)reader->device()->pos()/reader->device()->size());
//            reader->readElementText(QXmlStreamReader::SkipChildElements);
            RecordParser recordParser(reader);
//            qDebug()<<recordParser.authors();
            recordParser.setAuthorIndex(&authorIndex_);
            recordParser.parse();
        }
//        qDebug()<<"Parser: name = "<<reader->name();
//        qDebug()<<"Parser: token = "<<reader->tokenString();
    }
}
