#pragma once

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDataStream>

#include "util.h"
#include "stringref.h"

struct LinkedList;

struct WordCount
{
    QString word;
    int count;
    WordCount(const QString &w = QString(), int c = int())
        :word(w), count(c) {}
    bool operator<(const WordCount &that) const {
        return count < that.count;
    }
};

QDataStream &operator<<(QDataStream &out, const WordCount &wc);

QDataStream &operator>>(QDataStream &in, WordCount &wc);

struct WordPos
{
    QString word;
    quint32 pos;
    WordPos(const QString &w = QString(), quint32 p = quint32())
        :word(w), pos(p) {}
    bool operator<(const WordPos &that) const {
        return word < that.word;
    }
};

QDataStream &operator<<(QDataStream &out, const WordPos &wp);

QDataStream &operator>>(QDataStream &in, WordPos &wp);

typedef QMap<int/*year*/, QVector<WordCount>> YearWord;

class Parser : public QObject
{
    Q_OBJECT
    
public:
    Parser(QObject *parent = nullptr);
    void run();
    void parse();
    
signals:
    void stateChanged(const QString &state);
    void done();
    
public:
    static bool sortByDesc(QPair<StringRef, int> l,QPair<StringRef,int> r){return l.second>r.second;}
    
private:
    QElapsedTimer timing;
    int costMsecs{};
    int elapsedTime{};
    int totalAuthor{};
    
    StringRef ref;
    
    QVector<StringRef> authorIndexs;
    QVector<StringRef> titleIndexs;
    QMap<StringRef, QPair<int/*id*/, int/*stac*/>> authorInfos;
    QVector<QVector<int>> authorIdRelations;
    QVector<QPair<QString/*author*/, int/*stac*/>> authorStacs;
    
    int maxYear{};
    int minYear{};
    QVector<QPair<StringRef/*title*/, int/*year*/>> titleYears;
    YearWord topKWords;
    
    void timeMark(QString msg);
    void parseInit();
    void countWordPerYear();
    void saveYearWord();
    void saveTitleWordIndex();
    void genIndex();
    void saveAuthors();

    // treat child element as text, after reading, from equal to the
    // last position of the end element
    static StringRef readElementText(const StringRef &r, quint32 &from);
    
    // read element attribute, return its value of StringRef
    // from equal to the start position of the attr value
    static StringRef readElementAttr(const StringRef &r, quint32 from);

    // read example: "<year>1998</year>"
    static int readYear(const StringRef &r, quint32 &from);

    static const QStringList commonwords;
    static const QString noNeedChars;
};
