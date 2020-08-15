#pragma once

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget *parent);
    
signals:
    void clicked();
    
protected:
    void mousePressEvent(QMouseEvent *ev) override;
};

