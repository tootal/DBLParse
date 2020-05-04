#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QPushButton>
#include <QWebChannel>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStyle>

#include "parser.h"
#include "parsedialog.h"
#include "util.h"
#include "record.h"
#include "finder.h"
#include "webpage.h"
#include "loader.h"
#include "settingsdialog.h"
#include "configmanager.h"

extern ConfigManager *g_config;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->webview);
    m_finder = new Finder(this);
    ui->webview->registerObject("finder", m_finder);
    ui->webview->setUrl(QUrl("qrc:/resources/index.html"));

    m_parser = new Parser;
    m_loader = new Loader(this);
    
    m_parser->moveToThread(&m_parseThread);
    connect(this, &MainWindow::startParse,
            m_parser, &Parser::run);
    connect(m_parser, &Parser::done,
            this, &MainWindow::load);
    m_parseThread.start();
    
    connect(m_finder, &Finder::notReady,
            this, &MainWindow::on_actionStatus_triggered);
    
    connect(m_loader, &Loader::stateChanged,
            this, [this](const QString &state){
        statusBar()->showMessage(state); 
    });
    connect(m_loader, &Loader::authorStacLoadDone,
            m_finder, &Finder::setAuthorStacLoaded);
    connect(m_loader, &Loader::authorLoadDone,
            m_finder, &Finder::setAuthorLoaded);
    connect(m_loader, &Loader::titleLoadDone,
            m_finder, &Finder::setTitleLoaded);
    connect(m_loader, &Loader::yearWordLoadDone,
            m_finder, &Finder::setYearWordLoaded);
    connect(m_loader, &Loader::loadDone,
            this, [this](){
        statusBar()->showMessage(tr("Load finished."), 3000); 
    });
    connect(m_loader, &Loader::loadDone,
            m_finder, &Finder::setLoaded);
    
    connect(ui->webview->page(), &WebPage::request,
            m_finder, &Finder::handleRequest);
    
    load();
}

MainWindow::~MainWindow()
{
    delete ui;
    m_loader->quit();
    m_loader->wait();
    m_parseThread.quit();
    m_parseThread.wait();
}

void MainWindow::showAboutBox(QPixmap icon, const QString &info)
{
    auto box = new QMessageBox(this);
    box->setModal(false);
    box->setText(info);
    box->setIconPixmap(icon.scaled(64, 64));
    box->setStandardButtons(QMessageBox::Ok);
    box->button(QMessageBox::Ok)->setText(tr("OK"));
    box->setDefaultButton(QMessageBox::Ok);
    box->show();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QIcon icon = style()->standardIcon(QStyle::SP_TitleBarMenuButton);
    showAboutBox(icon.pixmap(64), tr(
        "<b>About Qt</b><br><br>"
        "This program uses Qt version %1.<br><br>"
        "Qt is a C++ toolkit for cross-platform application development.<br><br>"
        "See <a href=\"https://www.qt.io/\">qt.io</a> for more information."
    ).arg(QT_VERSION_STR));
}

void MainWindow::on_actionAboutDBLParse_triggered()
{
    showAboutBox(QPixmap(":/resources/DBLParse.png"), tr(
        "<b>DBLParse</b><br>Version: %1<br><br>Built on %2<br><br>"
        "DBLParse is an application that bases on dblp computer "
        "science bibliography.<br/><br/>Please visit "
        "<a href=\"https://github.com/tootal/DBLParse\">DBLParse</a> "
        "for more information."
    ).arg(VERSION_STR, __TIMESTAMP__));
}

