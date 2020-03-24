#include "webenginepage.h"

#include <QDesktopServices>

bool WebEnginePage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
    // Only allow qrc:/index.html
    if(url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
