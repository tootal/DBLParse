#include "downloaddialog.h"
#include "ui_downloaddialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QNetworkReply>
#include <QRegularExpression>

#include "networker.h"
#include "util.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    initDownloadSources();
    getDownloadList(ui->comboBox->currentText());
    connect(ui->comboBox, &QComboBox::currentTextChanged,
            this, [this](const QString &source) {
        getDownloadList(source);
    });
    ui->tableWidget->setHorizontalHeaderLabels({
        tr("File"),
        tr("Last Modified"),
        tr("Size")
    });
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::initDownloadSources()
{
    ui->comboBox->addItems(Util::availableDownloadSources);
}

void DownloadDialog::getDownloadList(const QString &source)
{
    
    auto networker = NetWorker::instance();
    networker->get(source + "release/");
    connect(networker, &NetWorker::finished,
            this, [this](QNetworkReply *reply) {
        auto html = reply->readAll();
        reply->deleteLater();
        QRegularExpression re(
R"(<td><a href=".*\.xml\.gz">(.*)<\/a><\/td><td align="right">(.*)<\/td><td align="right">(.*?)<\/td>)"
        );
        auto i = re.globalMatch(html);
        QVector<DblpRelease> rs;
        while (i.hasNext()) {
            auto match = i.next();
            auto fileName = match.captured(1);
            auto lastModified = match.captured(2);
            auto size = match.captured(3).trimmed();
            rs.append({fileName, lastModified, size});
        }
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(rs.size());
        for (int j = 0; j < rs.size(); j++) {
            ui->tableWidget->setItem(j, 0, new QTableWidgetItem(rs[j].fileName));
            ui->tableWidget->setItem(j, 1, new QTableWidgetItem(rs[j].lastModified));
            ui->tableWidget->setItem(j, 2, new QTableWidgetItem(rs[j].size));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->setCurrentCell(0, 0);
    });
}

void DownloadDialog::on_pushButton_clicked()
{
    auto src = ui->comboBox->currentText();
    int row = ui->tableWidget->currentRow();
    auto fileName = ui->tableWidget->item(row, 0)->text();
    QDesktopServices::openUrl(src + "release/" + fileName);
}
