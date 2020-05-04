#pragma once

#include <QMainWindow>
#include <QThread>

class Parser;
class ParseDialog;
class Finder;
class Loader;
class Calculator;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static const qint64 PROMOT_FILE_SIZE = (1 << 26);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void showAboutBox(QPixmap icon, const QString &info);
    
private slots:
    void on_actionAboutQt_triggered();
    
    void on_actionAboutDBLParse_triggered();
    
    void on_actionAboutDBLP_triggered();
    
    void on_actionExit_triggered();
    
    void on_actionOpen_triggered();
    
    void on_actionStatus_triggered();
    
    void on_actionClearIndex_triggered();
    
    void on_actionOpenIndexFolder_triggered();
    
    void on_actionAuthorStac_triggered();

    void on_actionViewLog_triggered();
    
    void on_actionSettings_triggered();

    void on_actionKeyWord_triggered();
    
    void on_actionCountClique_triggered();
    
private:
    Ui::MainWindow *ui;
    Parser *m_parser;
    QThread m_parseThread;
    Finder *m_finder;
    Loader *m_loader;
    
    void load();
    
signals:
    void startParse();
};
