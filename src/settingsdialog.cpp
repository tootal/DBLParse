#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>

#include "util.h"
#include "configmanager.h"
#include "application.h"

extern ConfigManager *g_config;

const QByteArray 
SettingsDialog::SystemLanguage = QT_TR_NOOP("System");

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItems(
        QVector<QString>(Util::s_availableLanguages.size() + 1).toList());
    retranslateUi();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::retranslateUi()
{
    auto &box = ui->comboBox;
    box->setItemText(0, 
        QApplication::translate("SettingsDialog", SystemLanguage));
    box->setItemData(0, "System");
    auto &langs = Util::s_availableLanguages;
    int cur = 0;
    for (int i = 1; i <= langs.size(); i++) {
        box->setItemText(i,
            QApplication::translate("Util", langs[i-1].second));
        box->setItemData(i, langs[i-1].first);
        if (langs[i-1].first == App->config->value("language"))
            cur = i;
    }
    box->setCurrentIndex(cur);
}

void SettingsDialog::on_pushButton_clicked()
{
    close();
}

void SettingsDialog::on_comboBox_activated(int index)
{
    auto lang = ui->comboBox->itemData(index).toString();
    QString locale = Util::getLocale();
    if (App->config->value("language") != lang)
        App->config->setValue("language", lang);
    if (locale != Util::getLocale()) emit languageChanged(Util::getLocale());
}

void SettingsDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        retranslateUi();
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(e);   
}
