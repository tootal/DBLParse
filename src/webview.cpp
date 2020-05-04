#include "webview.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "webpage.h"

WebView::WebView(QWidget *parent)
    :QWebEngineView(parent)
{
    m_page = new WebPage(this);
    setPage(m_page);
    m_channel = new QWebChannel(this);
    m_page->setWebChannel(m_channel);
}

WebPage *WebView::page() const
{
    return m_page;
}

void WebView::registerObject(const QString &id, QObject *object)
{
    m_channel->registerObject(id, object);
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    auto *menu = new QMenu(this);
    QAction *action = new QAction(tr("Inspector"));
    connect(action, &QAction::triggered,
            this, [this](){
        auto *view = new QWebEngineView(this);
        view->setWindowFlag(Qt::Window);
        view->resize(800, 600);
        page()->setDevToolsPage(view->page());
        view->show();
    });
    menu->addAction(action);
    menu->popup(event->globalPos());
}
