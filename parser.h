#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>
#include <QTime>
#include <QXmlStreamReader>
#include <QDebug>

#include "util.h"
#include "stringref.h"

class QFile;

class Parser : public QObject
{
    Q_OBJECT
    
public:
    Parser(QObject *parent = nullptr);
    void run();
    void parse();
    int costMsecs();
    static void clearIndex();
    
signals:
    void stateChanged(const QString &state);
    void done();
    
public:
    static bool sortByDesc(QPair<StringRef, int> l,QPair<StringRef,int> r){return l.second>r.second;}
    
private:
    QTime m_timing;
    int m_costMsecs;
    int m_elapsedTime;
    
    void timeMark(const QString &msg);

    // treat child element as text, after reading, from equal to the
    // last position of the end element
    static StringRef readElementText(const StringRef &r, quint32 &from);
    
    // read element attribute, return its value of StringRef
    // from equal to the start position of the attr value
    static StringRef readElementAttr(const StringRef &r, quint32 from);

};

#endif // PARSER_H
