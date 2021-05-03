#include "parsedialog.h"
#include "ui_parsedialog.h"

#include <QDebug>
#include <QTime>
#include <QTimer>

#ifdef Q_OS_WIN
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

#include "util.h"

ParseDialog::ParseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParseDialog)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    button = new QWinTaskbarButton(this);
    button->setWindow(parent->windowHandle());
    progress = button->progress();
    progress->setRange(0, 100);
    progress->show();
#endif
}

ParseDialog::~ParseDialog()
{
    delete ui;
}

void ParseDialog::setState(int state)
{
    ui->progressBar->setValue(state);
#ifdef Q_OS_WIN
    progress->setValue(state);
#endif
}

void ParseDialog::handleDone()
{
#ifdef Q_OS_WIN
    progress->reset();
    progress->hide();
#endif
    ui->pushButton->setEnabled(true);
    ui->label->setText(tr("Parsing completed."));
}


void ParseDialog::on_pushButton_clicked()
{
    close();
}
