#include "changelogdialog.h"
#include "ui_changelogdialog.h"

#include "util.h"

ChangeLogDialog::ChangeLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeLogDialog)
{
    ui->setupUi(this);
    auto data = Util::readFile(tr(":/docs/changelog.md"));
    ui->textEdit->setMarkdown(data);
}

ChangeLogDialog::~ChangeLogDialog()
{
    delete ui;
}
