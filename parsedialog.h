#ifndef PARSEDIALOG_H
#define PARSEDIALOG_H

#include <QDialog>

class QVBoxLayout;
class QHBoxLayout;
class QProgressBar;
class QLabel;
class QPushButton;

class ParseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParseDialog(QWidget *parent = nullptr);
    void showProgress(double ratio);
    void showDone(int count);
    void clear();
    
signals:
    void abortParse();
private:
    QVBoxLayout *layout;
    QHBoxLayout *buttonLayout;
    QProgressBar *progressBar;
    QLabel *label;
    QPushButton *button;
    bool abortFlag;
};

#endif // PARSEDIALOG_H
