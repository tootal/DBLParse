#pragma once

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    static const QByteArray SystemLanguage;
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;
    void retranslateUi();
signals:
    void languageChanged(const QString &locale);
    
private slots:
    void on_pushButton_clicked();
    
    void on_comboBox_activated(int index);
    
protected:
    void changeEvent(QEvent *e) override;
    
private:
    Ui::SettingsDialog *ui;
};
