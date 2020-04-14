#include "util.h"
#include "configmanager.h"

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QUrl>

extern ConfigManager *g_config;

QVector<QPair<QString, QString>> Util::s_availableLanguages;

QString Util::formatTime(int ms)
{
    Q_ASSERT(ms >= 0);
    QTime time = QTime::fromMSecsSinceStartOfDay(ms);
    if(time.hour() == 0){
        if(time.minute() == 0){
            return time.toString("s.zzz ")+tr("s");
        }else{
            return time.toString("m:ss.z");
        }
    }else{
        return time.toString("H:mm:ss");
    }
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
    if(url.isEmpty()) return url;
    if(QUrl(url).isRelative()) return "https://dblp.uni-trier.de/" + url;
    else return url;
}

void Util::initAvailableLanguage()
{
    if (!s_availableLanguages.isEmpty()) {
        return;
    }

    s_availableLanguages.append({"en_US", tr("English (United States)")});
    s_availableLanguages.append({"zh_CN", tr("Chinese (China)")});
}

const QVector<QPair<QString, QString> > Util::availableLanguages()
{
    if(s_availableLanguages.isEmpty()) {
        initAvailableLanguage();
    }
    
    return s_availableLanguages;
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
    for (const QString &fileName : QStringList{
         "author.dat",
         "title.dat",
         "authorStac.dat",
         "authors.txt",
         "authors_relation.txt",
         "authors.edges",
         "cliques_count.txt"
    }) {
        QFile(fileName).remove();
    }
}

