#pragma once

#include <QObject>

class Loader : public QObject
{
    Q_OBJECT
public:
    explicit Loader(QObject *parent = nullptr);
    void run();
    
signals:
    void stateChanged(const QString &state);
    void loadDone();
    void authorLoadDone();
    void titleLoadDone();
    void authorStacLoadDone();
    void yearWordLoadDone();
    void titleWordLoadDone();
};

