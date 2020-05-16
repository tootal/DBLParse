#include "webview.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QPlainTextEdit>

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
    menu->setAttribute(Qt::WA_DeleteOnClose, true);
    if (hasSelection()) {
        QAction *copy = pageAction(QWebEnginePage::Copy);
        copy->setText(tr("Copy"));
        menu->addAction(copy);
    }
    auto tools = new QAction(tr("Dev Tools"), this);
    connect(tools, &QAction::triggered,
            this, [this]() {
        auto view = new QWebEngineView(this);
        view->setWindowFlag(Qt::Window);
        view->resize(800, 600);
        page()->setDevToolsPage(view->page());
        view->show();
    });
    menu->addAction(tools);
    
    auto source = new QAction(tr("View Source"), this);
    connect(source, &QAction::triggered,
            this, [this]() {
        page()->toHtml([this](const QString &s) {
            auto view = new QPlainTextEdit(this);
            view->setReadOnly(true);
            view->setPlainText(s);
            view->setWindowFlag(Qt::Window);
            view->resize(800, 600);
            view->show();
        });
    });
    menu->addAction(source);
    menu->popup(event->globalPos());
}
