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
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    char *data = new char[static_cast<quint64>(file.size())];
    qint64 len = file.read(data, file.size());
    Q_ASSERT(len > 0);
    qint64 x = 0;
    while(x < len){
        if(data[x] == '<'){
            if(Util::startsWith(data, "author", x + 1)){
                QByteArray author = Util::readElementText(data, x);
                qDebug() << "author: " << author;
                m_authorIndex.append(qMakePair(author, x));
            }else if(Util::startsWith(data, "title", x + 1)){
                QByteArray title = Util::readElementText(data, x);
                qDebug() << "title: " << title;
                m_titleIndex.append(qMakePair(title, x));
            }
        }
        ++x;
    }
    delete[] data;
    file.close();
    m_costMsecs = m_timing.elapsed();
    if(!m_abortFlag) m_parsed = true;
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
//                if(author.size() > m_maxAuthorLength){
//                    m_maxAuthorLength = author.size();
//                }
//                foreach(QChar c, author){
//                    auto &v = m_authorCharCount[c];
//                    v = v.toInt() + 1;
//                }
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
//    return m_authorIndex.values(author);
    return {};
}

QList<QVariant> Parser::indexOfTitle(const QString &title) const
{
//    return m_titleIndex.values(title);
    return {};
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
