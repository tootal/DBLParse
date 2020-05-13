#include "parser.h"

#include <QFile>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QRegularExpression>

#include <set>

#include "LinkedList.h"
#include "misc.h"

const QStringList Parser::commonwords = {
    "are", "all", "any", "been", "both",
    "each", "either", "one", "two", "three",
    "four", "five", "six", "seven", "eight",
    "nine", "ten", "none", "little", "few",
    "many", "much", "other", "another", "some",
    "every", "nobody", "anybody", "somebody",
    "everybody", "when", "under", "uuml",
    "first", "second", "third", "forth", "fifth",
    "sixth", "seventh", "above", "over", "below",
    "under", "beside", "behind", "the", "after",
    "from", "since", "for", "which", "next",
    "where", "how", "who", "there", "was", "der",
    "were", "did", "done", "this", "that",
    "last", "brfore", "because", "against",
    "except", "beyond", "along", "among", "but",
    "towards", "you", "multi", "time", "von",
    "your", "his", "her", "she", "its", "they",
    "them", "and", "has", "have", "had",
    "would", "then", "too", "our", "off",
    "into", "weel", "can", "being", "zur",
    "been", "having", "even", "these", "those",
    "ours", "with", "use", "using", "used",
    "the", "based", "problem", "problems", "systems",
    "methods", "ways", "ideas", "learning", "information",
    "works", "solve", "solving", "solved", "old", "new",
    "analysis", "data", "big", "small", "large",
    "their", "between", "method"
};

const QString Parser::noNeedChars = ":,.?()";

Parser::Parser(QObject *parent)
    :QObject(parent)
{
    
}

void Parser::run()
{
    m_timing.restart();
    m_elapsedTime = 0;
    
    emit stateChanged(tr("Parsing start."));  
    
    parseInit();
    
    timeMark(tr("XML file read successful."));
    
    parse();
    
    timeMark(tr("XML file parse successful."));
    
    countWordPerYear();
    
    timeMark(tr("The title of each year has been analyzed."));
    
    saveTitleWordIndex();
    
    timeMark(tr("The word of each title has been analyzed."));
    
    genIndex();
    
    timeMark(tr("Index file generated."));
    
    StringRef::clean();
    
    saveAuthors();
    
    timeMark(tr("Authors information saved."));
    
    indexFileSave();
    
    timeMark(tr("Index file saved."));
    
    parseClean();
    
    timeMark(tr("Memeory cleaned."));
    
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(m_costMsecs);
    emit done();
}

