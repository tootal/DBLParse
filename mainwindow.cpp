#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "parsedialog.h"
#include "util.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_parser = new Parser(this);
    resume();
    m_parseDialog = new ParseDialog(this);
    connect(m_parser, &Parser::countChanged,
            m_parseDialog, &ParseDialog::showProgress);
    connect(m_parser, &Parser::done,
            m_parseDialog, &ParseDialog::showDone);
    connect(m_parseDialog, &ParseDialog::abortParse,
            m_parser, &Parser::abortParser);
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
Please visit <a href="https://github.com/tootal/dblparse">DBLParse</a> for more information.)");
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

void MainWindow::on_searchButton_clicked()
{
//    qDebug() << "search : " << ui->keyEdit->text();
    QString key = ui->keyEdit->text();
    if(key.isEmpty()){
        QMessageBox::information(this, tr("Information"),
                                 tr("Please enter a search key."));
        return ;
    }
    
    if(ui->authorRadioButton->isChecked()){
        
    }else{
        
    }
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
    m_parser->clear();
    m_parser->setFileName(fileName);
    m_parser->start();
    m_parseDialog->clear();
    m_parseDialog->open();
}

void MainWindow::on_action_Status_triggered()
{
    QMessageBox msgBox(this);
    if(m_parser->parsed()){
        msgBox.setText(tr(R"(<b>The XML file has been parsed.</b><br/>
Record count: %1 <br/>
Parse cost time: %2 <br/>
)").arg(m_parser->count()).arg(Util::formatTime(m_parser->costTime())));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
    }else{
        msgBox.setText(tr("No XML file has been parsed."));
        msgBox.setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
        msgBox.button(QMessageBox::Open)->setText("Open XML file");
        msgBox.setDefaultButton(QMessageBox::Cancel);
    }
    int ret = msgBox.exec();
    if(ret == QMessageBox::Open){
        on_action_Open_triggered();
    }
}

void MainWindow::resume()
{
    if(m_parser->parsed()) return ;
    if(!QFile("dblp.dat").exists()) return ;
    m_parser->load();
}
