#include "parser.h"

#include <QFile>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QRegularExpression>
#include <QThread>
#include <QEvent>
#include <QScopedArrayPointer>

#include <set>

#include "LinkedList.h"
#include "misc.h"
#include "reader.h"
#include "hash.h"
#include "saver.h"
#include "stemmer.h"

Parser::Parser(QObject *parent)
    :QObject(parent)
{
//    qDebug("Parser construct");
//    qDebug("Parser construct thread: %d", QThread::currentThreadId());
    totalAuthor = 0;
    yearWords.resize(MAX_YEAR - MIN_YEAR + 1);
}

Parser::~Parser()
{
    qDebug("Parser destruct");
}

void Parser::run()
{
//    qDebug("Parser run thread: %d", QThread::currentThreadId());
    timing.start();
    elapsedTime = 0;
    
    emit stateChanged(tr("Parsing start."));  
    
    parse();
    
    timeMark(tr("XML file parse successful."));
    
    countWordPerYear();
    
    timeMark(tr("The title of each year has been analyzed."));
    
    saveAuthors();
    
    timeMark(tr("Authors information saved."));
    
    emit stateChanged(tr("Parse done. Cost time: %1")
                      .arg(Util::formatTime(costMsecs)));
    qInfo() << QString("Parse done in %1 ms").arg(costMsecs);
    emit done();
}

bool Parser::event(QEvent *event)
{
//    if (event->type() == QEvent::DeferredDelete) {
//        qDebug("Parser deferred delete event");
//    }
    return QObject::event(event);
}

void Parser::parse()
{
    Util::initIndexs();
    Reader reader(Util::getXmlFileName());
    Saver titleSaver("title");
    Saver authorSaver("author");
    Saver wordSaver("word");
    QMap<QByteArray, AuthorInfo> authorInfos;
    while (reader.next()) {
        AuthorInfo *info;
        QVector<int> recordAuthorsId;
        for (const auto &author : reader.authors()) {
            auto hash1 = Hash::hash1(author);
            auto hash2 = Hash::hash2(author);
            if (authorInfos.contains(author)) {
                info = &authorInfos[author];
            } else {
                info = &authorInfos[author];
                info->id = totalAuthor;
                ++totalAuthor;
            }
            ++info->stac;
            recordAuthorsId.append(info->id);
            authorSaver.save(hash1, { hash2, reader.end()});
        }
        if (reader.title() != "Home Page")
            titleSaver.save(Hash::hash1(reader.title()), 
                            {Hash::hash2(reader.title()), reader.end()});
        auto words = Stemmer::stem(reader.title());
        for (const auto &word : words) {
            wordSaver.save(Hash::hash1(word),
                           { Hash::hash2(word), reader.end()});
        }
        authorIdRelations.append(recordAuthorsId);
        if (reader.hasYear()) {
            int year_n = reader.year() - MIN_YEAR;
            for (const auto &word : words) {
                yearWords[year_n].append(word);
            }
        }
    }
    if (reader.hasError()) {
        emit error(reader.error());
    }
    QVector<AuthorStac> authorStacs(authorInfos.size());
    auto it = authorInfos.begin();
    while (it != authorInfos.end()) {
        authorStacs.append({it.key(),it.value().stac});
        it++;
    }
    std::sort(authorStacs.begin(), authorStacs.end());
    {
        QFile file("authorStac.dat");
        QDataStream dataStream(&file);
        file.open(QFile::WriteOnly);
        if (authorStacs.size() > 100) authorStacs.resize(100);
        dataStream << authorStacs;
        file.close();
        authorInfos.clear();
    }
}

void Parser::timeMark(QString msg)
{
    costMsecs = timing.elapsed();
    msg += " " + tr("(%1 ms)").arg(costMsecs - elapsedTime);
    emit stateChanged(msg);
    elapsedTime = costMsecs;
}

void Parser::countWordPerYear()
{
    for (int i = 0; i < yearWords.size(); ++i) {
        auto &words = yearWords[i];
        std::sort(words.begin(), words.end());
        std::set<WordCount> topK;
        for (int j = 0; j < words.size();) {
            auto word = words[j];
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
            auto &res = topKWords[MIN_YEAR + i];
            for (auto &cw : topK) {
                auto word = cw.word;
                res.append({word, cw.count});
            }
        }
    }
    //    qDebug() << Util::str(m_topKWords);
    QFile file("yearWord.txt");
    QDataStream s(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    s << topKWords;
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

QDataStream &operator<<(QDataStream &out, const HashIndex &x)
{
    out << x.hash << x.pos;
    return out;
}

QDataStream &operator>>(QDataStream &in, HashIndex &x)
{
    in >> x.hash >> x.pos;
    return in;
}
