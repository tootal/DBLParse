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

QString Util::findRecord(const QString &fileName, quint32 pos)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    Q_ASSERT(file.isOpen());
    const int bufferSize = 3000;
    QString buffer = file.read(bufferSize);
    QRegularExpression re(R"(<\/(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data)>)");
    auto m = re.match(buffer);
    Q_ASSERT(m.hasMatch());
    QString recordName = m.captured(1);
    int endPos = m.capturedEnd(1);
    buffer = buffer.left(endPos);
    file.seek(pos < bufferSize ? 0 : pos - bufferSize);
    buffer.prepend(file.read(pos - file.pos()));
    int beginPos = buffer.lastIndexOf("<"+recordName);
    buffer.remove(0, beginPos);
    return buffer; 
}
