#pragma once

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDataStream>

#include "util.h"

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

typedef QMap<int/*year*/, QVector<WordCount>> YearWord;

struct AuthorInfo
{
    int id;
    int stac;
};

struct AuthorStac
{
    QByteArray author;
    int stac;
    bool operator<(const AuthorStac &that) const {
        return stac > that.stac;
    }
};

QDataStream &operator<<(QDataStream &out, const AuthorStac &as);

QDataStream &operator>>(QDataStream &in, AuthorStac &as);

struct TitleYear
{
    QByteArray title;
    int year;
};

struct HashIndex
{
    quint32 hash;
    quint32 pos;
};

QDataStream &operator<<(QDataStream &out, const HashIndex &x);

QDataStream &operator>>(QDataStream &in, HashIndex &x);


class Parser : public QObject
{
    Q_OBJECT
    
public:
    Parser(QObject *parent = nullptr);
    ~Parser();
    
public slots:
    void run();
    
protected:
    bool event(QEvent *event) override;
    
signals:
    void stateChanged(const QString &state);
    void done();
    void error(const QString &msg);
    
private:
    QElapsedTimer timing;
    int costMsecs{};
    int elapsedTime{};
    int totalAuthor{};
        
    QMap<QByteArray, AuthorInfo> authorInfos;
    QVector<QVector<int>> authorIdRelations;
    QVector<AuthorStac> authorStacs;
    
    int maxYear{};
    int minYear{};
    QVector<TitleYear> titleYears;
    YearWord topKWords;
    
    void timeMark(QString msg);
    void parse();
    void countWordPerYear();
    void genIndex();
    void saveAuthors();
};
