#include "downloaddialog.h"
#include "ui_downloaddialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::on_yesButton_clicked()
{
    auto n = ui->radioLayout->count();
    for (int i = 0; i < n; i++) {
        auto x = qobject_cast<QRadioButton*>(
                    ui->radioLayout->itemAt(i)->widget());
        if (x->isChecked()) {
            QDesktopServices::openUrl(QUrl(x->text()));
            break;
        }
    }
    close();
}

void DownloadDialog::on_noButton_clicked()
{
    close();
}
