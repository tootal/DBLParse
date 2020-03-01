#include "recordcountdialog.h"
#include "ui_recordcountdialog.h"

RecordCountDialog::RecordCountDialog(const QMap<QString, QVariant> &recordCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordCountDialog)
{
    ui->setupUi(this);
    auto &v = recordCount;
    ui->tableWidget->setRowCount(v.size());
    int j = 0;
    for(auto i = v.begin(); i != 
        v.end(); ++i){
        ui->tableWidget->setItem(j, 0, new QTableWidgetItem(i.key()));
        ui->tableWidget->setItem(j, 1, new QTableWidgetItem(i.value().toString()));
        ++j;
    }
    ui->tableWidget->resizeColumnsToContents();
}

RecordCountDialog::~RecordCountDialog()
{
    delete ui;
}
