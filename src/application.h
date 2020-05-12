#pragma once

#include <QApplication>

#include "mainwindow.h"

#define App (static_cast<Application *>(qApp))

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
};

