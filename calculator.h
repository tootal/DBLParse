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
    explicit Calculator(QObject *parent = nullptr);

public slots:
    void calc();
    
private:
    QVector<QVector<int>> G;
    QMap<int, int> cnt;
    void enumerateAllCliques();
    void connectedComponents();
    void cutBridges();
    
signals:
    void resultReady();
};

#endif // CALCULATOR_H
