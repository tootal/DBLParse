#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "parsedialog.h"
#include "util.h"
#include "record.h"
#include "finder.h"
#include "webpage.h"
#include "loader.h"
#include "authorstacdialog.h"
#include "settingsdialog.h"
#include "configmanager.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QPushButton>
#include <QWebChannel>
#include <QDesktopServices>

extern ConfigManager *g_config;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->webview);

    m_parser = new Parser(this);
    m_finder = new Finder(this);
    m_loader = new Loader(this);
    
    ui->webview->registerObject("finder", m_finder);
    ui->webview->setUrl(QUrl("qrc:/resources/index.html"));
    
    connect(m_parser, &Parser::done,
            this, &MainWindow::load);
    
    connect(m_finder, &Finder::notReady,
            this, &MainWindow::on_action_Status_triggered);
    
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
    connect(m_loader, &Loader::keyLoadDone,
            m_finder, &Finder::setKeyLoaded);
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
    m_parser->quit();
    m_parser->wait();
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_action_About_Dblparse_triggered()
{
    QString info = tr(R"(DBLParse %1<br/>Built on %2<br/><br/>
DBLParse is an application that bases on dblp computer science bibliography.<br/><br/>
Please visit <a href="https://github.com/tootal/DBLParse">DBLParse</a> for more information.)")
            .arg(g_config->value("version"))
            .arg(__TIMESTAMP__);
    
    QMessageBox::about(this, tr("About DBLParse"), info);
}

void MainWindow::on_actionAbout_DBLP_triggered()
{
    QString info = tr(R"(DBLP<br/><br/>
The <em>dblp computer science bibliography</em> provides
open bibliographic information on major computer science journals and proceedings.
Originally created at the <a href="https://www.uni-trier.de/">University of Trier</a> in 1993,
dblp is now operated and further developed by <a href="https://www.dagstuhl.de/">Schloss Dagstuhl</a>.<br/><br/>
For more information <a href="https://dblp.uni-trier.de/faq/">check out our F.A.Q.</a>)");
    QMessageBox::about(this, tr("About DBLP"), info);
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::on_action_Open_triggered()
{
    QString lastOpenFileName = g_config->value("lastOpenFileName");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select XML file"),
                                                    lastOpenFileName,
                                                    tr("XML file (*.xml)"));
    if(fileName.isEmpty()) return ;
    g_config->setValue("lastOpenFileName", fileName);
    // question when size greater than 64MiB
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        box.resize(500, 170);
        box.setText(tr("Parsing the file will last for a while and will take up a lot of memory."));
        box.setInformativeText(tr("Do you want to continue?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::No);
        int ret = box.exec();
        if(ret == QMessageBox::No) return ;
    }
    ParseDialog *dialog = new ParseDialog(this);
    connect(m_parser, &Parser::stateChanged,
            dialog, &ParseDialog::showStatus);
    connect(m_parser, &Parser::done,
            dialog, &ParseDialog::activeButton);
    dialog->open();
    m_parser->setFileName(fileName);
    m_parser->start();
}

void MainWindow::on_action_Status_triggered()
{
    QMessageBox msgBox(this);
    QString text;
    QString parserStatus;
    QString loaderStatus;
    if(m_finder->parsed()){
        parserStatus = QString(R"(<font color="green">OK</font>)");
    }else{
        parserStatus = QString(R"(<font color="red">NO</font>)");
    }
    if(m_finder->loaded()){
        loaderStatus = QString(R"(<font color="green">OK</font>)");
    }else{
        loaderStatus = QString(R"(<font color="red">NO</font>)");
    }
    text = tr("Parser: %1 <br>Loader: %2").arg(parserStatus).arg(loaderStatus);
    msgBox.setText(text);
    if(m_finder->parsed()){
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
    }else{
        msgBox.setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
        msgBox.button(QMessageBox::Open)->setText(tr("Open XML file"));
        msgBox.button(QMessageBox::Cancel)->setText(tr("Cancel"));
        msgBox.setDefaultButton(QMessageBox::Cancel);
    }
    int ret = msgBox.exec();
    if(ret == QMessageBox::Open){
        on_action_Open_triggered();
    }
}

void MainWindow::on_action_Clear_Index_triggered()
{
    m_parser->clearIndex();
    m_finder->clearIndex();
    statusBar()->showMessage(tr("Clear index file successful!"));
}

void MainWindow::on_action_Open_Index_Folder_triggered()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath()));    
}
//void MainWindow::on_authorStacRadioButton_clicked()
//{
//    ui->stackedWidget->setCurrentIndex(4);
//    ui->keyEdit->setFocus();

//    QList<QPair<QString,int> > authorStac=m_finder->returnAuthorStac();

//    if(authorStac.isEmpty()){
//        QMessageBox::information(this, tr("Information"),
//                                 tr("please parse first."));
//        return ;
//    }
//    ui->tableWidget_2->clearContents();

//    int num=authorStac.size()<=100?authorStac.size():100;
//    ui->tableWidget_2->setRowCount(num);

//        for(qint32 t=0;t<num;t++){
//            ui->tableWidget_2->setItem(t, 0, new QTableWidgetItem(authorStac[t].first));
//            ui->tableWidget_2->setItem(t, 1, new QTableWidgetItem(QString::number(authorStac[t].second)));
//        }

//    ui->tableWidget->resizeRowsToContents();
//}

void MainWindow::load()
{
    m_loader->start();
    Finder::init();
}

void MainWindow::on_actionAuthorStac_triggered()
{
     if(!m_finder->parsed() || !m_finder->authorStacLoaded()){
        on_action_Status_triggered();
        return ;
    }
    AuthorStacDialog *dialog=new AuthorStacDialog(this,m_finder);
    dialog->open();
}

void MainWindow::on_actionView_Log_triggered()
{
#ifdef QT_NO_DEBUG
    QDesktopServices::openUrl(QUrl::fromLocalFile("DBLParse.log"));
#endif
}

void MainWindow::on_action_Settings_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->open();
}
