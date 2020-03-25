#include "util.h"

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>

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
    if(pos < BUF_SZ){
        file.seek(0);
    }else{
        file.seek(pos - BUF_SZ);
        pos = BUF_SZ;
    }
    QString data = file.read(BUF_SZ << 1);
    file.close();
    return data;
}

QString Util::findRecord(const QString &fileName, quint32 pos)
{
    QString data = readAround(fileName, pos);
    QRegularExpression re(R"(<\/(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data)>)");
    auto m = re.match(data, static_cast<int>(pos));
    if(!m.hasMatch()) qDebug() << pos;
    Q_ASSERT(m.hasMatch());
    QString name = m.captured(1);
    int endPos = m.capturedEnd(1) + 1;
    data.remove(endPos, data.size() - endPos);
    int beginPos = data.lastIndexOf("<"+name);
    return data.remove(0, beginPos);
}
