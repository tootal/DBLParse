#ifndef PARSER_H
#define PARSER_H

#include <QThread>
#include <QMap>

class QXmlStreamReader;

class Parser : public QThread
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    void setReader(QXmlStreamReader *r);
    void run() override;
    QString documentVersion() const;
    QString documentEncoding() const;
    QString dtdName() const;
    QString dtdSystemId() const;
    int count() const;
    int count(QString recordName) const;
    QStringList recordNames() const;
    void abortParse();
    
signals:
    void done(int recordCount);
    void countChanged(double ratio);
    
private:
    QXmlStreamReader *reader;
    QString documentVersion_;
    QString documentEncoding_;
    QString dtdName_;
    QString dtdSystemId_;
    int count_;
    QMap<QString,int> recordCount_;
    bool abortFlag;
    
    void setDocumentVersion(QString s);
    void setDocumentEncoding(QString s);
    void setDtdName(QString s);
    void setDtdSystemId(QString s);
    void parseRecords();
};

#endif // PARSER_H
