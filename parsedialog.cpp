#include "parsedialog.h"

#include <QLabel>
#include <QProgressBar>
#include <QBoxLayout>
#include <QPushButton>
#include <QVariant>

ParseDialog::ParseDialog(QWidget *parent) : QDialog(parent)
{
    setMinimumSize(400, 120);
    label = new QLabel;
    progressBar = new QProgressBar;
    progressBar->setRange(0,10000);
    button = new QPushButton;
    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(button);
    buttonLayout->addStretch();
    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(progressBar);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    
    connect(button, &QPushButton::clicked, this, [this](){
        if(button->property("type").toString() == "close"){
            this->close();
        }else{
            abortFlag = true;
            emit abortParse();
        }
    });
    clear();
}

void ParseDialog::showProgress(double ratio)
{
    progressBar->setValue(ratio*10000);
}

void ParseDialog::showDone(int count)
{
    if(abortFlag){
        label->setText(tr("Parsing Abort. Processed %1 records.").arg(count));
    }else{
        label->setText(tr("Parsing Done. %1 records.").arg(count));
        progressBar->setValue(10000);
    }
    button->setText(tr("Close"));
    button->setProperty("type", QVariant("close"));
}

void ParseDialog::clear()
{
    label->setText(tr("Parsing..."));
    abortFlag = false;
    progressBar->setValue(0);
    button->setText(tr("Abort"));
    button->setProperty("type", QVariant("abort"));
}
