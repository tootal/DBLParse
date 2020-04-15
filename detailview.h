#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#include <QWebEngineView>

#include "detailpage.h"

class DetailView : public QWebEngineView
{
    Q_OBJECT
public:
    DetailView(QWidget *parent = nullptr);
    DetailPage* page() const;
protected:
    void contextMenuEvent(QContextMenuEvent *) override;
    
private:
    DetailPage *m_page;
};

#endif // WEBVIEW_H
