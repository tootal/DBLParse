#pragma once

#include <QObject>

class QSettings;

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    void init();
    QString value(const QString &key) const;
    void setValue(const QString &key, const QString &value);
    void setDefault(const QString &key, const QString &value);
    
private:
    QSettings *m_settings;
};
