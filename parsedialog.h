#ifndef PARSEDIALOG_H
#define PARSEDIALOG_H

#include <QDialog>

class QVBoxLayout;
class QProgressBar;
class QLabel;
class QButton;

class ParseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParseDialog(QWidget *parent = nullptr);
    void showProgress(double ratio);
    
private:
    QVBoxLayout *layout;
    QProgressBar *progressBar;
    QLabel *label;
    QButton *button;
};

#endif // PARSEDIALOG_H
