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
