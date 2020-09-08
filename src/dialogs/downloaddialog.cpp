#include "downloaddialog.h"
#include "ui_downloaddialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QRegularExpression>

#include "networker.h"
#include "util.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    initDownloadSources();
    refresh();
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

void DownloadDialog::refresh()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tableWidget->setRowCount(0);
    getDownloadList(ui->comboBox->currentText());
}

void DownloadDialog::append(const DblpRelease &data)
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(data.fileName));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(data.lastModified));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(data.size));
}

void DownloadDialog::getReleases(const QString &source)
{
    auto networker = NetWorker::instance();
    auto reply = networker->get(source);
    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() {
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
        for (auto j : rs) {
            append(j);
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->setCurrentCell(0, 0);
        ui->stackedWidget->setCurrentIndex(0);
    });
}

void DownloadDialog::getLatest(const QString &source)
{
    auto networker = NetWorker::instance();
    auto reply = networker->get(source);
    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() {
        auto html = reply->readAll();
        reply->deleteLater();
        QRegularExpression re(
R"(<a href="dblp\.xml\.gz">dblp\.xml\.gz<\/a><\/td><td align="right">(.*?)<\/td><td align="right">(.*?)<\/td>)"
        );
        auto i = re.match(html);
        DblpRelease rs;
        if (i.hasMatch()) {
            rs.fileName = "dblp.xml.gz";
            rs.lastModified = i.captured(1);
            rs.size = i.captured(2);
        }
        append(rs);
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->setCurrentCell(0, 0);
        ui->stackedWidget->setCurrentIndex(0);
    });
}

void DownloadDialog::getDownloadList(const QString &source)
{
    if (ui->latestCheckBox->isChecked()) {
        getLatest(source);
    }
    if (ui->releasesCheckBox->isChecked()) {
        getReleases(source + "release/");
    }
}

void DownloadDialog::on_pushButton_clicked()
{
    auto src = ui->comboBox->currentText();
    int row = ui->tableWidget->currentRow();
    auto fileName = ui->tableWidget->item(row, 0)->text();
    if (fileName == "dblp.xml.gz") {
        QDesktopServices::openUrl(QUrl("https://dblp.org/xml/dblp.xml.gz"));
    } else {
        QDesktopServices::openUrl(src + "release/" + fileName);
    }
}

void DownloadDialog::on_releasesCheckBox_stateChanged(int)
{
    refresh();
}

void DownloadDialog::on_latestCheckBox_stateChanged(int)
{
    refresh();
}
