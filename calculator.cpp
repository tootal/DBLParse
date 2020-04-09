#include "calculator.h"
#include "util.h"

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
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<int> nodes;
        foreach (QString node, line.split(' ')) {
            nodes.append(node.toInt());    
        }
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
//                if (nodes[i] > nodes[j]) std::swap(nodes[i], nodes[j]);
                // nodes[i] < nodes[j]
                G[nodes[i]].append(nodes[j]);
                G[nodes[j]].append(nodes[i]);
            }
        }
    }
    file.close();
    for (int i = 0; i < G.size(); ++i) {
        std::sort(G[i].begin(), G[i].end());
        G[i].erase(std::unique(G[i].begin(), G[i].end()), G[i].end());
    }
    
    qDebug() << Util::str(G);
//    enumerateAllCliques();
//    qDebug() << Util::str(cnt);
    
//    connectedComponents();
    cutBridges();
    
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
    QVector<bool> visited(G.size());
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

void Calculator::cutBridges()
{
    list preorder(G.size());
    list lowlink(G.size());
    list parent(G.size());
    int i = 0; // Preorder counter
    for (int s = 0; s < G.size(); ++s) {
        if (preorder[s] != 0) continue;
        parent[s] = s;
        QStack<int> stack;
        stack.push(s);
        while (!stack.isEmpty()) {
            int u = stack.top();
            if (preorder[u] == 0) preorder[u] = ++i;
            bool done = true;
            for (int v : G[u]) {
                if (preorder[v] != 0) continue;
                parent[v] = u;
                stack.push(v);
                done = false;
                break;
            }
            if (!done) continue;
            lowlink[u] = preorder[u];
            for (int v : G[u]) {
                if (v == parent[u]) continue;
                if (preorder[v] > preorder[u]) {
                    lowlink[u] = std::min(lowlink[u], lowlink[v]);
                } else {
                    lowlink[u] = std::min(lowlink[u], preorder[v]);
                }
            }
            stack.pop();
        }
    }
//    qDebug() << "parent" << Util::str(parent);
//    qDebug() << "preorder" << Util::str(preorder);
//    qDebug() << "lowlink" << Util::str(lowlink);
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
            if (v < u) continue;
            if (lowlink[u] > preorder[v] || lowlink[v] > preorder[u]) {
                qDebug() << "bridge:" << u << v;
            }
        }
    }
}
