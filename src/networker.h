#pragma once

#include <QObject>

class QNetworkReply;
class QNetworkAccessManager;

class NetWorker : public QObject
{
    Q_OBJECT
public:
    static NetWorker* instance();
    ~NetWorker();
    QNetworkReply* get(const QString &url);
    QStringList supportedSchemes();
    
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

class NetWorker::Private 
{
public:
    Private(NetWorker *q);
    
    QNetworkAccessManager *manager; 
};
