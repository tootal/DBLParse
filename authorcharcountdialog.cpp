#include "authorcharcountdialog.h"
#include "ui_authorcharcountdialog.h"

#include <QDebug>

AuthorCharCountDialog::AuthorCharCountDialog(const QMap<QString, QVariant> &v,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorCharCountDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(v.size());
//    qDebug() << v;
    int j = 0;
    for(auto i = v.begin(); i != v.end(); ++i){
        auto *keyItem = new QTableWidgetItem("\'"+i.key()+"\'");
        keyItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(j, 0, keyItem);
        auto *valueItem = new QTableWidgetItem;
        valueItem->setData(Qt::DisplayRole, i.value());
        valueItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(j, 1, valueItem);
        ++j;
    }
}

AuthorCharCountDialog::~AuthorCharCountDialog()
{
    delete ui;
}
