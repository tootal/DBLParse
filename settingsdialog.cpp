#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSettings>
#include <QAbstractButton>
#include <QDebug>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->listWidget->setMaximumWidth(ui->listWidget->sizeHintForColumn(0) + 10);
    QSettings settings;
    if(settings.contains("language")){
        QString language = settings.value("language").toString();
        if(language == "english") ui->comboBox->setCurrentIndex(1);
        else if(language == "chinese") ui->comboBox->setCurrentIndex(2);
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::applySettings()
{
    QSettings settings;
    int i = ui->comboBox->currentIndex();
    if(i == 0) settings.setValue("language", "system");
    else if(i == 1) settings.setValue("language", "english");
    else if(i == 2) settings.setValue("language", "chinese");
}

void SettingsDialog::on_buttonBox_accepted()
{
    applySettings();
    close();
}

void SettingsDialog::on_buttonBox_rejected()
{
    close();
}
