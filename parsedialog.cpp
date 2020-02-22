#include "parsedialog.h"

#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

ParseDialog::ParseDialog(QWidget *parent) : QDialog(parent)
{
    setMinimumSize(300, 50);
    label = new QLabel(tr("Parsing..."));
    progressBar = new QProgressBar;
    progressBar->setRange(0,10000);
    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(progressBar);
    setLayout(layout);
}

void ParseDialog::showProgress(double ratio)
{
    progressBar->setValue(ratio*10000);
}
