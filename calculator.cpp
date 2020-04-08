#include "calculator.h"
#include "util.h"

#include <QDebug>
#include <QFile>
#include <QQueue>

#include <algorithm>

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
    G.resize(totalAuthor + 1);
    visited.resize(totalAuthor);
    cnt[1] = totalAuthor;
    
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<int> nodes;
        foreach (QString node, line.split(' ')) {
            nodes.append(node.toInt());    
        }
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
                if (nodes[i] > nodes[j]) std::swap(nodes[i], nodes[j]);
                // nodes[i] < nodes[j]
                G[nodes[i]].append(nodes[j]);
            }
        }
    }
//    printG();
    file.close();
    
    emit resultReady();
}

void Calculator::enumerateAllCliques()
{
    typedef QVector<int> list;
    QQueue<QPair<list, list>> queue;
    for (int i = 1; i <= G.size(); ++i) {
        queue.enqueue(qMakePair<list, list>({i}, G[i]));
    }
    
    while (!queue.isEmpty()) {
        list base = queue.head().first;
        // cnbrs is a set of common neighbors of nodes in base.
        list cnbrs = queue.head().second;
        ++cnt[base.size()];
        
        for (int i = 0; i < cnbrs.size(); ++i) {
            list new_base(base);
            new_base.append(cnbrs[i]);
            list new_cnbrs;
            for (int j = i + 1; j < cnbrs.size(); ++j) {
                if (G[cnbrs[i]].contains(cnbrs[j])) {
                    new_cnbrs.append(cnbrs[j]);
                }
            }
            queue.enqueue(qMakePair<list, list>(
                new_base,
                new_cnbrs
            ));
        }
    }
}
