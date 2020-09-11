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
        header = "Debug:";
        break;
    case QtInfoMsg:
        header = "Info:";
        break;
    case QtWarningMsg:
        header = "Warning:";
        break;
    case QtCriticalMsg:
        header = "Critical:";
        break;
    case QtFatalMsg:
        header = "Fatal:";
        break;
    }
    QString text;
    if (context.file == nullptr) {
        text = QString("%1 %4")
                .arg(header, 
                     msg);
    } else {
        QString fileName = QFileInfo(context.file).fileName();
        text = QString("%1 (%2:%3) %4")
                .arg(header, 
                     fileName, 
                     QString::number(context.line), 
                     msg);
    }
#ifdef QT_NO_DEBUG
    QTextStream stream(&logFile);
    bool logDebug = true;
    if (logDebug && type == QtDebugMsg) {
        std::cerr << text.toStdString() << std::endl;
    } else {
        stream << QString("[%1]%2\n").arg(
            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"), 
            text);
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
    Application a(argc, argv);
    QCoreApplication::setOrganizationName("SCUT_CS");
    QCoreApplication::setApplicationName("DBLParse");
    
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
