#include "dblp.h"

#include <QFile>
#include <QDebug>

const QStringList Dblp::defaultRecordNames = { "article",
                                               "inproceedings",
                                               "proceedings",
                                               "book",
                                               "incollection",
                                               "phdthesis",
                                               "mastersthesis",
                                               "www",
                                               "person",
                                               "data" };

QPair<qint64, qint64> Dblp::findRecordRange(const QString &filePath, qint64 pos)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "fail to open file" << filePath << "to read";
        return qMakePair(0, 0);
    }
    QString recordName;
    int offset = -1;
    file.seek(pos);
    while(!file.atEnd()){
        QString s = file.readLine();
        foreach(recordName, defaultRecordNames){
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
