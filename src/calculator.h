#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QVector>
#include <QMap>

class Calculator : public QObject
{
    Q_OBJECT
public:
    typedef QVector<int> list;
    typedef QSet<int> set;
    typedef QStack<int> stack;
    explicit Calculator(QObject *parent = nullptr);

public slots:
    void calc();
    void handleAuthorRelations();
    void generateAuthorsEdges();
    
private:
    QVector<QVector<int>> G;
    QMap<int, int> cnt;
    int edges;
    
signals:
    void resultReady();
};

#endif // CALCULATOR_H
