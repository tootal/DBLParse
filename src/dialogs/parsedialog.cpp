#include "parsedialog.h"
#include "ui_parsedialog.h"

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>

#include "util.h"

ParseDialog::ParseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParseDialog)
{
    ui->setupUi(this);
    button = new QWinTaskbarButton(this);
    button->setWindow(parent->windowHandle());
    progress = button->progress();
    progress->setRange(0, 100);
    progress->show();
}

ParseDialog::~ParseDialog()
{
    delete ui;
}

void ParseDialog::setState(int state)
{
    ui->progressBar->setValue(state);
    progress->setValue(state);
}

void ParseDialog::handleDone()
{
    progress->reset();
    progress->hide();
    ui->pushButton->setEnabled(true);
    ui->label->setText(tr("Parsing completed."));
}


void ParseDialog::on_pushButton_clicked()
{
    close();
}
