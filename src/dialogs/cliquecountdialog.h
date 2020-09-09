#pragma once

#include <QDialog>

namespace Ui {
class CliqueCountDialog;
}

class CliqueCountDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CliqueCountDialog(QWidget *parent = nullptr);
    ~CliqueCountDialog();
    
private:
    Ui::CliqueCountDialog *ui;
    void setItem(int row, int col, const QString &data);
};

