#include "networker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

NetWorker *NetWorker::instance()
{
    static NetWorker netWorker;
    return &netWorker;
}

NetWorker::~NetWorker()
{
    delete d;
    d = nullptr;
}

void NetWorker::get(const QString &url)
{
    d->manager->get(QNetworkRequest(QUrl(url)));
}

QStringList NetWorker::supportedSchemes()
{
    return d->manager->supportedSchemes();
}

NetWorker::NetWorker(QObject *parent) 
    : QObject(parent),
      d(new NetWorker::Private(this))
{
    connect(d->manager, QOverload<QNetworkReply*>::of(&QNetworkAccessManager::finished),
            this, &NetWorker::finished);
}

NetWorker::Private::Private(NetWorker *q)
    : manager(new QNetworkAccessManager(q))
{
    
}
