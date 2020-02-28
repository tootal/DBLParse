#ifndef DBLP_H
#define DBLP_H

#include <QPair>
#include <QStringList>

class Dblp
{
public:
    static QPair<qint64, qint64> findRecordRange(const QString &filePath, qint64 pos);
    static const QStringList defaultRecordNames;
private:
    Dblp() {}
};

#endif // DBLP_H
