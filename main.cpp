#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SCUT_CS");
    QCoreApplication::setApplicationName("DBLParse");
    MainWindow w;
    w.show();
    return a.exec();
}
