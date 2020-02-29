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
    void showProgress(double ratio);
    void showDone(Parser *parser);
    void clear();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::ParseDialog *ui;
    bool abortFlag;
    
signals:
    void abortParse();
};

#endif // PARSEDIALOG_H
