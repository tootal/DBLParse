#include "documentdialog.h"
#include "ui_documentdialog.h"

#include "util.h"

DocumentDialog::DocumentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentDialog)
{
    ui->setupUi(this);
    auto data = Util::readFile(tr(":/docs/README.md"));
    ui->textEdit->setMarkdown(data);
}

DocumentDialog::~DocumentDialog()
{
    delete ui;
}
