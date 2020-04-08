#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QVector>
#include <QMap>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

public slots:
    void calc();
    
private:
    QVector<QVector<int>> G;
    QMap<int, int> cnt;
    QVector<bool> visited;
    void enumerateAllCliques();
    
signals:
    void resultReady();
};

#endif // CALCULATOR_H
