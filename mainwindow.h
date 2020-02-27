#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QXmlStreamReader;
class QSettings;
class Parser;
class ParseDialog;
class QFile;
class QTextBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    QAction *fileOpenAction;
    QAction *fileInfoAction;
    QAction *useNetworkDataAction;
    QAction *useLocalDataAction;
    QTextBrowser *textBrowser;
    
    QXmlStreamReader *reader;
    QSettings *settings;
    Parser *parser;
    ParseDialog *parseDialog;
    QFile *parseFile;
    
    void openFile();
    void parseDone();
    void resume();
    void search(QString word);
    void searchLocal(QString word);
    void searchNetwork(QString word);
};
#endif // MAINWINDOW_H
