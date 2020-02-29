#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
//    qDebug() << QLocale();
    qDebug() << QLocale().uiLanguages();
    if (translator.load(QLocale(), "DBLParse",
            "_", ":/translations")){
//        qDebug() << "translator load successful!";
        a.installTranslator(&translator);
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
