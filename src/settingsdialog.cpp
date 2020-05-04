#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>

#include "util.h"
#include "configmanager.h"

extern ConfigManager *g_config;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem(tr("System"), "System");
    
    for (const auto &lang : Util::availableLanguages()) {
        ui->comboBox->addItem(lang.second, lang.first);
    }
    
    for(int i = 0; i < ui->comboBox->count(); ++i) {
        if(ui->comboBox->itemData(i).toString() == g_config->value("language")) {
            ui->comboBox->setCurrentIndex(i);
        }
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

void SettingsDialog::on_comboBox_activated(int index)
{
    g_config->setValue("language", ui->comboBox->itemData(index).toString());
}
