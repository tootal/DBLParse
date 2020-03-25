#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QWebEnginePage>

class WebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit WebPage(QObject *parent = nullptr) : QWebEnginePage(parent) {}
    
protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};

#endif // WEBENGINEPAGE_H
