#include "parser.h"
#include "util.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    m_aFile.resize(256);
    clear();
}

void Parser::setAction(const QString &action)
{
    Q_ASSERT(action == "parse"|action == "reload");
    m_action = action;
}

void Parser::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Parser::run()
{
    if(m_action == "parse"){
        parse();
    }else{
        load();
    }
}

void Parser::parse()
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
    if(!m_abortFlag) m_parsed = true;
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

int Parser::authorCount() const
{
    return m_authorCount;
}

void Parser::clear()
{
    m_count = 0;
    m_authorCount = 0;
    m_abortFlag = false;
    m_parsed = false;
    m_action = "parse";
    m_recordCount.clear();
    m_authorCharCount.clear();
    m_maxAuthorLength = 0;
    m_authorIndex.clear();
    m_titleIndex.clear();
    
    for(int i = 0; i < 256; ++i){
        if(m_aFile[i] == nullptr){
            m_aFile[i] = new QFile("dblp"+QString::number(i)+".dat");
        }
        if(!m_aFile[i]->isOpen()){
            m_aFile[i]->open(QFile::WriteOnly);
        }
    }
}

bool Parser::parsed() const
{
    return m_parsed;
}

void Parser::parseRecords()
{
    Q_ASSERT(reader.isStartElement());
    while(!reader.atEnd()){
        if(m_abortFlag){
            emit done(this);
            break;
        }
        reader.readNext();
        if(reader.isEndElement() && reader.name() == "dblp") break;
        if(reader.isStartElement()){
            auto &r1 = m_recordCount[reader.name().toString()];
            r1 = r1.toInt() + 1;
            ++m_count;
            emit countChanged(static_cast<double>(reader.device()->pos())/reader.device()->size());
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
                ++m_authorCount;
                if(author.size() > m_maxAuthorLength){
                    m_maxAuthorLength = author.size();
                }
                foreach(QChar c, author){
                    auto &v = m_authorCharCount[c];
                    v = v.toInt() + 1;
                }
//                qDebug() << m_authorCharCount;
//                m_authorIndex.insertMulti(author, reader.characterOffset());
//                qDebug() << Util::hash(author);
                int h = Util::hash(author);
                Q_ASSERT(m_aFile[h]);
                Q_ASSERT(m_aFile[h]->isOpen());
                QByteArray data(author.toUtf8());
//                qDebug() << "author string size = " << author.size();
//                qDebug() << "author byte size = " << data.size();
                data.resize(100);
//                qDebug() << data;
                m_aFile[h]->write(data);
                m_aFile[h]->write(QByteArray::number(reader.characterOffset()));
            }else if(reader.name() == "title"){
                QString title = reader.readElementText(QXmlStreamReader::IncludeChildElements);
//                m_titleIndex.insertMulti(title, reader.characterOffset());
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
           << m_authorCount
           << m_authorCharCount
           << m_parsed
           << m_recordCount
           << m_authorIndex
           << m_titleIndex
           << m_maxAuthorLength
    ;
//    qDebug() << m_costMsecs;
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
           >> m_authorCount
           >> m_authorCharCount
           >> m_parsed
           >> m_recordCount
           >> m_authorIndex
           >> m_titleIndex
           >> m_maxAuthorLength
    ;
//    qDebug() << m_costMsecs;
    file.close();
    m_parsed = true;
    emit loadDone();
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

const QMap<QString, QVariant> &Parser::recordCount() const
{
    return m_recordCount;
}

int Parser::maxAuthorLength() const
{
    return m_maxAuthorLength;
}

const QMap<QString, QVariant> &Parser::authorCharCount() const
{
//    qDebug() << m_authorCharCount;
    return m_authorCharCount;
}
