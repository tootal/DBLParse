#include "cliquecountdialog.h"
#include "ui_cliquecountdialog.h"

#include <QFile>
#include <QDataStream>

CliqueCountDialog::CliqueCountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CliqueCountDialog)
{
    ui->setupUi(this);
    QFile file("data/authorclique");
    QDataStream s(&file);
    file.open(QFile::ReadOnly);
    qint32 total;
    s >> total;
    ui->tableWidget->setRowCount(total);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels({
        tr("Number of Cliques")
    });
    for (int i = 1; i <= total; i++) {
        QString data;
        s >> data;
        setItem(i - 1, 0, data);
    }
}

CliqueCountDialog::~CliqueCountDialog()
{
    delete ui;
}

void CliqueCountDialog::setItem(int row, int col, const QString &data)
{
    ui->tableWidget->setItem(row, col, new QTableWidgetItem(data));
}
