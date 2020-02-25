#include "parsedialog.h"
#include "parser.h"

#include <QLabel>
#include <QProgressBar>
#include <QBoxLayout>
#include <QPushButton>
#include <QVariant>
#include <QTime>

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

void ParseDialog::showDone(Parser *parser)
{
    QString processInfo;
    if(abortFlag){
        processInfo = "Abort";
    }else{
        processInfo = "Successful";
        progressBar->setValue(10000);
    }
    label->setText(tr("Parsing %1.\nProcessed %2 records in %3 .")
            .arg(processInfo)
            .arg(parser->count())
            .arg(formatTime(parser->parseCostTime())));
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

QString ParseDialog::formatTime(QTime time)
{
    if(time.hour() == 0){
        if(time.minute() == 0){
            return time.toString("s.zzz")+" s";
        }else{
            return time.toString("m:ss.z");
        }
    }else{
        return time.toString("H:mm:ss");
    }
}
