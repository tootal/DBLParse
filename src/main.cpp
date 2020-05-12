#include "mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QDateTime>
#include <QTranslator>
#include <QLocale>

#include "application.h"
#include "util.h"
#include "configmanager.h"

extern ConfigManager *g_config;
ConfigManager *g_config;

extern MainWindow *g_mainwindow;
MainWindow *g_mainwindow;

#ifdef QT_NO_DEBUG

static QFile s_logFile;

static void initLogFile(const QString &fileName)
{
    s_logFile.setFileName(fileName);
    s_logFile.open(QFile::Append | QFile::Text);
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
    QTextStream stream(&s_logFile);
    
    if (type == QtDebugMsg) {
        std::cerr << text.toStdString() << std::endl;
    } else {
        stream << text << QString("[%1]\n").arg(QDateTime::currentDateTime().toString());
    }
    
    if (type == QtFatalMsg) {
        s_logFile.close();
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
    g_config = &config;
    
    
#ifdef QT_NO_DEBUG
    initLogFile("DBLParse.log");
#endif // QT_NO_DEBUG
    qInstallMessageHandler(logger);
    
    qInfo() << "DBLParse start";
    
    QString locale = Util::getLocale();
    QLocale::setDefault(QLocale(locale));
    QTranslator translator;
    translator.load("DBLParse_" + locale, ":/");
    a.installTranslator(&translator);
    
    MainWindow w;
    w.setTranslator(&translator);
    g_mainwindow = &w;
    w.show();
    return a.exec();
}
