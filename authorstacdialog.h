#ifndef AUTHORSTACDIALOG_H
#define AUTHORSTACDIALOG_H

#include <QDialog>
#include "finder.h"

namespace Ui {
class AuthorStacDialog;
}

class AuthorStacDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorStacDialog(QWidget *parent = nullptr,Finder *finder=nullptr);
    ~AuthorStacDialog();

private:
    Ui::AuthorStacDialog *ui;
};

#endif // AUTHORSTACDIALOG_H
