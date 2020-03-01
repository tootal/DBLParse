#include "parsedialog.h"
#include "ui_parsedialog.h"
#include "util.h"

#include <QDebug>
#include <QTime>

ParseDialog::ParseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParseDialog)
{
    ui->setupUi(this);
    clear();
}

ParseDialog::~ParseDialog()
{
    delete ui;
}

void ParseDialog::showProgress(double ratio)
{
    ui->progressBar->setValue(ratio*100);
}

void ParseDialog::showDone(Parser *parser)
{
    QString info;
    if(abortFlag){
        info = "Abort";
    }else{
        info = "Successful";
        ui->progressBar->setValue(10000);
    }
    ui->label->setText(tr("Parsing %1.\nProcessed %2 records in %3 .")
            .arg(info)
            .arg(parser->count())
            .arg(Util::formatTime(parser->costMsecs())));
    ui->pushButton->setText(tr("Close"));
    ui->pushButton->setProperty("type", "close");
}

void ParseDialog::clear()
{
    ui->label->setText(tr("Parsing..."));
    abortFlag = false;
    ui->progressBar->setValue(0);
    ui->pushButton->setText(tr("Abort"));
    ui->pushButton->setProperty("type", "abort");
}

void ParseDialog::on_pushButton_clicked()
{
    if(ui->pushButton->property("type") == "close"){
        close();
    }else{
        abortFlag = true;
        emit abortParse();
    }
}
