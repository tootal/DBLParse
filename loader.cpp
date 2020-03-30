#include "loader.h"
#include "finder.h"

#include <QFile>
#include <QDataStream>

Loader::Loader(QObject *parent) 
    : QThread(parent)
{
    
}

void Loader::run()
{
    if(!Finder::parsed()) return ;
    QTime timing;
    timing.start();
    emit stateChanged(tr("Loading author index..."));
    QFile file("author.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    Finder::s_authorIndexs = static_cast<quint32>(file.size() >> 3);
    Finder::s_authorIndex = new Parser::StringRef[Finder::s_authorIndexs];
    for(quint32 i = 0; i < Finder::s_authorIndexs; ++i){
        stream >> Finder::s_authorIndex[i].l >> Finder::s_authorIndex[i].r;
    }
    file.close();
    emit stateChanged(tr("Loading title index..."));
    file.setFileName("title.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    Finder::s_titleIndexs = static_cast<quint32>(file.size() >> 3);
    Finder::s_titleIndex = new Parser::StringRef[Finder::s_titleIndexs];
    for(quint32 i = 0; i < Finder::s_titleIndexs; ++i){
        stream >> Finder::s_titleIndex[i].l >> Finder::s_titleIndex[i].r;
    }
    file.close();
    emit stateChanged(tr("Loading key index..."));
    file.setFileName("key.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    Finder::s_keyIndexs = static_cast<quint32>(file.size() >> 3);
    Finder::s_keyIndex = new Parser::StringRef[Finder::s_keyIndexs];
    for(quint32 i = 0; i < Finder::s_keyIndexs; ++i){
        stream >> Finder::s_keyIndex[i].l >> Finder::s_keyIndex[i].r;
    }
    file.close();

    Finder::authorStac.clear();
    emit stateChanged(tr("Loading authorStac index..."));
    file.setFileName("authorStac.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
//    stream.setDevice(&file);
     QDataStream in(&file);
    QPair<QString,int>   tempAuthorStac;
    while(!in.atEnd()){
        in >> tempAuthorStac.first >> tempAuthorStac.second;
        Finder::authorStac.append(tempAuthorStac);
    }
    file.close();
    int ms = timing.elapsed();
    qDebug() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
