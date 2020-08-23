#pragma once

#include <QWidget>

namespace Ui {
class LoadingLabel;
}

class LoadingLabel : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoadingLabel(QWidget *parent = nullptr);
    ~LoadingLabel();
    void paintEvent(QPaintEvent *event) override;
    
private:
    Ui::LoadingLabel *ui;
};

