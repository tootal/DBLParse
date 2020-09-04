#pragma once

#include <QDialog>

class AuthorStacModel;
struct AuthorStac;

namespace Ui {
class AuthorStacDialog;
}

class AuthorStacDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AuthorStacDialog(const QVector<AuthorStac> &authorStacs, 
                              QWidget *parent = nullptr);
    ~AuthorStacDialog();
    
private:
    Ui::AuthorStacDialog *ui;
    void setItem(int row, int col, const QByteArray &text);
    void setItem(int row, int col, int text);
};

