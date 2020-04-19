#ifndef LOADER_H
#define LOADER_H

#include <QThread>

class Loader : public QThread
{
    Q_OBJECT
public:
    explicit Loader(QObject *parent = nullptr);
    void run() override;
    
signals:
    void stateChanged(const QString &state);
    void loadDone();
    void authorLoadDone();
    void titleLoadDone();
    void authorStacLoadDone();
    void yearWordLoadDone();
};

#endif // LOADER_H
