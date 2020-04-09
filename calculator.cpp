#include "calculator.h"
#include "util.h"

#include <QDebug>
#include <QFile>
#include <QQueue>
#include <QTime>

#include <algorithm>

Calculator::Calculator(QObject *parent) : QObject(parent)
{
    
}

void Calculator::calc()
{
    qDebug() << "start calc";
    QTime timing;
    timing.start();
    
    QString fileName("authors_relation.txt");
    QFile file(fileName);
    QTextStream in(&file);
    if (!file.exists()) {
        qDebug() << "authors_relation.txt not exists!";    
        return ;
    }
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
    
    int totalAuthor;
    in >> totalAuthor;
    G.resize(totalAuthor);
    visited.resize(totalAuthor);
    
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
    file.close();
    for (int i = 0; i < G.size(); ++i) {
        std::sort(G[i].begin(), G[i].end());
        G[i].erase(std::unique(G[i].begin(), G[i].end()), G[i].end());
    }
    
//    qDebug() << Util::str(G);
//    enumerateAllCliques();
//    qDebug() << Util::str(cnt);
    
    connectedComponents();
    
    qDebug() << "calc cost " << timing.elapsed() << "ms";
    emit resultReady();
}

void Calculator::enumerateAllCliques()
{
    QQueue<QPair<list, list>> queue;
    for (int i = 0; i < G.size(); ++i) {
        auto item = qMakePair<list, list>({i}, G[i]);
        queue.enqueue(item);
//        qDebug() << Util::str(item);
    }
    
    while (!queue.isEmpty()) {
        list base = queue.head().first;
        // cnbrs is a set of common neighbors of nodes in base.
        list cnbrs = queue.head().second;
        queue.dequeue();
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
            auto item = qMakePair<list, list>(new_base, new_cnbrs);
            queue.enqueue(item);
//            qDebug() << Util::str(item);
        }
    }
}

void Calculator::connectedComponents()
{
    int numberOfComponents = 0;
    int maxSizeComponent = 0;
    visited.fill(false);
    for (int i = 0; i < G.size(); ++i) {
        if (!visited[i]) {
            list component;
            // bfs
            list nextlevel(1, i);
            while (!nextlevel.isEmpty()) {
                list thislevel = nextlevel;
                nextlevel.clear();
                for (int u : thislevel) {
                    if (!visited[u]) {
                        component.append(u);
                        visited[u] = true;
                        for (int v : G[u]) {
                            nextlevel.append(v);
                        }
                    }
                }
            }
            maxSizeComponent = std::max(maxSizeComponent, component.size());
            ++numberOfComponents;
        }
    }
    qDebug() << "number of components: " << numberOfComponents;
    qDebug() << "max size components: " << maxSizeComponent;
}
