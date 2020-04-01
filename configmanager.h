#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>

class QSettings;

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    
    QVariant value(const QString &key) const;
    void setValue(const QString &key, const QString &value);
    
private:
    QSettings *m_settings;
};

#endif // CONFIGMANAGER_H
