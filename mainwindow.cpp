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
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DBLParse"));
    setMinimumSize(600,400);
//     Menu Action
    fileOpenAction = new QAction(tr("&Open XML File"),this);
    fileInfoAction = new QAction(tr("Parse Info"),this);
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
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileInfoAction);
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
    
    webView = new QWebEngineView;
    QFile htmlFile(":/www/html/index.html");
    htmlFile.open(QIODevice::ReadOnly|QIODevice::Text);
    webView->setHtml(htmlFile.readAll());
    setCentralWidget(webView);
//    setCentralWidget(widget);
    settings = new QSettings;
    
//    parseThread = new QThread;
    parser = new Parser(this);
    parseFile = nullptr;
    resume();
//    parser->moveToThread(parseThread);
    parseDialog = new ParseDialog(this);
    
//     Signal and Slot connect
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(fileInfoAction, &QAction::triggered, this, [this](){
        QMessageBox *msgBox = new QMessageBox(this);
//        msgBox->setMinimumSize(200, 200);
//        msgBox->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
        if(parser->parsed()){
            QString text;
            msgBox->setText(tr("The XML file has been parsed."));
            text.append(tr("Document version: %1\n").arg(parser->documentVersion()));
            text.append(tr("Document encoding: %1\n").arg(parser->documentEncoding()));
            text.append(tr("DTD name: %1\n").arg(parser->dtdName()));
            text.append(tr("DTD system id: %1\n").arg(parser->dtdSystemId()));
            text.append(tr("Record count: %1\n").arg(parser->count()));
            text.append(tr("Parse cost time: %1 ms\n").arg(parser->parseCostMsecs()));
            msgBox->setInformativeText(text);
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setDefaultButton(QMessageBox::Ok);
        }else{
            msgBox->setText(tr("No XML file has been parsed."));
            msgBox->setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
            msgBox->button(QMessageBox::Open)->setText("Open XML file");
            msgBox->setDefaultButton(QMessageBox::Cancel);
        }
        int ret = msgBox->exec();
        if(ret == QMessageBox::Open){
            openFile();
        }
    });
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
    QString lastOpenFileName;
    if(settings->contains("lastOpenFileName")){
        lastOpenFileName = settings->value("lastOpenFileName").toString();
    }else{
        // use document location as default
        lastOpenFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("Open XML File"), lastOpenFileName, tr("XML Files (*.xml)"));
    if(!fileName.isEmpty()){
        settings->setValue("lastOpenFileName", fileName);
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
    parseFile->close();
    QFile file("dblp.dat");
    if(file.open(QIODevice::WriteOnly)){
//        qDebug()<<"parseDone test";
        QDataStream stream(&file);
        stream<<parser->documentVersion();
        stream<<parser->documentEncoding();
        stream<<parser->dtdName();
        stream<<parser->dtdSystemId();
        stream<<parser->count();
        stream<<parser->recordCount();
        stream<<parser->authorIndex();
        stream<<parser->parseCostMsecs();
    }
    file.close();
    settings->setValue("recordNames", parser->recordNames());
}

void MainWindow::resume()
{
    QFile file("dblp.dat");
//    qDebug()<<"resume: "<<file.exists();
    if(file.exists() && settings->contains("lastOpenFileName")){
        if(file.open(QIODevice::ReadOnly)){
            QDataStream stream(&file);
            QString documentVersion;
            stream>>documentVersion;
            parser->setDocumentVersion(documentVersion);
            QString documentEncoding;
            stream>>documentEncoding;
            parser->setDocumentEncoding(documentEncoding);
            QString dtdName;
            stream>>dtdName;
            parser->setDtdName(dtdName);
            QString dtdSystemId;
            stream>>dtdSystemId;
            parser->setDtdSystemId(dtdSystemId);
            int count;
            stream>>count;
            parser->setCount(count);
            QMap<QString,QVariant> recordCount;
            stream>>recordCount;
            parser->setRecordCount(recordCount);
            QMap<QString,QVariant> authorIndex;
            stream>>authorIndex;
            parser->setAuthorIndex(authorIndex);
            int parseCostMsecs;
            stream>>parseCostMsecs;
            parser->setParseCostMsecs(parseCostMsecs);
        }
        file.close();
        parseFile = new QFile(settings->value("lastOpenFileName").toString());
//        qDebug()<<"parseFile set";
        parser->setParsed();
    }
}

void MainWindow::search(QString word)
{
    if(useLocalDataAction->isChecked()){
        if(parser->parsed()){
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
//    textBrowser->append("Local Search: "+word);
    auto list = parser->getOffsetsByAuthorName(word);
    if(list.isEmpty()){
        QMessageBox::information(this,tr("Information"),
            tr("The author does not exist."));
    }else{
        textBrowser->clear();
//        qDebug()<<"searchLocal: "<<list.size();
        textBrowser->append("<b>Records of "+word+"</b>");
        foreach(auto offset, list){
//            textBrowser->append(QString::number(offset));
            Q_ASSERT(parseFile != nullptr);
            RecordParser *recordParser = RecordParser::fromFile(parseFile, offset.toLongLong());
//            textBrowser->append(recordParser->key());
            textBrowser->append(recordParser->title());
        }
    }
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

