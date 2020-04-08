#include "detailview.h"
#include "detailpage.h"

#include <QMenu>
#include <QContextMenuEvent>

DetailView::DetailView(QWidget *parent)
    :QWebEngineView(parent)
{
    resize(800, 600);
    m_page = new DetailPage(this);
    setPage(m_page);
}

void DetailView::contextMenuEvent(QContextMenuEvent *event)
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
