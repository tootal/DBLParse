#include "loader.h"

#include <QFile>
#include <QDataStream>
#include <QElapsedTimer>

#include "finder.h"

Loader::Loader(Finder *p_finder, QObject *parent)
    :QObject(parent)
{
    finder = p_finder;
}

void Loader::run()
{
    if(!Util::canLoad()) return ;
    QElapsedTimer timing;
    timing.start();
    
    {
        emit stateChanged(tr("Loading authorStac index..."));
        finder->authorStacs.clear();
        QFile file("authorStac.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream in(&file);
        in >> finder->authorStacs;
        emit authorStacLoadDone();
        file.close();
    }
    
    {
        emit stateChanged(tr("Loading author index..."));
        QFile file("author.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream stream(&file);
        stream >> finder->authorIndexs;
        file.close();
        emit authorLoadDone();
    }
    
    {
        emit stateChanged(tr("Loading title index..."));
        QFile file("title.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream stream(&file);
        stream >> finder->titleIndexs;
        file.close();
        emit titleLoadDone();
    }
    
    {
        emit stateChanged(tr("Loading year word index..."));
        QFile file("yearWord.txt");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream s(&file);
        s >> finder->yearWord;
        emit yearWordLoadDone();
        file.close();
    }
    
    {
        emit stateChanged(tr("Loading title word index..."));
        finder->titleWords.clear();
        QFile file("words.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream s(&file);
        s >> finder->titleWords;
        emit titleWordLoadDone();
        file.close();
    }
    
    int ms = timing.elapsed();
    qInfo() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
