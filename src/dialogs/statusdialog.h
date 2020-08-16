#pragma once

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StatusDialog(QWidget *parent = nullptr);
    ~StatusDialog();
    
private:
    Ui::StatusDialog *ui;
};

