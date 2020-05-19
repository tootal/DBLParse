#pragma once

#include <QObject>

class QSettings;

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    void init();
    QVariant value(const QString &key) const;
    void setValue(const QString &key, const QVariant &value);
    void setDefault(const QString &key, const QVariant &value);
    bool contains(const QString &key) const;
    
private:
    QSettings *settings;
};
