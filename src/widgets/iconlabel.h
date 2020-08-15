#pragma once

#include <QWidget>
#include <QIcon>

class IconLabel : public QWidget
{
    Q_OBJECT
public:
    explicit IconLabel(QWidget *parent, QIcon icon, QSize size);
    
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;
    
    void setIcon(QIcon icon);
private:
    QSize m_size;
    QIcon m_icon;
};

