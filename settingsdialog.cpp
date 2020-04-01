#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "util.h"
#include "configmanager.h"

extern ConfigManager *g_config;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem(tr("System"), "System");
    foreach(const auto &lang, Util::availableLanguages()) {
        ui->comboBox->addItem(lang.second, lang.first);
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_pushButton_clicked()
{
    close();
}

void SettingsDialog::on_comboBox_currentIndexChanged(int)
{
    g_config->setValue("language", ui->comboBox->currentData().toString());
}
