#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

class Parser;
class ParseDialog;
class Finder;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    
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
    
    void on_action_Clear_Index_triggered();
    
    void on_action_Settings_triggered();
    
private:
    Ui::MainWindow *ui;
    Parser *m_parser;
    Finder *m_finder;
    
protected:
    void resizeEvent(QResizeEvent *event) override;
    
};
#endif // MAINWINDOW_H
