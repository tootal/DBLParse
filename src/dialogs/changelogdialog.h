#pragma once

#include <QDialog>

namespace Ui {
class ChangeLogDialog;
}

class ChangeLogDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChangeLogDialog(QWidget *parent = nullptr);
    ~ChangeLogDialog();
    
private:
    Ui::ChangeLogDialog *ui;
};

