#ifndef RECORDPARSER_H
#define RECORDPARSER_H

#include <QObject>
#include <QDate>

class QXmlStreamReader;

class RecordParser : public QObject
{
    Q_OBJECT
public:
    RecordParser(QXmlStreamReader *r);
    QString name() const;
    QString key() const;
    QDate mdate() const;
    QStringList authors() const;
    void setAuthorIndex(QMultiHash<QString, qint64> *index);
    void parse();
    
private:
    QXmlStreamReader *reader;
    QString name_;
    QString key_;
    QDate mdate_;
    QStringList authors_;
    QMultiHash<QString,qint64> *authorIndex_;
    
    void parseContent();
};

#endif // RECORDPARSER_H
