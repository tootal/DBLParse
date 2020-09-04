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
    ui->tableWidget->setRowCount(authorStacs.size());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({
        tr("Author Name"),
        tr("Number of Publications")
    });
    for (int i = 0; i < authorStacs.size(); i++) {
        setItem(i, 0, authorStacs[i].author);
        setItem(i, 1, authorStacs[i].stac);
    }
    ui->tableWidget->resizeColumnsToContents();
}

AuthorStacDialog::~AuthorStacDialog()
{
    delete ui;
}

void AuthorStacDialog::setItem(int row, int col, const QByteArray &text)
{
    ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(text)));
}

void AuthorStacDialog::setItem(int row, int col, int text)
{
    setItem(row, col, QByteArray::number(text));
}
