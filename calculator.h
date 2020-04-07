#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QVector>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

public slots:
    void calc();
    
private:
    QVector<QVector<int>> G;
    void printG();
    
signals:
    void resultReady();
};

#endif // CALCULATOR_H
