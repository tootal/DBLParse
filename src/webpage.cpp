#include "webpage.h"

#include <QDesktopServices>

WebPage::WebPage(QObject *parent)
    :QWebEnginePage(parent)
{
    
}

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
//    qDebug() << url;
    if (url.scheme() == "data") {
        return true;
    } else if(url.scheme() == "qrc") {
        return true;
    } else if(url.scheme() == "dblp") {
        emit request(url);
        return false;
    } else if(url.scheme() == "year") {
        emit wordCloud(url);
        return false;
    }
    QDesktopServices::openUrl(url);
    return false;
}
