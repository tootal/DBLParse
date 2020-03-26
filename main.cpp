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
    QSettings settings;
    QTranslator translator;
    QString language = "system";
    if(settings.contains("language")){
        language = settings.value("language").toString();
    }
    if(language == "system" && QLocale::system().name() == "zh_CN"){
        language = "chinese";
    }
    if(language == "chinese"){
        if(translator.load("DBLParse_zh_CN", ":/translations")){
            a.installTranslator(&translator);
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
