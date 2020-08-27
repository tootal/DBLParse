#include "authorstacdialog.h"
#include "ui_authorstacdialog.h"

#include "parser.h"
#include "models/authorstacmodel.h"

AuthorStacDialog::AuthorStacDialog(const QVector<AuthorStac> &authorStacs, 
                                   QWidget *parent)
    : QDialog(parent),
      ui(new Ui::AuthorStacDialog)
{
    ui->setupUi(this);
    model = new AuthorStacModel{authorStacs};
    ui->tableView->setModel(model);
}

AuthorStacDialog::~AuthorStacDialog()
{
    delete ui;
    delete model;
}
