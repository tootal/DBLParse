#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SCUT_CS");
    QCoreApplication::setApplicationName("DBLParse");
    MainWindow w;
    w.show();
    return a.exec();
}
