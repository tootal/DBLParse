#pragma once

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QXmlStreamReader>
#include <QDebug>

#include "util.h"
#include "stringref.h"

struct LinkedList;

class Parser : public QObject
{
    Q_OBJECT
    
public:
    typedef QPair<int/*count*/, QString/*word*/> WordCount; // Word Count Type
    typedef QMap<int/*year*/, QVector<WordCount>> YearWord; // Year Word Type
    typedef QPair<QString/*word*/, quint32/*pos*/> WordPos; // Word Pos Type
    
    Parser(QObject *parent = nullptr);
    void run();
    void parse();
    int costMsecs();
    
signals:
    void stateChanged(const QString &state);
    void done();
    
public:
    static bool sortByDesc(QPair<StringRef, int> l,QPair<StringRef,int> r){return l.second>r.second;}
    
private:
    QElapsedTimer m_timing;
    int m_costMsecs{};
    int m_elapsedTime{};
    int m_totalAuthor{};
    
    StringRef m_ref;
    
    QVector<StringRef> m_authorIndex;
    QVector<StringRef> m_titleIndex;
    QMap<StringRef, QPair<int/*id*/, int/*stac*/>> m_authorInfo;
    QVector<QVector<int>> m_authorsIdRelation;
    QVector<QPair<QString/*author*/, int/*stac*/>> m_authorStac;
    
    int m_maxYear{};
    int m_minYear{};
    QVector<QPair<StringRef/*title*/, int/*year*/>> m_titleYear;
    YearWord m_topKWords;
    
    void timeMark(QString msg);
    void parseInit();
    void countWordPerYear();
    void saveYearWord();
    void saveTitleWordIndex();
    void genIndex();
    void saveAuthors();
    void indexFileSave();
    void parseClean();

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
