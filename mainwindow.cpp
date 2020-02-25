#include "mainwindow.h"
#include "parser.h"
#include "parsedialog.h"

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DBLParse"));
    setMinimumSize(800,600);
    
    openAction = new QAction(tr("&Open XML File"),this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    
    settings = new QSettings;
    
//    parseThread = new QThread;
    parser = new Parser(this);
//    parser->moveToThread(parseThread);
    parseDialog = new ParseDialog(this);
//    connect(parseThread, &QThread::finished, parser, &QObject::deleteLater);
    connect(parser, &Parser::countChanged, parseDialog, &ParseDialog::showProgress);
    connect(parser, &Parser::done, parseDialog, &ParseDialog::showDone);
    connect(parser, &Parser::done, this, &MainWindow::parseDone);
    connect(parseDialog, &ParseDialog::abortParse, parser, &Parser::abortParse);
    
//    parseThread->start();
}

MainWindow::~MainWindow()
{
    parser->quit();
    parser->wait();
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
        parseFile = new QFile(fileName);
        if(parseFile->open(QFile::ReadOnly | QFile::Text)){
            reader = new QXmlStreamReader(parseFile);
            parser->clear();
            parser->setReader(reader);
//            parser->work();
            parser->start();
            parseDialog->clear();
            parseDialog->exec();
        }
    }
}

void MainWindow::parseDone()
{
    
}

