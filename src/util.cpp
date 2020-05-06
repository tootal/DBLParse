#include "util.h"

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QUrl>

#include "configmanager.h"

extern ConfigManager *g_config;

const QVector<QPair<QString, QByteArray>> 
Util::s_availableLanguages = {
    {"en_US", QT_TR_NOOP("English (United States)")},
    {"zh_CN", QT_TR_NOOP("Chinese (China)")}
};

QStringList Util::s_loadedFiles = {
    "author.dat",
    "title.dat",
    "authorStac.dat",
    "yearWord.txt",
    "words.dat"
};

QStringList Util::s_parsedFiles = {
    "author.dat",
    "title.dat",
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
    if(data.isEmpty()){
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
//    qDebug() << data;
    QRegularExpression re(R"(<\/(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data)>)");
    auto m = re.match(data, static_cast<int>(pos));
    if(!m.hasMatch()){
//        qDebug() << pos;
//        qDebug() << data;
    }
    Q_ASSERT(m.hasMatch());
    QString name = m.captured(1);
    int endPos = m.capturedEnd(1) + 1;
    data.remove(endPos, data.size() - endPos);
    int beginPos = data.lastIndexOf("<" + name + " ");
    Q_ASSERT(beginPos != -1);
    auto ret = data.remove(0, beginPos);
//    qDebug() << ret;
    return ret;
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
    return g_config->value("lastOpenFileName");
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
    QString locale = g_config->value("language");
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

