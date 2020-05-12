#pragma once

#include <QMainWindow>
#include <QThread>

class Parser;
class ParseDialog;
class Finder;
class Loader;
class Calculator;
class QTranslator;

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
    void showAboutBox(QPixmap pixmapIcon, const QString &info);
    void setTranslator(QTranslator *translator_);
    
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
    
    
protected:
    void changeEvent(QEvent *e) override;
    
private:
    Ui::MainWindow *ui;
    QTranslator *translator;
    Finder *m_finder;
    
    void load();
    void onLanguageChanged(const QString &locale);
    
signals:
    void startParse();
    void startLoad();
};
