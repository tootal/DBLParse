#include "calculator.h"

#include <QDebug>
#include <QFile>

Calculator::Calculator(QObject *parent) : QObject(parent)
{
    
}

void Calculator::calc()
{
    qDebug() << "start calc";
    
    QString fileName("authors_relation.txt");
    QFile file(fileName);
    QTextStream in(&file);
    Q_ASSERT(file.exists());
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
    
    
    
    file.close();
    emit resultReady();
}
