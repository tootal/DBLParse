#include "recordparser.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QMultiMap>
#include <QThread>
#include <QSettings>
#include <QFile>

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

void RecordParser::setAuthorIndex(QHash<QString, QVariant> *index)
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


RecordParser* RecordParser::fromFile(QFile *file, qint64 pos)
{
    RecordParser *recordParser = nullptr;
    QStringList recordNames;
    QSettings settings;
    Q_ASSERT(settings.contains("recordNames"));
    recordNames = settings.value("recordNames").toStringList();
    QString recordName;
    int lineOffset = -1;
    if(file->open(QFile::ReadOnly|QFile::Text)){
        file->seek(pos);
        while(!file->atEnd()){
//            qDebug()<<"fromFile findEndElement";
            QString s = file->readLine();
            qDebug()<<s;
            foreach(recordName, recordNames){
                lineOffset = s.indexOf(QString("</%1>").arg(recordName));
                if(lineOffset !=-1){
                    break;
                }
            }
            if(lineOffset != -1){
//                qDebug()<<lineOffset;
//                qDebug()<<pos;
//                qDebug()<<file->pos();
//                qDebug()<<"recordName = "<<recordName;
                break;
            }
        }
        pos = file->pos();
        qint64 leftPos = 0;
        if(pos > 2000) leftPos = pos-2000;
        file->seek(leftPos);
        char *data = new char[pos-leftPos+10];
        file->read(data, pos-leftPos+5);
        QString dataStr(data);
//        qDebug()<<"dataStr = "<<dataStr;
        int beginPos = dataStr.lastIndexOf(QString("<%1").arg(recordName));
//        qDebug()<<"beginPos = "<<beginPos;
        QString endEleStr = QString("</%1>").arg(recordName);
        int endPos = dataStr.lastIndexOf(endEleStr);
        QString recordStr = dataStr.mid(beginPos, endPos - beginPos + endEleStr.size());
        recordParser = fromStr(recordStr);
    }
    file->close();
    return recordParser;
}

RecordParser *RecordParser::fromStr(QString str)
{
    QXmlStreamReader reader;
    reader.addData(str);
    while(!reader.isStartElement()){
        reader.readNext();
    }
    RecordParser *recordParser=new RecordParser(&reader);
    recordParser->parse();
    return recordParser;
}
