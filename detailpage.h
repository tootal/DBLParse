#ifndef DETAILPAGE_H
#define DETAILPAGE_H

#include <QWebEnginePage>

class DetailPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit DetailPage(QObject *parent = nullptr);
    
signals:
    void request(QUrl url);
    
protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};

#endif // DETAILPAGE_H
