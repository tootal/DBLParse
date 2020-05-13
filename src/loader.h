#pragma once

#include <QObject>

class Finder;

class Loader : public QObject
{
    Q_OBJECT
public:
    explicit Loader(Finder *p_finder = nullptr, QObject *parent = nullptr);
    void run();
    
private:
    Finder *finder;
    
signals:
    void stateChanged(const QString &state);
    void loadDone();
    void authorLoadDone();
    void titleLoadDone();
    void authorStacLoadDone();
    void yearWordLoadDone();
    void titleWordLoadDone();
};

