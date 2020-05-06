#include "webpage.h"

#include <QDesktopServices>

#include "util.h"

WebPage::WebPage(QObject *parent)
    :QWebEnginePage(parent)
{
    
}

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
//    qDebug() << url;
    if (url.scheme() == "data")
        return true;
    if(url.scheme() == "qrc")
        return true;
    if(url.scheme() == "dblp") {
        emit request(url);
        return false;
    }
    if(url.scheme() == "year") {
        emit wordCloud(url);
        return false;
    }
    QDesktopServices::openUrl(url);
    return false;
}
