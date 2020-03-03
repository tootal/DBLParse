#include "finder.h"

#include <QFile>
#include <QDataStream>

Parser::StringRef *Finder::s_authorIndex = nullptr;
Parser::StringRef *Finder::s_titleIndex = nullptr;
quint32 Finder::s_authorIndexs = 0;
quint32 Finder::s_titleIndexs = 0;
QFile *Finder::s_file = nullptr;

Finder::Finder(QObject *parent) : QObject(parent)
{
    
}

bool Finder::parsed()
{
    return QFile("author.dat").exists() && QFile("title.dat").exists();
}

QList<quint32> Finder::indexOfAuthor(const QString &author) const
{
    if(s_authorIndex == nullptr){
        return QList<quint32>{};
    }
    
}

QList<quint32> Finder::indexOfTitle(const QString &title) const
{
    if(s_titleIndex == nullptr){
        return QList<quint32>{};
    }
    
}

void Finder::init()
{
    Q_ASSERT(parsed());
    QFile file("author.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    s_authorIndexs = static_cast<quint32>(file.size() >> 3);
    s_authorIndex = new Parser::StringRef[s_authorIndexs];
    for(quint32 i = 0; i < s_authorIndexs; ++i){
        stream >> s_authorIndex[i].l >> s_authorIndex[i].r;
    }
    file.close();
    file.setFileName("title.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    s_titleIndexs = static_cast<quint32>(file.size() >> 3);
    s_titleIndex = new Parser::StringRef[s_titleIndexs];
    for(quint32 i = 0; i < s_titleIndexs; ++i){
        stream >> s_titleIndex[i].l >> s_titleIndex[i].r;
    }
    file.close();
    if(s_file != nullptr){
        if(s_file->isOpen()) s_file->close();
        delete s_file;
        s_file = nullptr;
    }
}
