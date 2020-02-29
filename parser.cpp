#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QBuffer>

Parser::Parser(const QString &fileName)
{
    m_fileName = fileName;
    m_status = "init";
    m_authorIndexFileName = "authorIndex.dat";
    m_titleIndexFileName = "titleIndex.dat";
    m_maxAuthorNameLength = 0;
}

void Parser::parse()
{
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
    QBuffer authorIndexBuffer;
    authorIndexBuffer.open(QBuffer::ReadWrite);
    QDataStream authorStream(&authorIndexBuffer);
    QBuffer titleIndexBuffer;
    titleIndexBuffer.open(QBuffer::ReadWrite);
    QDataStream titleStream(&titleIndexBuffer);
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.name() == "author"){
                QString author = reader.readElementText();
                if(author.size() > m_maxAuthorNameLength){
                    m_maxAuthorNameLength = author.size();
                }
                foreach(QChar c, author){
                    m_authorNameCharCount[c]++;
                }
                authorStream << author << reader.characterOffset();
                if(authorIndexBuffer.size() > c_maxIndexBufferSize){
                    QFile indexFile(m_authorIndexFileName);
                    if(!indexFile.open(QFile::Append)){
                        m_status = "author index file open failed";
                        return ;
                    }
                    indexFile.write(authorIndexBuffer.data());
                    indexFile.close();
                    authorIndexBuffer.buffer().clear();
                }
            }else if(reader.name() == "title"){
                QString title = reader.readElementText();
                titleStream << title << reader.characterOffset();
                if(titleIndexBuffer.size() > c_maxIndexBufferSize){
                    QFile indexFile(m_titleIndexFileName);
                    if(!indexFile.open(QFile::Append)){
                        m_status = "title index file open failed";
                        return ;
                    }
                    indexFile.write(titleIndexBuffer.data());
                    indexFile.close();
                    titleIndexBuffer.buffer().clear();
                }
            }
        }
    }
    if(reader.hasError()){
        m_status = "parse error : " + reader.errorString();
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
