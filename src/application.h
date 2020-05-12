#pragma once

#include <QApplication>

class MainWindow;
class ConfigManager;

#define App (static_cast<Application *>(qApp))

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    
    MainWindow *window;
    ConfigManager *config;
};

