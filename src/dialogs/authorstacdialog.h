#pragma once

#include <QDialog>

class AuthorStacModel;

namespace Ui {
class AuthorStacDialog;
}

class AuthorStacDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AuthorStacDialog(QWidget *parent = nullptr);
    ~AuthorStacDialog();
    
private:
    Ui::AuthorStacDialog *ui;
    AuthorStacModel *model;
};

