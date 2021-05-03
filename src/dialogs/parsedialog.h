#pragma once

#include <QDialog>

#include "parser.h"

#ifdef Q_OS_WIN
class QWinTaskbarProgress;
class QWinTaskbarButton;
#endif

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
#ifdef Q_OS_WIN
    QWinTaskbarButton *button;
    QWinTaskbarProgress *progress;
#endif
};
