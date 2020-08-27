#include "authorstacdialog.h"
#include "ui_authorstacdialog.h"

#include "models/authorstacmodel.h"

AuthorStacDialog::AuthorStacDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorStacDialog)
{
    ui->setupUi(this);
    model = new AuthorStacModel{};
    ui->tableView->setModel(model);
}

AuthorStacDialog::~AuthorStacDialog()
{
    delete ui;
    delete model;
}
