#pragma once

#include <QWebEnginePage>

class WebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit WebPage(QObject *parent = nullptr);
    void onLanguageChanged();
    
signals:
    void request(QUrl url);
    void wordCloud(QUrl url);
    void languageChanged(const QString &language);
    
protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};
