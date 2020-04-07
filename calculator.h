#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

public slots:
    void calc();
    
signals:
    void resultReady();
};

#endif // CALCULATOR_H
