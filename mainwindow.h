#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

class Parser;
class ParseDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_actionAbout_Qt_triggered();
    
    void on_action_About_Dblparse_triggered();
    
    void on_actionAbout_DBLP_triggered();
    
    void on_actionE_xit_triggered();
    
    void on_searchButton_clicked();
    
    void on_action_Open_triggered();
    
    void on_action_Status_triggered();
    
    void on_authorRadioButton_clicked();
    
    void on_titleRadioButton_clicked();

    void on_coauthorRadioButton_clicked();

    void on_fuzzyRadioButton_clicked();
    
private:
    Ui::MainWindow *ui;
    Parser *m_parser;
    ParseDialog *m_parseDialog;
    
    void resume();
    
protected:
    void resizeEvent(QResizeEvent *event) override;
    
signals:
    void startParse(const QString &fileName);
};
#endif // MAINWINDOW_H
