#ifndef FINDER_H
#define FINDER_H

#include <QObject>

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);
    
signals:
    
};

#endif // FINDER_H
