#include "loader.h"

#include <QFile>
#include <QDataStream>
#include <QElapsedTimer>

#include "finder.h"

Loader::Loader(QObject *parent) 
    : QThread(parent)
{
    
}

void Loader::run()
{
    if(!Util::canLoad()) return ;
    QElapsedTimer timing;
    timing.start();
    
    {
        emit stateChanged(tr("Loading authorStac index..."));
        Finder::s_authorStac.clear();
        QFile file("authorStac.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream in(&file);
        QPair<QString,int>   tempAuthorStac;
        while (!in.atEnd()) {
            in >> tempAuthorStac.first >> tempAuthorStac.second;
            Finder::s_authorStac.append(tempAuthorStac);
        }
        emit authorStacLoadDone();
        file.close();
    }
    
    {
        emit stateChanged(tr("Loading author index..."));
        Finder::authorIndexs.clear();
        QFile file("author.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream stream(&file);
        auto len = static_cast<quint32>(file.size() >> 3);
        for (quint32 i = 0; i < len; ++i) {
            StringRef t;
            stream >> t.l >> t.r;
            Finder::authorIndexs.append(t);
        }
        file.close();
        emit authorLoadDone();
    }
    
    {
        emit stateChanged(tr("Loading title index..."));
        QFile file("title.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream stream(&file);
        Finder::s_titleIndexs = static_cast<quint32>(file.size() >> 3);
        if (Finder::s_titleIndex) delete [] Finder::s_titleIndex;
        Finder::s_titleIndex = new StringRef[Finder::s_titleIndexs];
        for (quint32 i = 0; i < Finder::s_titleIndexs; ++i) {
            stream >> Finder::s_titleIndex[i].l >> Finder::s_titleIndex[i].r;
        }
        file.close();
        emit titleLoadDone();
    }
    
    {
        emit stateChanged(tr("Loading year word index..."));
        Finder::s_yearWord.clear();
        QFile file("yearWord.txt");
        file.open(QFile::ReadOnly | QFile::Text);
        Q_ASSERT(file.isOpen());
        QTextStream read(&file);
        while (!read.atEnd()) {
            QStringList temp = read.readLine().split(' ');
            QVector<Parser::WordCount> tempYearWord;
            int year = temp[0].toInt();
            for (int i = 1; i < temp.size() - 1; i += 2) {
                tempYearWord.append(qMakePair(
                    temp[i].toInt()/*count*/,
                    temp[i + 1]/*word*/
                ));
            }
            Finder::s_yearWord.insert(year, tempYearWord);
        }
        emit yearWordLoadDone();
        file.close();
    }
    
    {
        emit stateChanged(tr("Loading title word index..."));
        Finder::s_titleWords.clear();
        QFile file("words.dat");
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        QDataStream s(&file);
        s >> Finder::s_titleWords;
        emit titleWordLoadDone();
        file.close();
    }
    
    int ms = timing.elapsed();
    qInfo() << QString("load finished in %1 ms").arg(ms);
    emit loadDone();
}
