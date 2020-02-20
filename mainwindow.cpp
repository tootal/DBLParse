#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamReader>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DBLParse"));
    setMinimumSize(300,200);
    
    openAction = new QAction(tr("&Open XML File"),this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("Open XML File"), "D:/", tr("XML Files (*.xml)"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly | QFile::Text)){
            reader = new QXmlStreamReader(&file);
            QString curToken = reader->tokenString();
            QMessageBox::information(this, tr("Info"),
                tr("Open File Successful!\nCurrent Token: %1").arg(curToken));
        }
    }
}

