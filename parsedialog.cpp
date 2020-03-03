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

void ParseDialog::showStatus(const QString &msg)
{
    ui->textBrowser->append(msg);
}

void ParseDialog::clear()
{
    ui->pushButton->setEnabled(false);
}

void ParseDialog::activeButton()
{
    ui->pushButton->setEnabled(true);
}

void ParseDialog::on_pushButton_clicked()
{
    close();
}
