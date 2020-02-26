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
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QNetworkAccessManager>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DBLParse"));
    setMinimumSize(400,300);
//     Menu Action
    openAction = new QAction(tr("&Open XML File"),this);
    useNetworkDataAction = new QAction(tr("Use Network Data"),this);
    useNetworkDataAction->setCheckable(true);
    useLocalDataAction = new QAction(tr("Use Local Data"),this);
    useLocalDataAction->setCheckable(true);
    useLocalDataAction->setChecked(true);
    QActionGroup *useDataType = new QActionGroup(this);
    useDataType->addAction(useNetworkDataAction);
    useDataType->addAction(useLocalDataAction);
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *settingMenu = menuBar()->addMenu(tr("&Setting"));
    fileMenu->addAction(openAction);
    settingMenu->addAction(useNetworkDataAction);
    settingMenu->addAction(useLocalDataAction);
    
    QWidget *widget = new QWidget;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    QLineEdit *lineEdit = new QLineEdit(this);
    QPushButton *searchButton = new QPushButton("&Search", this);
    searchLayout->addWidget(lineEdit);
    searchLayout->addWidget(searchButton);
    textBrowser = new QTextBrowser;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(searchLayout);
    layout->addWidget(textBrowser);
    widget->setLayout(layout);
    setCentralWidget(widget);
    settings = new QSettings;
    
//    parseThread = new QThread;
    parser = new Parser(this);
//    parser->moveToThread(parseThread);
    parseDialog = new ParseDialog(this);
    
//     Signal and Slot connect
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(searchButton, &QPushButton::clicked, this, [this,lineEdit](){
       QString word = lineEdit->text();
       if(word.isEmpty()){
           QMessageBox::information(this, tr("Information"),
                tr("Search key can not be empty!"));
       }else{
           search(word);
       }
    });
    connect(parser, &Parser::countChanged, parseDialog, &ParseDialog::showProgress);
    connect(parser, &Parser::done, parseDialog, &ParseDialog::showDone);
    connect(parser, &Parser::done, this, &MainWindow::parseDone);
    connect(parseDialog, &ParseDialog::abortParse, parser, &Parser::abortParse);
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

void MainWindow::search(QString word)
{
    if(useLocalDataAction->isChecked()){
        if(parser->hasReader()){
            searchLocal(word);
        }else{
            QMessageBox::information(this,tr("Information"),
                tr("Index File Not Found.\nPlease Open XML File."));
//            textBrowser->append(
//                tr("Not Found Index File, Please Open XML File."));
        }
    }else{
        searchNetwork(word);
    }
}

void MainWindow::searchLocal(QString word)
{
    textBrowser->append("Local Search: "+word);
}

void MainWindow::searchNetwork(QString word)
{
    Q_UNUSED(word);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("https://dblp.uni-trier.de/search/author?xauthor=Nicola:Soldati"));
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    conf.setProtocol(QSsl::TlsV1SslV3);
//    request.setSslConfiguration(conf);
//    QNetworkReply *reply = manager->get(request);
//    qDebug() << QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << QSslSocket::supportsSsl();
//    qDebug() << QSslSocket::sslLibraryVersionString();
    qDebug() << manager->supportedSchemes();
    
//    connect(reply, &QIODevice::readyRead,
//            this, [this](QNetworkReply *reply){
//        qDebug()<<"finished";
//        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//        qDebug()<<"status code: "<<statusCode;
//        textBrowser->setText(reply->readAll());
//    });
//    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
//            this, [](){
        
//    });
}

