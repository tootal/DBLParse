#include "mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QDateTime>
#include <QTranslator>
#include <QLocale>
#include <QSplashScreen>
#include <QDir>

#include "application.h"
#include "util.h"
#include "configmanager.h"

#ifdef QT_NO_DEBUG

static QFile logFile;

static void initLogFile(const QString &fileName)
{
    logFile.setFileName(fileName);
    logFile.open(QFile::Append | QFile::Text);
}

#endif // QT_NO_DEBUG

static void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString header;
    
    switch(type){
    case QtDebugMsg:
        header = "Debug: ";
        break;
    case QtInfoMsg:
        header = "Info: ";
        break;
    case QtWarningMsg:
        header = "Warning: ";
        break;
    case QtCriticalMsg:
        header = "Critical: ";
        break;
    case QtFatalMsg:
        header = "Fatal: ";
        break;
    }
    
    QString fileName = QFileInfo(context.file).fileName();
    QString text = header 
            + QString("(%1:%2) ").arg(fileName).arg(context.line)
            + msg;
#ifdef QT_NO_DEBUG
    QTextStream stream(&logFile);
    
    if (type == QtDebugMsg) {
        std::cerr << text.toStdString() << std::endl;
    } else {
        stream << text << QString("[%1]\n").arg(QDateTime::currentDateTime().toString());
    }
    
    if (type == QtFatalMsg) {
        logFile.close();
        abort();
    }
#else // QT_NO_DEBUG
    std::cerr << text.toStdString() << std::endl;
#endif // QT_NO_DEBUG
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SCUT_CS");
    QCoreApplication::setApplicationName("DBLParse");
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    Application a(argc, argv);
    
    ConfigManager config;
    a.config = &config;
    
    QSplashScreen splash(QPixmap(":/resources/splash.jpg").scaled(510, 357));
    if (App->config->contains("lastGeometry")) {
        auto g = splash.geometry();
        g.moveCenter(App->config->value("lastGeometry").toRect().center());
        splash.setGeometry(g);
    }
    splash.show();
    splash.setDisabled(true);
    
#ifdef QT_NO_DEBUG
    initLogFile(Util::getLogPath());
#endif // QT_NO_DEBUG
    qInstallMessageHandler(logger);
    
    qInfo() << "DBLParse start";
    
    QString locale = Util::getLocale();
    QLocale::setDefault(QLocale(locale));
    QTranslator translator;
    translator.load("DBLParse_" + locale, ":/");
    a.installTranslator(&translator);
    MainWindow w{};
    w.setTranslator(&translator);
    a.window = &w;
    a.setStyleSheet(Util::readFile(":/resources/DBLParse.qss"));
    splash.finish(&w);
    return a.exec();
}
