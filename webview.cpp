#include "webview.h"

#include <QMenu>
#include <QContextMenuEvent>

WebView::WebView(QWidget *parent)
    :QWebEngineView(parent)
{
    
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *action = new QAction(tr("Inspector"));
    connect(action, &QAction::triggered,
            this, [this](){
        QWebEngineView *view = new QWebEngineView();
        page()->setDevToolsPage(view->page());
        view->show();
    });
    menu->addAction(action);
    menu->popup(event->globalPos());
}
