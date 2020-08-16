#pragma once

#include <QDialog>

#include "parser.h"

namespace Ui {
class ParseDialog;
}

class ParseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParseDialog(QWidget *parent = nullptr);
    ~ParseDialog();
    
public slots:
    void setState(int state);
    void handleDone();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::ParseDialog *ui;
};
