#include "authorstacdialog.h"
#include "ui_authorstacdialog.h"
#include "finder.h"

#include <QMessageBox>

AuthorStacDialog::AuthorStacDialog(QWidget *parent,Finder *finder) :
    QDialog(parent),
    ui(new Ui::AuthorStacDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, static_cast<int>(width() * 0.5));
    QList<QPair<QString,int> > authorStac=finder->returnAuthorStac();

    if(authorStac.isEmpty()){
        QMessageBox::information(this, tr("Information"),
                                 tr("please parse first."));
        return ;
    }
    ui->tableWidget->clearContents();

    int num=authorStac.size()<=100?authorStac.size():100;
    ui->tableWidget->setRowCount(num);

        for(qint32 t=0;t<num;t++){
            ui->tableWidget->setItem(t, 0, new QTableWidgetItem(authorStac[t].first));
            ui->tableWidget->setItem(t, 1, new QTableWidgetItem(QString::number(authorStac[t].second)));
        }

    ui->tableWidget->resizeRowsToContents();
}

AuthorStacDialog::~AuthorStacDialog()
{
    delete ui;
}
