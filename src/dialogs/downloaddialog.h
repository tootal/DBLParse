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
    
    void on_releasesCheckBox_stateChanged(int);
    
    void on_latestCheckBox_stateChanged(int);
    
private:
    Ui::DownloadDialog *ui;
    static const QStringList tableLables;
    void initDownloadSources();
    void refresh();
    void append(const DblpRelease &data);
    void getReleases(const QString &source);
    void getLatest(const QString &source);
    void getDownloadList(const QString &source);
};

