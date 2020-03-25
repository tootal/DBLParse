#include "webpage.h"

#include <QDesktopServices>

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool)
{
    return true;
}
