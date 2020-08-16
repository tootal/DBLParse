#include "statusdialog.h"
#include "ui_statusdialog.h"

#include <QFileInfo>
#include <QDateTime>

#include "util.h"
#include "application.h"
#include "configmanager.h"

StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);
    if (Util::parsed()) {
        ui->stackedWidget->setCurrentIndex(0);
        auto filePath = Util::getXmlFileName();
        ui->dataPathLineEdit->setText(filePath);
        auto fileName = QFileInfo(filePath).fileName();
        ui->fileNameLabel->setText(fileName);
        connect(ui->copyPathButton, &QPushButton::clicked,
                this, [filePath]() {
            Util::setClipboardText(filePath);
        });
        connect(ui->openFolderButton, &QPushButton::clicked,
                this, [filePath]() {
            Util::openFolder(filePath);
        });
        ui->dataSizeLabel->setText(Util::getFileReadableSize(filePath));
        ui->parsedTimeLabel->setText(
            Util::displayDateTime(
                App->config->value("lastDateTime").toDateTime()
            )
        );
        ui->costTimeLabel->setText(
            Util::displayCostTime(
                App->config->value("costMsecs").toInt()
            )
        );
        ui->articleCountLabel->setText(
            QString::number(App->config->value("articleCount").toInt())
        );
        ui->authorCountLabel->setText(
            QString::number(App->config->value("authorCount").toInt())
        );
    } else {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

StatusDialog::~StatusDialog()
{
    delete ui;
}
