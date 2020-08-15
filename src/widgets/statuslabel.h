#pragma once

#include <QWidget>

namespace Ui {
class StatusLabel;
}

class StatusLabel : public QWidget
{
    Q_OBJECT
    static constexpr int iconSize = 16;
public:
    explicit StatusLabel(QWidget *parent = nullptr);
    ~StatusLabel();
    void setOk();
    void setNo();
    
signals:
    void clicked();
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    Ui::StatusLabel *ui;
    QPixmap okIcon, noIcon;
};

