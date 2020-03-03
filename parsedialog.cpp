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

void ParseDialog::showDone(Parser *parser)
{
    QString info;
    if(abortFlag){
        info = tr("Abort");
    }else{
        info = tr("Successful");
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
