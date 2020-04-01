#include "mainwindow.h"
#include "util.h"
#include "configmanager.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QDateTime>

ConfigManager *g_config;

#ifdef QT_NO_DEBUG

QFile g_logFile;

static void initLogFile(const QString &fileName)
{
    g_logFile.setFileName(fileName);
    g_logFile.open(QFile::Append | QFile::Text);
}

#endif // QT_NO_DEBUG

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString header;
    
    switch(type){
    case QtDebugMsg:
        header = QObject::tr("Debug: ");
        break;
    case QtInfoMsg:
        header = QObject::tr("Info: ");
        break;
    case QtWarningMsg:
        header = QObject::tr("Warning: ");
        break;
    case QtCriticalMsg:
        header = QObject::tr("Critical: ");
        break;
    case QtFatalMsg:
        header = QObject::tr("Fatal: ");
        break;
    }
    
    QString fileName = QFileInfo(context.file).fileName();
    QString text = header 
            + QString("(%1:%2) ").arg(fileName).arg(context.line)
            + msg;
#ifdef QT_NO_DEBUG
    QTextStream stream(&g_logFile);
    stream << text << QString("[%1]\n").arg(QDateTime::currentDateTime().toString());
    
    if(type == QtFatalMsg) {
        g_logFile.close();
        abort();
    }
#else // QT_NO_DEBUG
    std::cerr << text.toStdString() << std::endl;
#endif // QT_NO_DEBUG
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    // Set default locale.
    if (locale == "zh_CN") {
        QLocale::setDefault(QLocale(QLocale::Chinese, QLocale::China));
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
