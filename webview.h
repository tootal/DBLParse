#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>
#include <QWebChannel>

#include "webpage.h"

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    WebView(QWidget *parent = nullptr);
    WebPage* page() const;
    void setWebChannel(QWebChannel *channel);
    void registerObject(const QString &id, QObject *object);

protected:
    void contextMenuEvent(QContextMenuEvent *) override;
    
private:
    WebPage *m_page;
    QWebChannel *m_channel;
};

#endif // WEBVIEW_H
