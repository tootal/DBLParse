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
    
    QFile file;
    QDataStream stream;
    
    emit stateChanged(tr("Loading authorStac index..."));
    Finder::s_authorStac.clear();
    file.setFileName("authorStac.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
//    stream.setDevice(&file);
     QDataStream in(&file);
    QPair<QString,int>   tempAuthorStac;
    while(!in.atEnd()){
        in >> tempAuthorStac.first >> tempAuthorStac.second;
        Finder::s_authorStac.append(tempAuthorStac);
    }
    emit authorStacLoadDone();
    file.close();
    
    emit stateChanged(tr("Loading author index..."));
    file.setFileName("author.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    Finder::s_authorIndexs = static_cast<quint32>(file.size() >> 3);
    Finder::s_authorIndex = new StringRef[Finder::s_authorIndexs];
    for(quint32 i = 0; i < Finder::s_authorIndexs; ++i){
        stream >> Finder::s_authorIndex[i].l >> Finder::s_authorIndex[i].r;
    }
    file.close();
    emit authorLoadDone();
    
    emit stateChanged(tr("Loading title index..."));
    file.setFileName("title.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    Finder::s_titleIndexs = static_cast<quint32>(file.size() >> 3);
    Finder::s_titleIndex = new StringRef[Finder::s_titleIndexs];
    for(quint32 i = 0; i < Finder::s_titleIndexs; ++i){
        stream >> Finder::s_titleIndex[i].l >> Finder::s_titleIndex[i].r;
    }
    file.close();
    emit titleLoadDone();
    
    emit stateChanged(tr("Loading yearWord index..."));
    Finder::s_yearWord.clear();
    file.setFileName("yearWord.txt");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
//    stream.setDevice(&file);
    QTextStream read(&file);
    QPair<int,QList<QString> > tempYearWord;
    while(!read.atEnd()){
        QString line = read.readLine();
        QList<QString> temp;
        temp=line.split(' ');
        tempYearWord.first = temp[0].toInt();
        temp.pop_front();
        temp.pop_back();
        tempYearWord.second = temp;
        Finder::s_yearWord.insert(tempYearWord.first,tempYearWord.second);
    }
//    qDebug()<<Finder::s_yearWord;
    emit yearWordLoadDone();
    file.close();
    
    int ms = timing.elapsed();
    qInfo() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
