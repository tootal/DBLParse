#include "finder.h"

#include <QFile>
#include <QDataStream>
#include <QSettings>

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
    Q_ASSERT(s_file != nullptr);
    Q_ASSERT(s_file->isOpen());
    
}

QList<quint32> Finder::indexOfTitle(const QString &title) const
{
    if(s_titleIndex == nullptr){
        return QList<quint32>{};
    }
    
}

QString Finder::readText(const Parser::StringRef &ref)
{
    s_file->seek(ref.l);
    return s_file->read(ref.r - ref.l);
}

QPair<const Parser::StringRef *, const Parser::StringRef *> 
Finder::equalRange(const Parser::StringRef *begin, const Parser::StringRef *end, const QString &key)
{
    Q_ASSERT(begin <= end);
    quint32 len = static_cast<quint32>(end - begin);
    while(len > 0){
        quint32 half = len >> 1;
        const Parser::StringRef *mid = begin + half;
        QString text = readText(*mid);
        int cmp = QString::compare(text, key);
        if(cmp < 0){
            begin = mid;
            ++begin;
            len = len - half - 1;
        }else if(cmp > 0){
            len = half;
        }else{
            
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
    
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    QString fileName = settings.value("lastOpenFileName").toString();
    s_file = new QFile(fileName);
    s_file->open(QFile::ReadOnly);
}
