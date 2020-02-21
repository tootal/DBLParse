#ifndef PARSER_H
#define PARSER_H

#include <QObject>

class QXmlStreamReader;

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QXmlStreamReader *r);
    QString documentVersion() const;
    QString documentEncoding() const;
    QString dtdName() const;
    QString dtdSystemId() const;
    
private:
    QXmlStreamReader *reader;
    QString documentVersion_;
    QString documentEncoding_;
    QString dtdName_;
    QString dtdSystemId_;
    
    void setDocumentVersion(QString s);
    void setDocumentEncoding(QString s);
    void setDtdName(QString s);
    void setDtdSystemId(QString s);
    void parseRecords();
};

#endif // PARSER_H
