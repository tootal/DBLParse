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

QNetworkReply* NetWorker::get(const QString &url)
{
    QUrl uri(url);
    QNetworkRequest request;
    if (uri.scheme() == "https") {
        QSslConfiguration config = request.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1_0);
        request.setSslConfiguration(config);
    }
    request.setUrl(uri);
    return d->manager->get(request);
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
