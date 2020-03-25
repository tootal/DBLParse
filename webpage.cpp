#include "webpage.h"

#include <QDesktopServices>

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
    // Only allow qrc
    if(url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}