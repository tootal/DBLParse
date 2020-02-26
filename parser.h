#ifndef PARSER_H
#define PARSER_H

#include "recordparser.h"

#include <QThread>
#include <QMap>
#include <QMultiHash>
#include <QTime>

class QXmlStreamReader;

class Parser : public QThread
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    void clear();
    void setReader(QXmlStreamReader *r);
    void run() override;
    QString documentVersion() const;
    QString documentEncoding() const;
    QString dtdName() const;
    QString dtdSystemId() const;
    int count() const;
    int count(QString recordName) const;
    QStringList authorNames() const;
    int authorCount() const;
    QStringList recordNames() const;
    QMap<QString,int> recordCount();
    void abortParse();
    QTime parseCostTime();
    QList<qint64> getOffsetsByAuthorName(QString authorName);
    
signals:
    void done(Parser *parser);
    void countChanged(double ratio);
    
private:
    QXmlStreamReader *reader;
    QString documentVersion_;
    QString documentEncoding_;
    QString dtdName_;
    QString dtdSystemId_;
    int count_;
    QMap<QString,int> recordCount_;
    QMultiHash<QString,qint64> authorIndex_;
    QTime parseTiming;
    int parseCostMsecs_;
    bool abortFlag;
    
    void setDocumentVersion(QString s);
    void setDocumentEncoding(QString s);
    void setDtdName(QString s);
    void setDtdSystemId(QString s);
    void parseRecords();
};

#endif // PARSER_H
