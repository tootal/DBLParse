#pragma once

#include <QWidget>
#include <QIcon>
#include <QSvgWidget>

class StatusLabel : public QWidget
{
    Q_OBJECT
public:
    explicit StatusLabel(QWidget *parent = nullptr);
    
signals:
    void clicked();
    
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    
private:
    QPixmap okIcon, noIcon;
};

