#include "configmanager.h"

#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>

#include "util.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
#if defined (Q_OS_WIN64)
    settings = new QSettings("DBLParse.ini" ,QSettings::IniFormat, this);
#else
    settings = new QSettings(QSettings::IniFormat, 
                             QSettings::UserScope, 
                             QCoreApplication::organizationName(),
                             QCoreApplication::applicationName(),
                             this);
#endif
    init();
}

void ConfigManager::init()
{
    setDefault("lastOpenFileName", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    setDefault("language", "System");
}

QVariant ConfigManager::value(const QString &key) const
{
    return settings->value(key);
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

void ConfigManager::setDefault(const QString &key, const QVariant &value)
{
    if(contains(key)) return ;
    setValue(key, value);
}

bool ConfigManager::contains(const QString &key) const
{
    return settings->contains(key);
}
