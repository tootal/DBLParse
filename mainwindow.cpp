#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "parsedialog.h"
#include "util.h"
#include "record.h"
#include "finder.h"
#include "webpage.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QWebChannel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->webview);

    m_parser = new Parser(this);
    m_finder = new Finder(this);
    
    WebPage *page = new WebPage(this);
    ui->webview->setPage(page);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("finder", m_finder);
    page->setWebChannel(channel);
    ui->webview->setUrl(QUrl("qrc:/resources/index.html"));
    
    Finder::init();
    connect(m_parser, &Parser::done,
            m_finder, &Finder::init);
}

MainWindow::~MainWindow()
{
    delete ui;
    m_parser->quit();
    m_parser->wait();
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_action_About_Dblparse_triggered()
{
    QString info = tr(R"(DBLParse<br/><br/>
DBLParse is a free and open source application that bases on dblp computer science bibliography.<br/><br/>
Please visit <a href="https://github.com/tootal/DBLParse">DBLParse</a> for more information.)");
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
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }else{
        // use document location as default
        lastOpenFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select XML file"),
                                                    lastOpenFileName,
                                                    tr("XML file (*.xml)"));
    if(fileName.isEmpty()) return ;
    settings.setValue("lastOpenFileName", fileName);
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
    if(m_finder->parsed()){
        msgBox.setText(tr("The XML file has been parsed."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
    }else{
        msgBox.setText(tr("No XML file has been parsed."));
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
