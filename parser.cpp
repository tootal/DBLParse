#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>

char *Parser::s_data;
Parser::StringRef *Parser::s_authorIndex;
Parser::StringRef *Parser::s_titleIndex;
quint32 Parser::s_authorIndexs = 0;
quint32 Parser::s_titleIndexs = 0;

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
    s_data = new char[static_cast<quint64>(file.size())];
    quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
    StringRef ref(0, len);
    file.close();
    s_authorIndex = new StringRef[1<<24|1<<19];
    s_titleIndex = new StringRef[1<<23];
    quint32 x = 0;
    while(x < len){
        if(ref[x] == '<'){
            if(ref.startsWith("author", x + 1)){
                StringRef author = readElementText(ref, x);
                ++m_authorCount;
                s_authorIndex[s_authorIndexs] = author;
                ++s_authorIndexs;
//                qDebug() << author;
            }else if(ref.startsWith("title", x + 1)){
                StringRef title = readElementText(ref, x);
                s_titleIndex[s_titleIndexs] = title;
                ++s_titleIndexs;
//                qDebug() << title;
            }
        }
        ++x;
    }
    std::sort(s_titleIndex, s_titleIndex + s_titleIndexs);
    std::sort(s_authorIndex, s_authorIndex + s_authorIndexs);
    
    file.setFileName("author.dat");
    QDataStream stream(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    for(quint32 i = 0; i < s_authorIndexs; ++i){
        stream << s_authorIndex[i].l << s_authorIndex[i].r;
    }
    file.close();
    
    file.setFileName("title.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    for(quint32 i = 0; i < s_titleIndexs; ++i){
        stream << s_titleIndex[i].l << s_titleIndex[i].r;
    }
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

Parser::StringRef Parser::readElementText(const Parser::StringRef &r, quint32 &from)
{
    StringRef s = r.mid(from);
    Q_ASSERT(s[0] == '<');
    quint32 i = 1;
    char name[30];
    name[0] = '<';
    name[1] = '/';
    while(s[i] != ' ' && s[i] != '>'){
        name[i + 1] = s[i];
        ++i;
    }
    from += i;
    name[i + 1] = '>';
    name[i + 2] = 0;
    // name = "</ele>"
    while(s[i] != '>') ++i;
    qint32 p = s.indexOf(name, i + 1);
    Q_ASSERT(p != -1);
    quint32 x = static_cast<quint32>(p);
    from += x + 1;
    return s.mid(i + 1, x - i - 1);
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

char &Parser::StringRef::operator[](quint32 x) const
{
    Q_ASSERT(0 <= x && x < r - l);
    return s_data[l + x];
}

bool Parser::StringRef::operator<(const Parser::StringRef &s) const
{
    quint32 len = r - l;
    if(s.r - s.l < len) len = s.r - s.l;
    for(quint32 i = 0; i < len; ++i){
        if(s_data[l + i] != s_data[s.l + i]){
            return s_data[l + i] < s_data[s.l + i];
        }
    }
    if(len == s.r - s.l) return false;
    else return true;
}

Parser::StringRef Parser::StringRef::mid(quint32 pos) const
{
    
    Q_ASSERT(0 <= pos && pos < r - l);
    return StringRef(l + pos, r);
}

Parser::StringRef Parser::StringRef::mid(quint32 pos, quint32 len) const
{
    Q_ASSERT(0 <= pos && pos < r - l);
    Q_ASSERT(pos + len <= r - l);
    return StringRef(l + pos, l + pos + len);
}

bool Parser::StringRef::startsWith(const char *str, quint32 from) const
{
    quint32 x = l + from;
    while(*str != 0){
        if(*str != s_data[x]) return false;
        ++str;
        ++x;
        if(x >= r) return false;
    }
    return true;
}

qint32 Parser::StringRef::indexOf(const char *str, quint32 from) const
{
    for(quint32 t = l + from; t < r; ++t){
        bool flag = true;
        for(const char *x = str; *x != 0; ++x){
            if(*x != s_data[t + x - str]){
                flag = false;
                break;
            }
        }
        if(flag){
            return static_cast<qint32>(t - l);
        }
    }
    return -1;
}
