#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    clear();
}

void Parser::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Parser::run()
{
    m_timing.start();
    Q_ASSERT(!m_fileName.isEmpty());
    QFile file(m_fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
    reader.setDevice(&file);
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isStartDocument()){
            // pass
        }else if(reader.isDTD()){
            // pass
        }else if(reader.isStartElement()){
            parseRecords();
            break;
        }
    }
    Q_ASSERT(!reader.hasError());
    file.close();
    m_costMsecs = m_timing.elapsed();
    m_parsed = true;
    save();
    emit done(this);
}

QString Parser::fileName() const
{
    return m_fileName;
}

int Parser::costMsecs() const
{
    return m_costMsecs;
}

int Parser::count() const
{
    return m_count;
}

void Parser::clear()
{
    m_count = 0;
    m_abortFlag = false;
    m_parsed = false;
    m_recordCount.clear();
    m_authorIndex.clear();
    m_titleIndex.clear();
}

bool Parser::parsed() const
{
    return m_parsed;
}

void Parser::parseRecords()
{
    Q_ASSERT(reader.isStartElement());
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isEndElement() && reader.name() == "dblp") break;
        if(reader.isStartElement()){
            auto &r1 = m_recordCount[reader.name().toString()];
            r1 = r1.toInt() + 1;
            ++m_count;
            emit countChanged((double)reader.device()->pos()/reader.device()->size());
            parseContent(reader.name());
        }
    }
}

void Parser::parseContent(QStringRef recordName)
{
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isEndElement() && reader.name() == recordName) break;
        if(reader.isStartElement()){
            if(reader.name() == "author"){
                QString author = reader.readElementText(QXmlStreamReader::IncludeChildElements);
                m_authorIndex.insertMulti(author, reader.characterOffset());
            }else if(reader.name() == "title"){
                QString title = reader.readElementText(QXmlStreamReader::IncludeChildElements);
                m_titleIndex.insertMulti(title, reader.characterOffset());
            }
        }
    }
}

void Parser::save()
{
    QFile file("dblp.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    stream << m_fileName 
           << m_costMsecs
           << m_count
           << m_parsed
           << m_recordCount
           << m_authorIndex
           << m_titleIndex;
    qDebug() << m_costMsecs;
    file.close();
}

void Parser::load()
{
    QFile file("dblp.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    stream >> m_fileName
           >> m_costMsecs
           >> m_count
           >> m_parsed
           >> m_recordCount
           >> m_authorIndex
           >> m_titleIndex;
    qDebug() << m_costMsecs;
    file.close();
    m_parsed = true;
}

void Parser::abortParser()
{
    m_abortFlag = true;
}

QList<QVariant> Parser::indexOfAuthor(const QString &author) const
{
    return m_authorIndex.values(author);
}

QList<QVariant> Parser::indexOfTitle(const QString &title) const
{
    return m_titleIndex.values(title);
}
