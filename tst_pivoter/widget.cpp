#include "widget.h"
#include "ui_widget.h"

#include <QVector>
#include <QFile>
#include <QDataStream>

#include "misc.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QVector<QVector<int>> G;
    {
        QFile file("data/authordata");
        file.open(QFile::ReadOnly);
        QDataStream s(&file);
        s >> G;
        file.close();
    }
    int n = G.size();
    QVector<LinkedList*> adjList(n);
    for (int i = 0; i < n; i++)
        adjList[i] = createLinkedList();
    for (int u = 0; u < n; u++) {
        for (int v : G[u]) {
            if (v <= u) continue;
            addLast(adjList[u], v);
            addLast(adjList[v], u);
        }
    }
    G.clear();
    G.squeeze();
    runAndPrintStatsCliques(adjList, n);
    for (int i = 0; i < n; i++)
        destroyLinkedList(adjList[i]);
}

Widget::~Widget()
{
    delete ui;
}

