#include "finder.h"

#include <QFile>

Finder::Finder(QObject *parent) : QObject(parent)
{
    
}

bool Finder::parsed()
{
    return QFile("author.dat").exists() && QFile("title.dat").exists();
}
