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
    if(!Util::parsed()) return ;
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
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
//    stream.setDevice(&file);
    QTextStream read(&file);
    while (!read.atEnd()) {
        QStringList temp = read.readLine().split(' ');
//        qDebug() << temp;
        QVector<Parser::CW_T> tempYearWord;
        int year = temp[0].toInt();
        for (int i = 1; i < temp.size() - 1; i += 2) {
            tempYearWord.append(qMakePair(
                temp[i].toInt()/*count*/,
                temp[i + 1]/*word*/
            ));
        }
//        qDebug() << Util::str(tempYearWord);
        Finder::s_yearWord.insert(year, tempYearWord);
    }
//    qDebug()<<Finder::s_yearWord;
    emit yearWordLoadDone();
    file.close();
    
    int ms = timing.elapsed();
    qInfo() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
