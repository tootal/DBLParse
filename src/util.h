#pragma once

#include <QObject>
#include <QDebug>
#include <string>

class Util : public QObject
{
    Q_OBJECT
public:
    static const QVector<QPair<QString, QByteArray>> availableLanguages;
    static const QStringList availableDownloadSources;
    static QByteArray readFile(const QString &fileName);
    static QString getXmlFileName();
    static QString getLocale();
    static QString getLogPath();
    static void clearIndexs();
    static void initIndexs();
    static bool parsed();
    static bool canLoad();
    static void showMarkdown(const QString &content, QWidget *parent);
    static QPixmap svgToPixmap(const QString &fileName, QSize size);
    static void setClipboardText(const QString &text);
    static void openFolder(const QString &filePath);
    static QString getFileReadableSize(const QString &fileName);
    static QString displayDateTime(const QDateTime &dateTime);
    static QString displayCostTime(int costMsecs);
    static void copyText(const QString &text);
};