void Parser::parse()
{
    quint32 x = 0;
    quint32 len = m_ref.r;
    
    // authorId starts from 0
    while (x < len) {
        if (m_ref.startsWith("key=\"", x)) {
            x += 5;
            QVector<int> recordAuthorsId;
            StringRef title;
            int year = 0;
            while (x <= len) {
                if (x == len || m_ref.startsWith("key=\"", x + 1)) {
                    if (recordAuthorsId.size() > 1) {
                        m_authorsIdRelation.append(recordAuthorsId);
                    }
                    if (year != 0) {
                        m_titleYear.append(qMakePair(title, year));
                        m_minYear = std::min(m_minYear, year);
                        m_maxYear = std::max(m_maxYear, year);
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
                        }
                        ++info->second;
                        m_authorIndex.append(author);
                        recordAuthorsId.append(info->first/*id*/);
                    } else if (m_ref.startsWith("title", x + 1)) {
                        title = readElementText(m_ref, x);
                        m_titleIndex.append(title);
                    } else if (m_ref.startsWith("year", x + 1)) {
                        year = readYear(m_ref, x);
                    }
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
    auto x = static_cast<quint32>(p);
    from += x + 1;
    return s.mid(i + 1, x - i - 1);
}

StringRef Parser::readElementAttr(const StringRef &r, quint32 from)
{
    quint32 i = from;
    while (r[i] != '\"') ++i;
    return r.mid(from, i - from);
}

int Parser::readYear(const StringRef &r, quint32 &from)
{
    from += 6;
    int year = 1000 * (r[from] - '0')
            + 100 * (r[from + 1] - '0')
            + 10 * (r[from + 2] - '0')
            + r[from + 3] - '0';
    from += 11;
    return year;
}

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::timeMark(QString msg)
{
    m_costMsecs = m_timing.elapsed();
    msg += " " + tr("(%1 ms)").arg(m_costMsecs - m_elapsedTime);
    emit stateChanged(msg);
    m_elapsedTime = m_costMsecs;
}

void Parser::parseInit()
{
    // read all file content
    StringRef::init(Util::getXmlFileName());
    m_ref.r = StringRef::s_len;
    m_totalAuthor = 0;
    m_minYear = 2222;
    m_maxYear = 0;
}

void Parser::countWordPerYear()
{
    static const int TOP_K = 100;
    
    QVector<QVector<QString>> yearWords(m_maxYear - m_minYear + 1);
    for (const auto &titleYear : m_titleYear) {
        int year_n = titleYear.second - m_minYear;
        QString title = titleYear.first.toString();
        for (const QChar &noNeedChar : noNeedChars) {
            title.remove(noNeedChar);
        }
        QStringList words = title.split(' ');
        for (QString &word : words) {
            if (word.size() <= 2) continue;
            word = word.toLower();
            if (commonwords.contains(word)) continue;
            yearWords[year_n].append(word);
        }
    }
    
    for (int i = 0; i < yearWords.size(); ++i) {
        auto &words = yearWords[i];
        std::sort(words.begin(), words.end());
        std::set<WordCount> topK;
        for (int j = 0; j < words.size();) {
            QString word = words[j];
            int count = 1;
            ++j;
            while (j < words.size() && word == words[j]) {
                ++count;
                ++j;
            }
            if (topK.size() < TOP_K) {
                topK.emplace(count, word);
            } else {
                if (topK.begin()->first/*count*/ < count) {
                    topK.erase(topK.begin());
                    topK.emplace(count, word);
                }
            }
        }
        if (!topK.empty()) {
            auto &res = m_topKWords[m_minYear + i];
            for (auto &cw : topK) {
                QString word = cw.second/*word*/;
                word.remove(QRegularExpression(R"(</?.*?/?>)"));
                res.append(qMakePair(cw.first/*count*/, word));
            }
        }
    }
    //    qDebug() << Util::str(m_topKWords);
    saveYearWord();
}

void Parser::saveYearWord()
{
    QFile file("yearWord.txt");
    QDataStream s(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    s << m_topKWords;
    file.close();
}

void Parser::saveTitleWordIndex()
{
    QVector<WordPos> words;
    for (const auto &title : m_titleIndex) {
        QString t = title.toString();
        for (auto &noNeedChar : noNeedChars) {
            t.remove(noNeedChar);
        }
        QStringList ws = t.split(' ');
        for (QString &w : ws) {
            if (w.size() <= 2) continue;
            w = w.toLower();
            if (commonwords.contains(w)) continue;
            words.append(qMakePair(w, title.l));
        }
    }
    std::sort(words.begin(), words.end());
    {
        QFile file("words.dat");
        file.open(QFile::WriteOnly);
        QDataStream s(&file);
        s << words;
        file.close();
    }
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

    
    for (auto i : temp) {
        m_authorStac.append(qMakePair(i.first.toString(), i.second));
    }

    std::sort(m_authorIndex.begin(), m_authorIndex.end());
    std::sort(m_titleIndex.begin(), m_titleIndex.end());
}

void Parser::saveAuthors()
{
    QFile file;
    QTextStream s(&file);
    
    int n = m_totalAuthor;
    qInfo() << "(Graph) number of nodes:" << n; 
    QVector<QVector<int>> G(n);
    for (auto &r : m_authorsIdRelation) {
        for (int i = 0; i < r.size() - 1; i++) {
            for (int j = i + 1; j < r.size(); j++) {
                if (r[i] == r[j]) continue;
                G[r[i]].append(r[j]);
                G[r[j]].append(r[i]);
            }
        }
    }
    for (auto &i : G) {
        std::sort(i.begin(), i.end());
        i.erase(std::unique(i.begin(), i.end()), i.end());
    }
    
    int m = 0;
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
            if (v <= u) continue;
            m++;
        }
    }
    qInfo() << "(Graph) number of edges:" << m;
    LinkedList** adjList = (LinkedList**)calloc(n, sizeof(LinkedList*));
    for (int i = 0; i < n; i++)
        adjList[i] = createLinkedList();
    for (int u = 0; u < n; ++u) {
        for (int v : G[u]) {
            if (v <= u) continue;
            addLast(adjList[u], v);
            addLast(adjList[v], u);
        }
    }
    m = m * 2;
    populate_nCr();
    runAndPrintStatsCliques(adjList, n);
    for (int i = 0; i < n; i++)
        destroyLinkedList(adjList[i]);
    free(adjList);
}

void Parser::indexFileSave()
{
    QFile file;
    QDataStream dataStream(&file);
    
    file.setFileName("author.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    dataStream << m_authorIndex;
    file.close();
    
    file.setFileName("title.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    dataStream << m_titleIndex;
    file.close();

    file.setFileName("authorStac.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    int num = m_authorStac.size() <= 100 ? m_authorStac.size() : 100;
    for (int i = 0; i < num; i++) {
        dataStream << m_authorStac[i].first << m_authorStac[i].second;
    }
    file.close();
}

void Parser::parseClean()
{
    m_authorIndex.clear();
    m_titleIndex.clear();
    m_authorInfo.clear();
    m_authorsIdRelation.clear();
    m_authorStac.clear();
    m_topKWords.clear();
    
    m_authorIndex.squeeze();
    m_titleIndex.squeeze();
    m_authorsIdRelation.squeeze();
    m_authorStac.squeeze();
}
