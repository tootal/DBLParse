#pragma once

#include <QDialog>

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DownloadDialog(QWidget *parent = nullptr);
    ~DownloadDialog();
    
private slots:
    void on_yesButton_clicked();
    
    void on_noButton_clicked();
    
private:
    Ui::DownloadDialog *ui;
    
    void initDownloadSources();
};

