#include "downloaddialog.h"
#include "ui_downloaddialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>

#include "networker.h"
#include "util.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    initDownloadSources();
//    auto networker = NetWorker::instance();
//    qDebug() << QSslSocket::supportsSsl();
//    qDebug() << QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << QSslSocket::sslLibraryVersionString();
//    qDebug() << networker->supportedSchemes();
//    networker->get("https://dblp.org/xml/README.txt");
//    connect(networker, &NetWorker::finished,
//            this, [](QNetworkReply *reply) {
//        qDebug() << reply->error();
//        qDebug() << reply->readAll();
//    });
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::on_yesButton_clicked()
{
    QDesktopServices::openUrl(QUrl(ui->comboBox->currentText()));
    close();
}

void DownloadDialog::on_noButton_clicked()
{
    close();
}

void DownloadDialog::initDownloadSources()
{
    ui->comboBox->addItems(Util::availableDownloadSources);
}
