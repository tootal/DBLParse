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

void WebPage::triggerAction(QWebEnginePage::WebAction action, bool checked)
{
//    qDebug() << "Trigger Action: " << action;
    QWebEnginePage::triggerAction(action, checked);
}

QWebEnginePage *WebPage::createWindow(QWebEnginePage::WebWindowType type)
{
//    qDebug() << "Create Window: " << type;
    return QWebEnginePage::createWindow(type);
}
