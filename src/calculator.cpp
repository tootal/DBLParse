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
    handleAuthorRelations();
    generateAuthorsEdges();
    degeneracyCliques();
    qDebug() << "calc cost " << timing.elapsed() << "ms";
    emit resultReady();
}

void Calculator::handleAuthorRelations()
{
    QFile file("authors_relation.txt");
    QTextStream in(&file);
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
    
    int totalAuthor;
    in >> totalAuthor;
    qInfo() << "(Graph) number of nodes:" << totalAuthor; 
    G.clear();
    G.resize(totalAuthor);
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<int> nodes;
        for (QString node : line.split(' ')) {
            nodes.append(node.toInt());    
        }
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
                if (nodes[i] == nodes[j]) continue;
//                if (nodes[i] > nodes[j]) std::swap(nodes[i], nodes[j]);
                // nodes[i] < nodes[j]
                G[nodes[i]].append(nodes[j]);
                G[nodes[j]].append(nodes[i]);
            }
        }
    }
    file.close();
    
    int maxDegree = 0;
    for (int i = 0; i < G.size(); ++i) {
        std::sort(G[i].begin(), G[i].end());
        G[i].erase(std::unique(G[i].begin(), G[i].end()), G[i].end());
        maxDegree = std::max(maxDegree, G[i].size());
    }
    qInfo() << "(Graph) max degree:" << maxDegree;
    
    edges = 0;
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
//            qDebug() << u << v;
            if (v <= u) continue;
            ++edges;
        }
    }
    qInfo() << "(Graph) number of edges:" << edges;
}

void Calculator::generateAuthorsEdges()
{
    QFile file("authors.edges");
    QTextStream out(&file);
    // expert authors.edges
    file.open(QFile::WriteOnly | QFile::Text);
    out << G.size() << ' ' << edges << '\n';
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
            if (v <= u) continue;
            out << u << ' ' << v << '\n';
        }
    }
    file.close();
}
