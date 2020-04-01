#include "configmanager.h"

#include <QSettings>
#include <QStandardPaths>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings("DBLParse.ini" ,QSettings::IniFormat, this);
    m_settings->setValue("version", "2.2");
    m_settings->setValue("lastOpenFileName", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    m_settings->setValue("language", "System");
}

QString ConfigManager::value(const QString &key) const
{
    return m_settings->value(key).toString();
}

void ConfigManager::setValue(const QString &key, const QString &value)
{
    m_settings->setValue(key, value);
}
