#ifndef AUTHORCHARCOUNTDIALOG_H
#define AUTHORCHARCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorCharCountDialog;
}

class AuthorCharCountDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AuthorCharCountDialog(const QMap<QString, QVariant> &v,
                                   QWidget *parent = nullptr);
    ~AuthorCharCountDialog();
    
private:
    Ui::AuthorCharCountDialog *ui;
};

#endif // AUTHORCHARCOUNTDIALOG_H
