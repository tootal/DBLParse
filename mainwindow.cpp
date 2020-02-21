#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DBLParse"));
    setMinimumSize(300,200);
    
    openAction = new QAction(tr("&Open XML File"),this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    
    settings = new QSettings;
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::openFile()
{
    QString lastOpenFilePath;
    if(settings->contains("lastOpenFilePath")){
        lastOpenFilePath = settings->value("lastOpenFilePath").toString();
    }else{
        // use document location as default
        lastOpenFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("Open XML File"), lastOpenFilePath, tr("XML Files (*.xml)"));
    if(!fileName.isEmpty()){
        QFileInfo fileInfo(fileName);
        settings->setValue("lastOpenFilePath", fileInfo.absolutePath());
        QFile file(fileName);
        if(file.open(QFile::ReadOnly | QFile::Text)){
            reader = new QXmlStreamReader(&file);
            parser = new Parser(reader);
            QMessageBox::information(this, tr("Info"),
                tr("Open File Successful!\nDocument Version: %1\nDocument Encoding: %2")
                .arg(parser->documentVersion()).arg(parser->documentEncoding()));
        }
    }
}

