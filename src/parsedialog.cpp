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
    clear();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, 
            this, &ParseDialog::showMemory);
    timer->start(500);
}

ParseDialog::~ParseDialog()
{
    delete ui;
}

void ParseDialog::showStatus(const QString &msg)
{
    ui->textBrowser->append(msg);
}

void ParseDialog::showMemory()
{
    MEMORYSTATUSEX st;
    st.dwLength = sizeof(st);
    GlobalMemoryStatusEx(&st);
    auto used = st.ullTotalPhys - st.ullAvailPhys;
    ui->progressBar->setValue(qreal(used) / st.ullTotalPhys * 100);
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
