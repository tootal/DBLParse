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
#include "reader.h"

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
    timing.restart();
    elapsedTime = 0;
    
    emit stateChanged(tr("Parsing start."));  
    
    parseInit();
    
    timeMark(tr("XML file read successful."));
    
    parse();
    
    timeMark(tr("XML file parse successful."));
    
    parse2();
    
    timeMark(tr("XML file parse successful. (reader)"));
    
    countWordPerYear();
    
    timeMark(tr("The title of each year has been analyzed."));
    
    saveTitleWordIndex();
    
    timeMark(tr("The word of each title has been analyzed."));
    
    genIndex();
    
    timeMark(tr("Index file generated."));
    
    StringRef::clean();
    
    saveAuthors();
    
    timeMark(tr("Authors information saved."));
    
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(costMsecs);
    emit done();
}

void Parser::parse()
{
    quint32 x = 0;
    quint32 len = ref.r;
    
    // authorId starts from 0
    while (x < len) {
        if (ref.startsWith("key=\"", x)) {
            x += 5;
            QVector<int> recordAuthorsId;
            StringRef title;
            int year = 0;
            while (x <= len) {
                if (x == len || ref.startsWith("key=\"", x + 1)) {
                    if (recordAuthorsId.size() > 1) {
                        authorIdRelations.append(recordAuthorsId);
                    }
                    break;
                }
                if (ref[x] == '<') {
                    if (ref.startsWith("author", x + 1)) {
                        StringRef author = readElementText(ref, x);
                        AuthorInfo *info;
                        if (authorInfos.contains(author)) {
                            info = &authorInfos[author];
                        } else {
                            info = &authorInfos[author];
                            info->id = totalAuthor;
                            ++totalAuthor;
                        }
                        ++info->stac;
                        authorIndexs.append(author);
                        recordAuthorsId.append(info->id);
                    } else if (ref.startsWith("title", x + 1)) {
                        title = readElementText(ref, x);
                    } else if (ref.startsWith("year", x + 1)) {
                        year = readYear(ref, x);
                    }
                }
                ++x;
            }
        }
        ++x;
    }
}

void Parser::parse2()
{
    Reader reader(Util::getXmlFileName());
    while (reader.next()) {
        titleIndexs.append({reader.title(), reader.begin(), reader.end()});
        if (reader.hasYear()) {
            titleYears.append({reader.title(), reader.year()});
            minYear = std::min(minYear, reader.year());
            maxYear = std::max(maxYear, reader.year());
        }
    }
    if (reader.hasError()) {
        emit error(reader.error());
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

void Parser::timeMark(QString msg)
{
    costMsecs = timing.elapsed();
    msg += " " + tr("(%1 ms)").arg(costMsecs - elapsedTime);
    emit stateChanged(msg);
    elapsedTime = costMsecs;
}

void Parser::parseInit()
{
    // read all file content
    StringRef::init(Util::getXmlFileName());
    ref.r = StringRef::s_len;
    totalAuthor = 0;
    minYear = 2222;
    maxYear = 0;
}

void Parser::countWordPerYear()
{
    static const int TOP_K = 100;
    
    QVector<QVector<QString>> yearWords(maxYear - minYear + 1);
    for (const auto &titleYear : titleYears) {
        int year_n = titleYear.year - minYear;
        QString title = titleYear.title;
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
                topK.emplace(word, count);
            } else {
                if (topK.begin()->count < count) {
                    topK.erase(topK.begin());
                    topK.emplace(word, count);
                }
            }
        }
        if (!topK.empty()) {
            auto &res = topKWords[minYear + i];
            for (auto &cw : topK) {
                QString word = cw.word;
                word.remove(QRegularExpression(R"(</?.*?/?>)"));
                res.append({word, cw.count});
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
    s << topKWords;
    file.close();
}

void Parser::saveTitleWordIndex()
{
    QVector<WordPos> words;
    for (const auto &titleIndex : titleIndexs) {
        QString t = titleIndex.title;
        for (auto &noNeedChar : noNeedChars) {
            t.remove(noNeedChar);
        }
        QStringList ws = t.split(' ');
        for (QString &w : ws) {
            if (w.size() <= 2) continue;
            w = w.toLower();
            if (commonwords.contains(w)) continue;
            words.append({w, titleIndex.end});
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
    auto it = authorInfos.begin();
    while (it != authorInfos.end()) {
        authorStacs.append({it.key().toString(),it.value().stac});
        it++;
    }
    std::sort(authorStacs.begin(), authorStacs.end());
    std::sort(authorIndexs.begin(), authorIndexs.end());
    std::sort(titleIndexs.begin(), titleIndexs.end());
    
    QFile file;
    QDataStream dataStream(&file);
    
    file.setFileName("author.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    dataStream << authorIndexs;
    file.close();
    
    file.setFileName("title.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    dataStream << titleIndexs;
    file.close();

    file.setFileName("authorStac.dat");
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    if (authorStacs.size() > 100) authorStacs.resize(100);
    dataStream << authorStacs;
    file.close();
}

void Parser::saveAuthors()
{
    QFile file;
    QTextStream s(&file);
    
    int n = totalAuthor;
    qInfo() << "(Graph) number of nodes:" << n; 
    QVector<QVector<int>> G(n);
    for (auto &r : authorIdRelations) {
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

QDataStream &operator<<(QDataStream &out, const WordCount &wc)
{
    out << wc.word << wc.count;
    return out;
}

QDataStream &operator>>(QDataStream &in, WordCount &wc)
{
    in >> wc.word >> wc.count;
    return in;
}

QDataStream &operator<<(QDataStream &out, const WordPos &wp)
{
    out << wp.word << wp.pos;
    return out;
}

QDataStream &operator>>(QDataStream &in, WordPos &wp)
{
    in >> wp.word >> wp.pos;
    return in;
}

QDataStream &operator<<(QDataStream &out, const AuthorStac &as)
{
    out << as.author << as.stac;
    return out;
}

QDataStream &operator>>(QDataStream &in, AuthorStac &as)
{
    in >> as.author >> as.stac;
    return in;
}

QDataStream &operator<<(QDataStream &out, const TitleIndex &x)
{
    out << x.title << x.begin << x.end;
    return out;
}

QDataStream &operator>>(QDataStream &in, TitleIndex &x)
{
    in >> x.title >> x.begin >> x.end;
    return in;
}
