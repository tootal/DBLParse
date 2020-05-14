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
Util::s_availableLanguages = {
    {"en_US", QT_TR_NOOP("English (United States)")},
    {"zh_CN", QT_TR_NOOP("Chinese (China)")}
};

QStringList Util::s_loadedFiles = {
    "authorStac.dat",
    "yearWord.txt",
    "words.dat"
};

QStringList Util::s_parsedFiles = {
    "authorStac.dat",
    "authors_cliques.txt",
    "yearWord.txt",
    "words.dat"
};

QString Util::formatTime(int ms)
{
    Q_ASSERT(ms >= 0);
    QTime time = QTime::fromMSecsSinceStartOfDay(ms);
    if(time.hour() == 0){
        if(time.minute() == 0)
            return time.toString("s.zzz ")+tr("s");
        return time.toString("m:ss.z");
    }
    return time.toString("H:mm:ss");
}

QString Util::readAround(const QString &fileName, quint32 &pos)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(file.isOpen());
    auto beginPos = pos < BUF_SZ ? 0 : pos - BUF_SZ;
    file.seek(beginPos);
    QString data = file.read(BUF_SZ << 1);
    if (data.isEmpty()) {
        qDebug() << fileName;
        qDebug() << pos;
        qDebug() << beginPos;
    }
    Q_ASSERT(!data.isEmpty());
    pos -= beginPos;
    file.close();
    return data;
}

QString Util::findRecord(const QString &fileName, quint32 pos)
{
    QString data = readAround(fileName, pos);
    int p1 = data.lastIndexOf("key=\"", pos);
    p1 = data.lastIndexOf('<', p1);
    int p2 = data.indexOf(' ', p1);
    auto name = data.midRef(p1 + 1, p2 - p1 - 1);
    p2 = data.indexOf(QString("</%1>").arg(name), p2);
    return data.mid(p1, p2 - p1 + 3 + name.length());
}

QString Util::readFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    return file.readAll();
}

QString Util::getXmlFileName()
{
    return App->config->value("lastOpenFileName");
}

QString Util::formatUrl(const QString &url)
{
    if(url.isEmpty()) 
        return url;
    if(QUrl(url).isRelative())
        return "https://dblp.uni-trier.de/" + url;
    return url;
}

QString Util::getLocale()
{
    QString locale = App->config->value("language");
    if (locale == "System") {
        locale = QLocale::system().name();
    }
    return locale;
}

void Util::clearIndexs()
{
    for (const QString &fileName : s_parsedFiles) {
        QFile(fileName).remove();
    }
}

void Util::initIndexs()
{
    QDir().mkdir("data");
    static QStringList dataFolders{
        "title",
        "author"
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
    for (const QString &fileName : s_parsedFiles) {
        if (QFile(fileName).exists()) {
            return true;
        }
    }
    return false;
}

bool Util::canLoad()
{
    for (const QString &fileName : s_loadedFiles) {
        if (!QFile(fileName).exists()) {
            return false;
        }
    }
    return true;
}

