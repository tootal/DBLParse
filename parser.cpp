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
    
}

void Parser::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Parser::run()
{
    parse();
}

void Parser::parse()
{
    m_timing.start();
    emit stateChanged(tr("Parsing start."));
    Q_ASSERT(!m_fileName.isEmpty());
    QFile file(m_fileName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    s_data = new char[static_cast<quint64>(file.size())];
    quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
    StringRef ref(0, len);
    file.close();
    emit stateChanged(tr("XML file read successful."));
    s_authorIndex = new StringRef[1<<24|1<<19];
    s_titleIndex = new StringRef[1<<23];
    quint32 x = 0;
    while(x < len){
        if(ref[x] == '<'){
            if(ref.startsWith("author", x + 1)){
                StringRef author = readElementText(ref, x);
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
    emit stateChanged(tr("XML file parse successful."));
    std::sort(s_titleIndex, s_titleIndex + s_titleIndexs);
    std::sort(s_authorIndex, s_authorIndex + s_authorIndexs);
    emit stateChanged(tr("Index file generated."));
    delete[] s_data;
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
    delete[] s_authorIndex;
    delete[] s_titleIndex;
    emit stateChanged(tr("Index file saved."));
    m_costMsecs = m_timing.elapsed();
    emit stateChanged(tr("Parse done."));
    emit done();
}

QString Parser::fileName() const
{
    return m_fileName;
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

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::clearIndex()
{
    QFile("author.dat").remove();
    QFile("title.dat").remove();
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
