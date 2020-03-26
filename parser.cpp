#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>

char *Parser::s_data;

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
    emit stateChanged(tr("XML file read successful. (%1 ms)").arg(m_timing.elapsed()));
    QVector<StringRef> authorIndex;
    QVector<StringRef> titleIndex;
    QVector<StringRef> keyIndex;
    quint32 x = 0;
    while(x < len){
        if(ref[x] == '<'){
            if(ref[x + 1] == '?'){ // document
                x += 2;
                while(ref[x - 1] != '>' || ref[x - 2] != '?') ++x;
            }else if(ref[x + 1] == '!'){ // dtd
                while(ref[x - 1] != '>') ++x;
            }else if(ref.startsWith("dblp", x + 1)){ // root element
                ++x;
                while(ref[x] != '<') ++x;
                while(x < len){
                    if(ref[x] == '<'){ // record element
                        ++x;
                        while(ref[x] != ' ' || ref[x] != '>') ++x;
                        while(!ref.startsWith("key=\"", x)) ++x;
                        x += 5;
                        StringRef key;
                        key.l = x;
                        while(ref[x] != '\"') ++x;
                        key.r = x;
                    }
                }
                if(startsIndex != -1){ // record start
                    StringRef key = readElementAttr(ref, x, "key");
                    keyIndex.append(key);
    //                qDebug() << key;
                }
                if(ref.startsWith("author", x + 1)){
                    StringRef author = readElementText(ref, x);
                    authorIndex.append(author);
    //                qDebug() << author;
                }else if(ref.startsWith("title", x + 1)){
                    StringRef title = readElementText(ref, x);
                    titleIndex.append(title);
    //                qDebug() << title;
                }
            }
        }
        ++x;
    }
    emit stateChanged(tr("XML file parse successful. (%1 ms)").arg(m_timing.elapsed()));
    std::sort(authorIndex.begin(), authorIndex.end());
    std::sort(titleIndex.begin(), titleIndex.end());
    std::sort(keyIndex.begin(), keyIndex.end());
    emit stateChanged(tr("Index file generated. (%1 ms)").arg(m_timing.elapsed()));
    delete[] s_data;
    file.setFileName("author.dat");
    QDataStream stream(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, authorIndex){
        stream << i.l << i.r;
    }
    file.close();
    file.setFileName("title.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, titleIndex){
        stream << i.l << i.r;
    }
    file.close();
    file.setFileName("key.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, keyIndex){
        stream << i.l << i.r;
    }
    file.close();
    emit stateChanged(tr("Index file saved. (%1 ms)").arg(m_timing.elapsed()));
    m_costMsecs = m_timing.elapsed();
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
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

Parser::StringRef Parser::readElementAttr(const Parser::StringRef &r, quint32 from, const char *key)
{
    StringRef s = r.mid(from);
    Q_ASSERT(s[0] == '<');
    quint32 i = 1;
    while(!s.startsWith(key, i)) ++i;
    while(s[i] != '\"') ++i;
    ++i;
    quint32 j = i;
    while(s[j] != '\"') ++j;
    return s.mid(i, j - i);
}

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::clearIndex()
{
    QFile("author.dat").remove();
    QFile("title.dat").remove();
    QFile("key.dat").remove();
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

int Parser::StringRef::startsWith(const QStringList &strs, quint32 from) const
{
    for(int i = 0; i < strs.length(); ++i){
        if(startsWith(strs[i].toUtf8(), from)){
            return i;
        }
    }
    return -1;
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

QString Parser::StringRef::toString() const
{
    return QByteArray::fromRawData(s_data + l, static_cast<int>(r - l));
}
