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
    QElapsedTimer timing;
    timing.start();
    
    {
        emit stateChanged(tr("Loading authorStac index..."));
        finder->authorStacs.clear();
        QFile file("data/authorstac");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream in(&file);
        in >> finder->authorStacs;
        emit authorStacLoadDone();
        file.close();
    }
    
    {
        emit stateChanged(tr("Loading year word index..."));
        QFile file("data/yearword");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream s(&file);
        s >> finder->yearWord;
        emit yearWordLoadDone();
        file.close();
    }
    
    int ms = timing.elapsed();
    qInfo() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