void MainWindow::on_actionAboutDBLP_triggered()
{
    showAboutBox(QPixmap(":/resources/dblp.png"), tr(
        "<b>DBLP</b><br><br>"
        "The <em>dblp computer science bibliography</em> provides "
        "open bibliographic information on major computer science "
        "journals and proceedings. Originally created at the "
        "<a href=\"https://www.uni-trier.de/\">University of Trier</a> "
        "in 1993, dblp is now operated and further developed by "
        "<a href=\"https://www.dagstuhl.de/\">Schloss Dagstuhl</a>."
        "<br/><br/>For more information <a href=\"https://dblp.uni-trier.de/faq/\">"
        "check out our F.A.Q.</a>"
    ));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString lastOpenFileName = g_config->value("lastOpenFileName");
    QString fileName;
    {
        QFileDialog dialog(this);
        dialog.setWindowTitle(tr("Open DBLP XML File"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.selectFile(lastOpenFileName);
        dialog.setNameFilter(tr("XML file (*.xml)"));
        dialog.setViewMode(QFileDialog::Detail);
        if (dialog.exec()) fileName = dialog.selectedFiles().first();
        else return ;
    }
    if(fileName.isEmpty()) return ;
    g_config->setValue("lastOpenFileName", fileName);
    // question when size greater than 64MiB
    if(QFile(fileName).size() > PROMOT_FILE_SIZE){
        QMessageBox box(this);
        box.resize(500, 170);
        box.setText(tr("Parsing the file will last for a while "
                       "and will take up a lot of memory."));
        box.setInformativeText(tr("Do you want to continue?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        if(box.exec() == QMessageBox::No) return ;
    }
    Util::clearIndexs();
    auto *dialog = new ParseDialog(this);
    connect(m_parser, &Parser::stateChanged,
            dialog, &ParseDialog::showStatus);
    connect(m_parser, &Parser::done,
            dialog, &ParseDialog::activeButton);
    dialog->open();
    emit startParse();
}

void MainWindow::on_actionStatus_triggered()
{
    QMessageBox msgBox(this);
    QString text;
    QString parserStatus;
    QString loaderStatus;
    if(Util::parsed()){
        parserStatus = QString(R"(<font color="green">OK</font>)");
    }else{
        parserStatus = QString(R"(<font color="red">NO</font>)");
    }
    if(m_finder->loaded()){
        loaderStatus = QString(R"(<font color="green">OK</font>)");
    }else{
        loaderStatus = QString(R"(<font color="red">NO</font>)");
    }
    text = tr("Parser: %1 <br>Loader: %2").arg(parserStatus, loaderStatus);
    msgBox.setText(text);
    if(Util::parsed()){
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.button(QMessageBox::Ok)->setText(tr("OK"));
        msgBox.setDefaultButton(QMessageBox::Ok);
    }else{
        msgBox.setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
        msgBox.button(QMessageBox::Open)->setText(tr("Open XML file"));
        msgBox.button(QMessageBox::Cancel)->setText(tr("Cancel"));
        msgBox.setDefaultButton(QMessageBox::Cancel);
    }
    int ret = msgBox.exec();
    if(ret == QMessageBox::Open){
        on_actionOpen_triggered();
    }
}

void MainWindow::on_actionClearIndex_triggered()
{
    Util::clearIndexs();
    m_finder->clearIndex();
    statusBar()->showMessage(tr("Clear index file successful!"));
}

void MainWindow::on_actionOpenIndexFolder_triggered()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath()));    
}

void MainWindow::load()
{
    m_loader->start();
    Finder::init();
}

void MainWindow::on_actionAuthorStac_triggered()
{
    static const int TOP_K = 100;
    if (!Util::parsed() || !m_finder->authorStacLoaded()) {
        on_actionStatus_triggered();
        return ;
    }

    auto *view = new WebView(this);
    view->setWindowFlag(Qt::Window);
    view->resize(600, 800);
    
    QList<QPair<QString,int>> authorStac=Finder::authorStac();
    QJsonArray authorStacArray;
    
    int num = authorStac.size() <= TOP_K ? authorStac.size() : TOP_K;
    
    for (qint32 t=0; t<num; t++) {
        QJsonObject obj;
        obj.insert("author",authorStac[t].first);
        obj.insert("articleNum",QString::number(authorStac[t].second));
        authorStacArray.append(obj);
    }
    
    auto html = Util::readFile(":/resources/authorStac.html");
    auto data = QJsonDocument(authorStacArray).toJson();
    
    //     qDebug() << data;
    
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/resources/"));
    view->show();
}

void MainWindow::on_actionViewLog_triggered()
{
#ifdef QT_NO_DEBUG
    QDesktopServices::openUrl(QUrl::fromLocalFile("DBLParse.log"));
#endif
}

void MainWindow::on_actionSettings_triggered()
{
    auto *dialog = new SettingsDialog(this);
    dialog->open();
}

void MainWindow::on_actionKeyWord_triggered()
{
    if(!Util::parsed() || !m_finder->yearWordLoaded()){
        on_actionStatus_triggered();
        return ;
    }

     auto *view = new WebView(this);
     view->setWindowFlag(Qt::Window);
     view->registerObject("finder", m_finder);
     view->resize(800,600);
     connect(view->page(), &WebPage::wordCloud,
             m_finder, &Finder::handleWordCloud);

     Parser::YW_T yearWord = Finder::yearWord();
     auto it= yearWord.begin();
     QJsonArray yearWordArray;

     while (it != yearWord.end()) {
         QJsonObject obj;
         obj.insert("year", it.key());
         QJsonArray arr;
         for (auto &cw : it.value()) {
             QJsonObject o;
             o.insert("count", cw.first);
             o.insert("word", cw.second);
             arr.append(o);
         }
         obj.insert("words", arr);
         yearWordArray.append(obj);
         it++;
     }

     auto html = Util::readFile(":/resources/yearWord.html");
     auto data = QJsonDocument(yearWordArray).toJson();

     html.replace("<!-- DATA_HOLDER -->", data);
     view->setHtml(html, QUrl("qrc:/resources/"));
     view->show();
}

void MainWindow::on_actionCountClique_triggered()
{
    if(!Util::parsed()){
        on_actionStatus_triggered();
        return ;
    }
    QFile file("authors_cliques.txt");
    auto *view = new WebView(this);
    view->setWindowFlag(Qt::Window);
    view->resize(850, 600);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&file);
    QJsonObject o;
    int total;
    in >> total;
    for (int i = 1; i <= total; ++i) {
        QString n;
        QString cnt;
        in >> n >> cnt;
        o.insert(n, cnt);
    }
    in >> total;
    o.insert("total", total);
    auto html = Util::readFile(":/resources/clique.html");
    auto data = QJsonDocument(o).toJson();
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/resources/"));
    view->show();
}
