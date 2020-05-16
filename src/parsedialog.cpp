#include "parsedialog.h"
#include "ui_parsedialog.h"

#include <QDebug>
#include <QTime>
#include <QTimer>

#include <Windows.h>

#include "util.h"

ParseDialog::ParseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParseDialog)
{
    ui->setupUi(this);
}

ParseDialog::~ParseDialog()
{
    delete ui;
}

void ParseDialog::setState(int state)
{
    ui->progressBar->setValue(state);
}

void ParseDialog::handleDone()
{
    ui->pushButton->setEnabled(true);
    ui->label->setText(tr("Parsing completed."));
}


void ParseDialog::on_pushButton_clicked()
{
    close();
}
