#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>

class QNetworkReply;

class NetWorker : public QObject
{
    Q_OBJECT
public:
    static NetWorker* instance();
    ~NetWorker();
    
    void get(const QString &url);
    
signals:
    void finished(QNetworkReply *reply);
    
private:
    class Private;
    friend class Private;
    Private *d;
    
    explicit NetWorker(QObject *parent = nullptr);
    NetWorker(const NetWorker &) = delete;
    NetWorker& operator=(NetWorker) = delete;
};

#endif // NETWORKER_H
