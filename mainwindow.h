#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QXmlStreamReader;
class QSettings;

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
    void openFile();
};
#endif // MAINWINDOW_H
