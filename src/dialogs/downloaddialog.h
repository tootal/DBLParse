#pragma once

#include <QDialog>

namespace Ui {
class DownloadDialog;
}

struct DblpRelease {
    QString fileName;
    QString lastModified;
    QString size;
};

class DownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DownloadDialog(QWidget *parent = nullptr);
    ~DownloadDialog();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::DownloadDialog *ui;
    static const QStringList tableLables;
    void initDownloadSources();
    void getDownloadList(const QString &source);
};

