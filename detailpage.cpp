#include "detailpage.h"

#include <QDesktopServices>

DetailPage::DetailPage(QObject *parent)
    :QWebEnginePage(parent)
{
    
}

bool DetailPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
    if(url.scheme() == "data")
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
