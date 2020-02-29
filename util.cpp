#include "util.h"

#include <QTime>
#include <QFile>

QString Util::formatTime(QTime time)
{
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

QPair<qint64, qint64> Util::findRecordRange(const QString &filePath, qint64 pos)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    Q_ASSERT(file.isOpen());
    QString recordName;
    int offset = -1;
    file.seek(pos);
    QStringList recordNames = { "article", "inproceedings", "proceedings", 
                                "book", "incollection", "phdthesis", 
                                "mastersthesis", "www", "person", "data" };
    while(!file.atEnd()){
        QString s = file.readLine();
        foreach(recordName, recordNames){
            offset = s.indexOf(QString("</%1>").arg(recordName));
            if(offset != -1) break;
        }
        if(offset != -1)break;
    }
    const int bufferSize = 2000;
    qint64 rightPos = file.pos();
    qint64 leftPos = 0;
    if(rightPos > bufferSize) leftPos = rightPos - bufferSize;
    Q_ASSERT(leftPos < rightPos);
    file.seek(leftPos);
    QString str = file.read(rightPos - leftPos + 5);
    QString endEleStr = QString("</%1>").arg(recordName);
    int endPos = str.lastIndexOf(endEleStr);
    Q_ASSERT(endPos != -1);
    QStringRef res = str.leftRef(endPos + endEleStr.size());
    QString beginEleStr = QString("<%1").arg(recordName);
    // ATTENTION EXAMPLE: <article mdate="2019-10-25"
    int beginPos = res.lastIndexOf(beginEleStr);
    Q_ASSERT(beginPos != -1);
    Q_ASSERT(beginPos < endPos);
//    res = res.right(res.size() - beginPos);
//    qDebug()<<res;
    return qMakePair(leftPos+beginPos, leftPos+endPos+endEleStr.size());
}
