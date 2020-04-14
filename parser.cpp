#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include <QMap>
#include <QList>

Parser::Parser(QObject *parent)
    :QObject(parent)
{
    
}

void Parser::run()
{
    m_timing = QTime::currentTime();
    m_elapsedTime = 0;
    
    emit stateChanged(tr("Parsing start."));  
    
    parseInit();
    
    timeMark(tr("XML file read successful. (%1 ms)"));
    
    parse();
    
    timeMark(tr("XML file parse successful. (%1 ms)"));
    
    genIndex();
    
    timeMark(tr("Index file generated. (%1 ms)"));
    
    saveAuthors();
    
    timeMark(tr("Authors information saved. (%1 ms)"));
    
    StringRef::clean();
    
    indexFileSave();
    
    timeMark(tr("Index file saved. (%1 ms)"));
    
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(m_costMsecs);
    emit done();
}

void Parser::parse()
{
    quint32 x = 0;
    quint32 len = m_ref.r;
    
    // authorId starts from 0
    
    while (x < len){
        if (m_ref.startsWith("key=\"", x)) {
            x += 5;
            QVector<int> recordAuthorsId;
            while (x <= len) {
                if (x == len || m_ref.startsWith("key=\"", x + 1)) {
                    if (recordAuthorsId.size() > 1) {
                        m_authorsIdRelation.append(recordAuthorsId);
                    }
                    break;
                }
                if (m_ref[x] == '<') {
                    if (m_ref.startsWith("author", x + 1)) {
                        StringRef author = readElementText(m_ref, x);
                        QPair<int, int> *info;
                        if (m_authorInfo.contains(author)) {
                            info = &m_authorInfo[author];
                        } else {
                            info = &m_authorInfo[author];
                            info->first/*id*/ = m_totalAuthor;
                            ++m_totalAuthor;
                            m_authors.append(author);
                        }
                        ++info->second;
                        m_authorIndex.append(author);
                        recordAuthorsId.append(info->first/*id*/);
//                        qDebug() << author;
                    } else if (m_ref.startsWith("title", x + 1)) {
                        StringRef title = readElementText(m_ref, x);
                        m_titleIndex.append(title);
//                        qDebug() << title;
                    }/* else if (ref.startsWith("year", x + 1)) {
                        StringRef year = readElementText(ref, x);
//                        qDebug() << year;
                    }*/
                }
                ++x;
            }
        }
        ++x;
    }
    
    
}

StringRef Parser::readElementText(const StringRef &r, quint32 &from)
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

StringRef Parser::readElementAttr(const StringRef &r, quint32 from)
{
    quint32 i = from;
    while (r[i] != '\"') ++i;
    return r.mid(from, i - from);
}

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::timeMark(const QString &msg)
{
    m_costMsecs = m_timing.elapsed();
    emit stateChanged(msg.arg(m_costMsecs - m_elapsedTime));
    m_elapsedTime = m_costMsecs;
}

void Parser::parseInit()
{
    // read all file content
    StringRef::init(Util::getXmlFileName());
    m_ref.r = StringRef::s_len;
    
    m_authorIndex.clear();
    m_titleIndex.clear();
    m_authorInfo.clear();
    m_authors.clear();
    m_authorsIdRelation.clear();
    m_authorStac.clear();
    
    m_totalAuthor = 0;
}

void Parser::genIndex()
{
    QList<QPair<StringRef, int>> temp;
    temp.reserve(m_authorInfo.size());

    auto it = m_authorInfo.begin();
    while (it != m_authorInfo.end()) {
        temp.append(qMakePair(it.key(),it.value().second));
        it++;
    }

    std::sort(temp.begin(),temp.end(),sortByDesc);

    
    for (qint32 t=0; t<temp.size(); t++) {
        m_authorStac.append(qMakePair(temp[t].first.toString(), temp[t].second));
    }

    std::sort(m_authorIndex.begin(), m_authorIndex.end());
    std::sort(m_titleIndex.begin(), m_titleIndex.end());
}

void Parser::saveAuthors()
{
    QFile file;
    QTextStream textStream(&file);
    
    // Save authors to authors.txt
    // The author's ID in line x is x
    
    file.setFileName("authors.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    foreach (StringRef author, m_authors) {
        textStream << author.toString() << '\n';
    }
    file.close();
    
    // Save authors relation to authors_relation.txt
    file.setFileName("authors_relation.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    textStream << m_totalAuthor << '\n';
    for (auto relation : m_authorsIdRelation) {
        textStream << relation[0];
        for (int i = 1; i < relation.size(); ++i) {
            textStream << ' ' << relation[i];
        }
        textStream << '\n';
    }
    file.close();
    
}

void Parser::indexFileSave()
{
    QFile file;
    QDataStream dataStream(&file);
    
    file.setFileName("author.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, m_authorIndex){
        dataStream << i.l << i.r;
    }
    file.close();
    
    file.setFileName("title.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, m_titleIndex){
        dataStream << i.l << i.r;
    }
    file.close();

    file.setFileName("authorStac.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    int num = m_authorStac.size() <= 100 ? m_authorStac.size() : 100;
    for(int i = 0; i < num; i++){
        dataStream << m_authorStac[i].first << m_authorStac[i].second;
    }
    file.close();
}

