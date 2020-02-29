#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_action_About_Dblparse_triggered()
{
    QString info = tr("DBLParse");
    info += "<br/><br/>";
    info += tr("DBLParse is a free and open source application that bases on dblp computer science bibliography.");
    info += "<br/><br/>";
    info += tr("Please visit <a href=\"https://github.com/tootal/dblparse\">DBLParse</a> for more information.");
    QMessageBox::about(this, tr("About DBLParse"), info);
}

void MainWindow::on_actionAbout_DBLP_triggered()
{
    QString info = tr("DBLP");
    info += "<br/><br/>";
    info += tr("The <em>dblp computer science bibliography</em> provides open bibliographic information on major computer science journals and proceedings.");
    info += tr("Originally created at the <a href=\"https://www.uni-trier.de/\">University of Trier</a> in 1993, ");
    info += tr("dblp is now operated and further developed by <a href=\"https://www.dagstuhl.de/\">Schloss Dagstuhl</a>.");
    info += "<br/><br/>";
    info += tr("For more information <a href=\"https://dblp.uni-trier.de/faq/\">check out our F.A.Q.</a>");
    QMessageBox::about(this, tr("About DBLP"), info);
}
