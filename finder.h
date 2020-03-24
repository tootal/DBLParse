#ifndef FINDER_H
#define FINDER_H

#include <QObject>

#include "parser.h"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);
    Q_INVOKABLE void find(const QString &word, const QString &type);
    static bool parsed();
    static void clearIndex();
    static void init();
    QList<quint32> indexOfAuthor(const QString &author) const;
    QList<quint32> indexOfTitle(const QString &title) const;
    
private:
    static QFile *s_file;
    static Parser::StringRef *s_authorIndex;
    static Parser::StringRef *s_titleIndex;
    static quint32 s_authorIndexs;
    static quint32 s_titleIndexs;
    static QString readText(const Parser::StringRef &ref);
    // thanks to stl algo
    static QPair<const Parser::StringRef*, const Parser::StringRef*>
            equalRange(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
    static const Parser::StringRef*
            lowerBound(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
    static const Parser::StringRef*
            upperBound(const Parser::StringRef *first,
                                         const Parser::StringRef *last,
                                         const QString &val);
};

#endif // FINDER_H
