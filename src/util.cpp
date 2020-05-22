#include "util.h"

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QUrl>
#include <QDir>
#include <QDirIterator>

#include "configmanager.h"
#include "application.h"

const QVector<QPair<QString, QByteArray>> 
Util::availableLanguages = {
    {"en_US", QT_TR_NOOP("English (United States)")},
    {"zh_CN", QT_TR_NOOP("Chinese (China)")}
};

QString Util::readFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    auto s = file.readAll();
    file.close();
    return s;
}

QString Util::getXmlFileName()
{
    return App->config->value("lastOpenFileName").toString();
}

QString Util::getLocale()
{
    QString locale = App->config->value("language").toString();
    if (locale == "System") {
        locale = QLocale::system().name();
    }
    return locale;
}

void Util::clearIndexs()
{
    QDir("data").removeRecursively();
}

void Util::initIndexs()
{
    QDir().mkdir("data");
    static QStringList dataFolders{
        "title",
        "author",
        "word"
    };
    for (auto f : dataFolders) {
        QString path = QString("data/%1").arg(f);
        if (QDir().exists(path)) {
            QDirIterator i(path, QDir::Files);
            while (i.hasNext()) QFile(i.next()).remove();
        } else {
            QDir().mkdir(path);
        }
    }
}

bool Util::parsed()
{
    return QDir("data").exists();
}

