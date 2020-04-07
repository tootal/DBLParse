#include "calculator.h"
#include "util.h"

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
    
    int totalAuthor;
    in >> totalAuthor;
    QVector<QVector<int>> G(totalAuthor + 1);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<int> nodes;
        foreach (QString node, line.split(' ')) {
            nodes.append(node.toInt());    
        }
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
                G[nodes[i]].append(nodes[j]);
                G[nodes[j]].append(nodes[i]);
            }
        }
    }
    for (int i = 1; i <= totalAuthor; ++i) {
        QString msg;
        QTextStream out(&msg);
        out << "G[" << i << "] = {";
        for (int j = 0; j < G[i].size(); ++j) {
            out << G[i][j];
            if (j != G[i].size() - 1) out << ' ';
        }
        out << '}';
        qDebug() << msg;
    }
    file.close();
    emit resultReady();
}
