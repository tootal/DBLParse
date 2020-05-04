#pragma once

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
    void changeEvent(QEvent *e) override;
    
private:
    WebPage *m_page;
    QWebChannel *m_channel;
};
