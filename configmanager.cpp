#include "configmanager.h"

#include <QSettings>
#include <QStandardPaths>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings("DBLParse.ini" ,QSettings::IniFormat, this);
    init();
}

void ConfigManager::init()
{
    setDefault("version", "2.4");
    setDefault("lastOpenFileName", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    setDefault("language", "System");
}

QString ConfigManager::value(const QString &key) const
{
    return m_settings->value(key).toString();
}

void ConfigManager::setValue(const QString &key, const QString &value)
{
    m_settings->setValue(key, value);
}

void ConfigManager::setDefault(const QString &key, const QString &value)
{
    if(m_settings->contains(key)) return ;
    else setValue(key, value);
}
