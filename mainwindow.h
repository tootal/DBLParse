#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QXmlStreamReader;
class QSettings;
class Parser;
class ParseDialog;
class QFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    QAction *openAction;
    QXmlStreamReader *reader;
    QSettings *settings;
    Parser *parser;
    QThread *parseThread;
    ParseDialog *parseDialog;
    QFile *parseFile;
    
    void openFile();
    void parseDone();
};
#endif // MAINWINDOW_H
