#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include<QMap>
#include <QList>

char *Parser::s_data;
QList<QPair<QString,int> > Parser::s_authorStac;

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    
}

void Parser::run()
{
    parse();
}

void Parser::parse()
{
    QTime timing;
    timing.start();
    emit stateChanged(tr("Parsing start."));
    int elapsedTime = 0;
    
    QFile file;
    QTextStream textStream(&file);
    QDataStream dataStream(&file);
    
    file.setFileName(Util::getXmlFileName());
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    s_data = new char[static_cast<quint64>(file.size())];
    quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
    StringRef ref(0, len);
    file.close();
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("XML file read successful. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;
    QVector<StringRef> authorIndex;
    QVector<StringRef> titleIndex;
    QVector<StringRef> keyIndex;
    quint32 x = 0;
    QMap<StringRef,int> authorStacTemp;
    int totalAuthor = 0;
    // authorId starts from 1
    QMap<StringRef, int> authorId;
    QVector<StringRef> authors;
    QVector<QStringList> authorsIdRelation;
    while (x < len){
        if (ref.startsWith("key=\"", x)) {
            x += 5;
            StringRef key;
            key.l = x;
            while(ref[x] != '\"') ++x;
            key.r = x;
            keyIndex.append(key);
//            qDebug() << "--record--";
//            qDebug() << key;
            ++x;
            QStringList recordAuthorsId;
            while (x <= len) {
                if (x == len || ref.startsWith("key=\"", x + 1)) {
                    if (recordAuthorsId.size() > 1) {
                        authorsIdRelation.append(recordAuthorsId);
                    }
                    break;
                }
                if (ref[x] == '<') {
                    if (ref.startsWith("author", x + 1)) {
                        StringRef author = readElementText(ref, x);
                        if (!authorId.contains(author)) {
                            ++totalAuthor;
                            authorId[author] = totalAuthor;
                            authors.append(author);
                        }
                        if(!authorStacTemp.contains(author)) {
                            authorStacTemp.insert(author,1);
                        } else {
                            authorStacTemp.insert(author,authorStacTemp.find(author).value()+1);
//                            qDebug()<<s_authorStacTemp.find(author).key();
                        }
                        authorIndex.append(author);
                        recordAuthorsId.append(QString::number(authorId[author]));
//                        qDebug() << author;
                    } else if (ref.startsWith("title", x + 1)) {
                        StringRef title = readElementText(ref, x);
                        titleIndex.append(title);
//                        qDebug() << title;
                    } else if (ref.startsWith("year", x + 1)) {
                        StringRef year = readElementText(ref, x);
//                        qDebug() << year;
                    }
                }
                ++x;
            }
        }
        ++x;
    }
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("XML file parse successful. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;

    // Save authors to authors.txt
    // The author's ID in line x is x
    
    file.setFileName("authors.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    foreach (StringRef author, authors) {
        textStream << author.toString() << '\n';
    }
    file.close();
    
    // Save authors relation to authors_relation.txt
    file.setFileName("authors_relation.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    textStream << totalAuthor << '\n';
    foreach (QStringList relation, authorsIdRelation) {
        textStream << relation.join(' ') << '\n';
    }
    file.close();

    QList<QPair<Parser::StringRef,int> > temp;

    QMap<StringRef, int>::iterator it=authorStacTemp.begin();
    while(it!=authorStacTemp.end()){
        temp.append(qMakePair(it.key(),it.value()));
        it++;
    }

    authorStacTemp.clear();

    std::sort(temp.begin(),temp.end(),sortByDesc);

    s_authorStac.clear();

    for(qint32 t=0;t<temp.size();t++){
        s_authorStac.append(qMakePair(temp[t].first.toString(),temp[t].second));
    }

    std::sort(authorIndex.begin(), authorIndex.end());
    std::sort(titleIndex.begin(), titleIndex.end());
    std::sort(keyIndex.begin(), keyIndex.end());
    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("Index file generated. (%1 ms)").arg(m_costMsecs - elapsedTime));
    elapsedTime = m_costMsecs;
    delete[] s_data;
    file.setFileName("author.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, authorIndex){
        dataStream << i.l << i.r;
    }
    file.close();
    
    file.setFileName("title.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, titleIndex){
        dataStream << i.l << i.r;
    }
    file.close();
    
    file.setFileName("key.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, keyIndex){
        dataStream << i.l << i.r;
    }
    file.close();

    file.setFileName("authorStac.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    int num = s_authorStac.size()<=100 ? s_authorStac.size() : 100;
    for(int i=0;i<num;i++){
        dataStream << s_authorStac[i].first << s_authorStac[i].second;
    }
    file.close();
    s_authorStac.clear();

    m_costMsecs = timing.elapsed();
    emit stateChanged(tr("Index file saved. (%1 ms)").arg(m_costMsecs - elapsedTime));
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(m_costMsecs);
    emit done();
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
    QFile("authorStac.dat").remove();
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
