#ifndef FINDER_H
#define FINDER_H

#include <QObject>

#include "parser.h"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);
    static bool parsed();
    static void init();
    QList<quint32> indexOfAuthor(const QString &author) const;
    QList<quint32> indexOfTitle(const QString &title) const;
    
private:
    static QFile *s_file;
    static Parser::StringRef *s_authorIndex;
    static Parser::StringRef *s_titleIndex;
    static quint32 s_authorIndexs;
    static quint32 s_titleIndexs;
};

#endif // FINDER_H
