#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>

const QStringList Parser::c_recordNames = {
    "article",
   "inproceedings",
   "proceedings",
   "book",
   "incollection",
   "phdthesis",
   "mastersthesis",
   "www",
   "person",
   "data"
};

Parser::Parser()
{
    m_cancelFlag = false;
    m_status = "init";
    m_authorIndexFileName = "authorIndex.dat";
    m_titleIndexFileName = "titleIndex.dat";
    m_maxAuthorNameLength = 0;
    m_recordCount = 0;
}

void Parser::parse(const QString &fileName)
{
//    qDebug() << "parse start";
    m_fileName = fileName;
    if(m_fileName.isEmpty()){
        m_status = "fileName is empty";
        return ;
    }
    QFile file(m_fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        m_status = "file open failed";
        return ;
    }
    QXmlStreamReader reader(&file);
    QFile authorFile(m_authorIndexFileName);
    if(!authorFile.open(QIODevice::WriteOnly)){
        m_status = "author file open failed";
        return ;
    }
    QDataStream authorStream(&authorFile);
    QFile titleFile(m_titleIndexFileName);
    if(!titleFile.open(QIODevice::WriteOnly)){
        m_status = "title file open failed";
        return ;
    }
    QDataStream titleStream(&titleFile);
    while(!reader.atEnd()){
        reader.readNext();
//        qDebug() << "current token: " << reader.tokenString();
        if(m_cancelFlag) break;
        if(reader.isStartElement()){
            if(c_recordNames.contains(reader.name())){
                emit posChanged(static_cast<double>(reader.device()->pos())/reader.device()->size());   
                ++m_recordCount;
//                qDebug() << "record count : " << m_recordCount;
            }
            if(reader.name() == "author"){
                QString author = reader.readElementText();
//                qDebug() << "author: " << author;
                if(author.size() > m_maxAuthorNameLength){
                    m_maxAuthorNameLength = author.size();
                }
                foreach(QChar c, author){
                    m_authorNameCharCount[c]++;
                }
//                qDebug() << "author name char count : " << m_authorNameCharCount;
                authorStream << author << reader.characterOffset();
//                if((m_recordCount & c_flushMask) == 0){
//                    authorFile.flush();
//                }
            }else if(reader.name() == "title"){
//              ATTENTION EXAMPLE : <title>Fully Persistent B<sup>+</sup>-trees</title>
                QString title = reader.readElementText(QXmlStreamReader::IncludeChildElements);
//                qDebug() << "title : " << title;
                titleStream << title << reader.characterOffset();
//                if((m_recordCount & c_flushMask) == 0){
//                    titleFile.flush();
//                }
            }
        }
    }
    authorFile.close();
    titleFile.close();
    if(reader.hasError()){
        m_status = "parse error : " + reader.errorString();
        qDebug() << "error line number : " << reader.lineNumber();
    }else{
        m_status = "ok";
    }
    file.close();
    emit parseDone();
}

QString Parser::status() const
{
    return m_status;
}

QString Parser::authorIndexFileName() const
{
    return m_authorIndexFileName;
}

void Parser::setAuthorIndexFileName(const QString &fileName)
{
    m_authorIndexFileName = fileName;
}

QString Parser::titleIndexFileName() const
{
    return m_titleIndexFileName;
}

void Parser::setTitleIndexFileName(const QString &fileName)
{
    m_titleIndexFileName = fileName;
}

int Parser::maxAuthorNameLength() const
{
    return m_maxAuthorNameLength;
}

QMap<QChar, int> Parser::authorNameCharCount() const
{
    return m_authorNameCharCount;
}

QTime Parser::costTime() const
{
    return QTime::fromMSecsSinceStartOfDay(m_costMsecs);
}

void Parser::cancel()
{
    m_cancelFlag = true;
}
