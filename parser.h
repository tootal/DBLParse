#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>
#include <QTime>
#include <QXmlStreamReader>
#include <QDebug>

#include "util.h"
#include "stringref.h"

class Parser : public QObject
{
    Q_OBJECT
    
public:
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
    QTime m_timing;
    int m_costMsecs;
    int m_elapsedTime;
    int m_totalAuthor;
    
    StringRef m_ref;
    
    QVector<StringRef> m_authorIndex;
    QVector<StringRef> m_titleIndex;
    QMap<StringRef, QPair<int/*id*/, int/*stac*/>> m_authorInfo;
    QVector<StringRef> m_authors;
    QVector<QVector<int>> m_authorsIdRelation;
    QVector<QPair<QString/*author*/, int/*stac*/>> m_authorStac;
    
    int m_maxYear;
    int m_minYear;
    QVector<QPair<StringRef/*title*/, int/*year*/>> m_titleYear;
    typedef QPair<int/*count*/, QString/*word*/> CW_T;
    QMap<int/*year*/, QVector<CW_T>> m_topKWords; 
    
    void timeMark(const QString &msg);
    void parseInit();
    void countWordPerYear();
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
};

#endif // PARSER_H
