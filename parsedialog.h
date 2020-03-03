#ifndef PARSEDIALOG_H
#define PARSEDIALOG_H

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
    void showStatus(const QString &msg);
    void clear();
    void activeButton();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::ParseDialog *ui;
};

#endif // PARSEDIALOG_H
