#include "calculator.h"
#include "util.h"

#include "degeneracy_cliques.h"

#include <QDebug>
#include <QFile>
#include <QQueue>
#include <QTime>
#include <QStack>
#include <QProcess>

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
    if (countCliques()) {
        qDebug() << "degeneracy_cliques ok";
    } else {
        qDebug() << "degeneracy_cliques no";
    }
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
        foreach (QString node, line.split(' ')) {
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

bool Calculator::countCliques()
{
    /*
    QString command = "degeneracy_cliques.exe -i authors.edges -t A -k 0 -d 0";
    QProcess *process = new QProcess(this);
    process->start(command);
    bool ret = process->waitForFinished(-1);
    if (ret) {
        QFile file("cliques_count.txt");
        file.open(QFile::WriteOnly | QFile::Text);
        file.write(process->readAllStandardOutput());
        file.close();
    }
    */
    bool ret = false;
    char fname[] = "authors";
    degeneracyCliques(fname);
    ret = true;
    return ret;
}

void Calculator::enumerateAllCliques()
{
    QQueue<QPair<list, list>> que;
    for (int u = 0; u < G.size(); ++u) {
        QPair<list, list> item;
        item.first.append(u);
        for (int v : G[u]) {
            if (v > u) item.second.append(v);
        }
        que.enqueue(item);
//        qDebug() << Util::str(item);
    }
    
    while (!que.isEmpty()) {
        list base = que.head().first;
        // cnbrs is a set of common neighbors of nodes in base.
        list cnbrs = que.head().second;
        que.dequeue();
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
            que.enqueue(item);
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
    qInfo() << "(Graph) number of components:" << numberOfComponents;
    qInfo() << "(Graph) max size components:" << maxSizeComponent;
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
        stack st;
        st.push(s);
        while (!st.isEmpty()) {
            int u = st.top();
            if (preorder[u] == 0) preorder[u] = ++i;
            bool done = true;
            for (int v : G[u]) {
                if (preorder[v] != 0) continue;
                parent[v] = u;
                st.push(v);
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
            st.pop();
        }
    }
//    qDebug() << "parent" << Util::str(parent);
//    qDebug() << "preorder" << Util::str(preorder);
//    qDebug() << "lowlink" << Util::str(lowlink);
    int bridges = 0;
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
            if (v < u) continue;
            if (lowlink[u] > preorder[v] || lowlink[v] > preorder[u]) {
                ++bridges;
//                qDebug() << "bridge:" << u << v;
                G[u].removeAll(v);
                G[v].removeAll(u);
                ++cnt[2];
            }
        }
    }
    qInfo() << "(Graph) number of bridges:" << bridges;
}

void Calculator::findCliques()
{
    QMap<int, set> adj;
    for (int u = 0; u < G.size(); ++u) {
        for (int v : G[u]) {
            adj[u].insert(v);
        }
    }
    list Q(1, -1);
    set subg, cand;
    int u = 0;
    for (int i = 0; i < G.size(); ++i) {
        subg.insert(i);
        cand.insert(i);
        if (G[i].size() > G[u].size()) {
            u = i;
        }
    }
    set ext_u = cand - adj[u];
    typedef std::tuple<set, set, set> args_type;
    QStack<args_type> st;
    while (true) {
        if (!ext_u.isEmpty()) {
            int q = *ext_u.begin();
            ext_u.erase(ext_u.begin());
            cand.remove(q);
            Q.back() = q;
            auto &adj_q = adj[q];
            auto subg_q = subg &adj_q;
            if (subg_q.isEmpty()) {
                // Q is maximal clique
//                qDebug() << "find clique:" << Util::str(Q);
                ++cnt[Q.size()];
            } else {
                auto cand_q = cand & adj_q;
                if (!cand_q.isEmpty()) {
                    st.append(std::make_tuple(subg, cand, ext_u));
                    Q.append(-1);
                    subg = subg_q;
                    cand = cand_q;
                    u = *subg.begin();
                    int maxu = (cand & adj[u]).size();
                    for (int i : subg) {
                        int maxi = (cand & adj[i]).size();
                        if (maxi > maxu) {
                            u = i;
                            maxu = maxi;
                        }
                    }
                    ext_u = cand - adj[u];
                }
            }
        } else {
            Q.pop_back();
            if (st.isEmpty()) break;
            subg = std::get<0>(st.top());
            cand = std::get<1>(st.top());
            ext_u = std::get<2>(st.top());
            st.pop();
        }
    }
}
