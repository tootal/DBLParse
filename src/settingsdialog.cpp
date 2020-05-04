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
    ui->comboBox->addItem(tr("System"), "System"); // 0
    auto &langs = Util::s_availableLanguages;
    int cur = 0;
    for (int i = 1; i <= langs.size(); i++) {
        ui->comboBox->addItem(langs[i-1].second, langs[i-1].first);
        if (langs[i-1].first == g_config->value("language"))
            cur = i;
    }
    ui->comboBox->setCurrentIndex(cur);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::retranslateUi()
{
    
}

void SettingsDialog::on_pushButton_clicked()
{
    close();
}

void SettingsDialog::on_comboBox_activated(int index)
{
    auto lang = ui->comboBox->itemData(index).toString();
    QString locale = Util::getLocale();
    if (g_config->value("language") != lang)
        g_config->setValue("language", lang);
    if (locale != Util::getLocale()) emit languageChanged();
}

void SettingsDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        qDebug() << "language changed";
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(e);   
}
