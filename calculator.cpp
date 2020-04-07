#include "calculator.h"

#include <QDebug>

Calculator::Calculator(QObject *parent) : QObject(parent)
{
    
}

void Calculator::calc()
{
    qDebug() << "start calc";
    emit resultReady();
}
