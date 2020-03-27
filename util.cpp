#include "util.h"

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QUrl>

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
    QRegularExpression re(R"(<\/(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data)>)");
    auto m = re.match(data, static_cast<int>(pos));
    if(!m.hasMatch()){
        qDebug() << pos;
        qDebug() << data;
    }
    Q_ASSERT(m.hasMatch());
    QString name = m.captured(1);
    int endPos = m.capturedEnd(1) + 1;
    data.remove(endPos, data.size() - endPos);
    int beginPos = data.lastIndexOf("<"+name);
    Q_ASSERT(beginPos != -1);
    return data.remove(0, beginPos);
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
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    return settings.value("lastOpenFileName").toString();
}

QString Util::formatUrl(const QString &url)
{
    if(url.isEmpty()) return url;
    if(QUrl(url).isRelative()) return "https://dblp.uni-trier.de/" + url;
    else return url;
}

void Util::htmlRender(QString &html, const QString &flag, const QString &holder, QString templ)
{
    if(flag.isNull() || flag.isEmpty()) return ;
    if(templ.contains("%1")) templ = templ.arg(flag);
    html.replace(QString("<!-- %1_holder -->").arg(holder), templ);
}

void Util::htmlRender(QString &html, const QStringList &flags, const QString &holder, QString templ)
{
    if(flags.isEmpty()) return ;
    QString templs;
    foreach(auto flag, flags){
        auto t = templ;
        if(t.contains("%1")) t = t.arg(flag);
        templs.append(t);
    }
    html.replace(QString("<!-- %1_holder -->").arg(holder), templs);
}
