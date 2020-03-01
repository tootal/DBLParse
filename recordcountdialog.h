#ifndef RECORDCOUNTDIALOG_H
#define RECORDCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class RecordCountDialog;
}

class RecordCountDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RecordCountDialog(const QMap<QString, QVariant> &recordCount, 
                               QWidget *parent = nullptr);
    ~RecordCountDialog();
    
private:
    Ui::RecordCountDialog *ui;
};

#endif // RECORDCOUNTDIALOG_H
