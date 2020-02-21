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
    
private:
    QXmlStreamReader *reader;
    QString documentVersion_;
    QString documentEncoding_;
    
    void setDocumentVersion(QString s);
    void setDocumentEncoding(QString s);
};

#endif // PARSER_H
