#include "webpage.h"

#include <QDesktopServices>

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
//    qDebug() << url;
    if(url.scheme() == QString("qrc"))
        return true;
    else if(url.scheme() == QString("dblp")){
        emit request(url);
        return false;
    }
    QDesktopServices::openUrl(url);
    return false;
}
