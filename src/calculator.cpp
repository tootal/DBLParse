#include "calculator.h"
#include "util.h"

#include "degeneracy_cliques.h"

#include <QDebug>
#include <QFile>
#include <QQueue>
#include <QTime>
#include <QStack>

#include <algorithm>

Calculator::Calculator(QObject *parent) : QObject(parent)
{
    
}

void Calculator::calc()
{
    qDebug() << "start calc";
    QTime timing;
    timing.start();
    degeneracyCliques();
    qDebug() << "calc cost " << timing.elapsed() << "ms";
    emit resultReady();
}
