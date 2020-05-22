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
//    qDebug("Parser destruct");
}

void Parser::run()
{
//    qDebug("Parser run thread: %d", QThread::currentThreadId());
    timing.start();
    elapsedTime = 0;
    
    qInfo() << "Parsing start: " << Util::getXmlFileName(); 
    
    parse();
    
    timeMark("XML file parse successful.");
    
    countWordPerYear();
    
    timeMark("The title of each year has been analyzed.");
    
    saveAuthors();
    
    timeMark("Authors information saved.");
    
    qInfo() << QString("Parse done. Cost: %1 ms").arg(costMsecs);
    emit stateChanged(100);
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
    int records{};
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
                G.append(QVector<int>());
            }
            ++info->stac;
            recordAuthorsId.append(info->id);
            authorSaver.save(hash1, { hash2, reader.begin()});
        }
        if (reader.title() != "Home Page")
            titleSaver.save(Hash::hash1(reader.title()), 
                            {Hash::hash2(reader.title()), reader.begin()});
        auto words = Stemmer::stem(reader.title());
        for (const auto &word : words) {
            wordSaver.save(Hash::hash1(word),
                           { Hash::hash2(word), reader.begin()});
        }
        {
            const auto &r = recordAuthorsId;
            for (int i = 0; i < r.size() - 1; i++) {
                for (int j = i + 1; j < r.size(); j++) {
                    if (r[i] == r[j]) continue;
                    G[r[i]].append(r[j]);
                    G[r[j]].append(r[i]);
                }
            }
        }
        if (reader.hasYear()) {
            int year_n = reader.year() - MIN_YEAR;
            for (const auto &word : words) {
                yearWords[year_n].append(word);
            }
        }
        if (reader.newBlock()) {
            int state = 50.0f * reader.end() / reader.size();
            emit stateChanged(state);
        }
        ++records;
    }
    emit stateChanged(50);
    qInfo() << "Authors: " << authorInfos.size();
    qInfo() << "Records: " << records;
    QVector<AuthorStac> authorStacs(authorInfos.size());
    {
        int i = 0;
        auto it = authorInfos.begin();
        while (it != authorInfos.end()) {
            authorStacs[i] = {it.key(),it.value().stac};
            ++it;
            ++i;
        }
    }
    emit stateChanged(53);
    std::sort(authorStacs.begin(), authorStacs.end());
    emit stateChanged(57);
    {
        QFile file("data/authorstac");
        QDataStream dataStream(&file);
        file.open(QFile::WriteOnly);
        if (authorStacs.size() > 100) authorStacs.resize(100);
        dataStream << authorStacs;
        file.close();
        authorInfos.clear();
    }
    emit stateChanged(60);
}

void Parser::timeMark(QString msg)
{
    costMsecs = timing.elapsed();
    msg += QString(" (%1 ms)").arg(costMsecs - elapsedTime);
    qInfo() << msg;
    elapsedTime = costMsecs;
}

void Parser::countWordPerYear()
{
    YearWord topKWords;
    int sz = yearWords.size();
    for (int i = 0; i < sz; ++i) {
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
        emit stateChanged(60 + 10.0 * i / sz);
    }
    QFile file("data/yearword");
    QDataStream s(&file);
    file.open(QFile::WriteOnly);
    s << topKWords;
    file.close();
    yearWords.clear();
    yearWords.squeeze();
    emit stateChanged(70);
}

void Parser::saveAuthors()
{
    int n = totalAuthor;
    qInfo() << "(Graph) number of nodes:" << n; 
    for (auto &i : G) {
        std::sort(i.begin(), i.end());
        i.erase(std::unique(i.begin(), i.end()), i.end());
    }
    emit stateChanged(72);
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
    emit stateChanged(74);
    G.clear();
    G.squeeze();
    emit stateChanged(75);
    runAndPrintStatsCliques(adjList, n);
    emit stateChanged(95);
    for (int i = 0; i < n; i++)
        destroyLinkedList(adjList[i]);
    free(adjList);
    emit stateChanged(98);
}

QDataStream &operator<<(QDataStream &out, const WordCount &x)
{
    out << x.word << x.count;
    return out;
}

QDataStream &operator>>(QDataStream &in, WordCount &x)
{
    in >> x.word >> x.count;
    return in;
}

QDataStream &operator<<(QDataStream &out, const AuthorStac &x)
{
    out << x.author << x.stac;
    return out;
}

QDataStream &operator>>(QDataStream &in, AuthorStac &x)
{
    in >> x.author >> x.stac;
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
