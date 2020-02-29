#ifndef PARSER_H
#define PARSER_H

#include <QThread>
#include <QMap>
#include <QTime>
#include <QXmlStreamReader>

class Parser : public QThread
{
    Q_OBJECT
public:
    Parser(QObject *parent = nullptr);
    void run() override;
    void setFileName(const QString &fileName);
    QTime costTime() const;
    int count() const;
    void clear();
    bool parsed() const;
    void load();
    void abortParser();
    
signals:
    void done(Parser *parser);
    void countChanged(double ratio);
    
private:
    QXmlStreamReader reader;
    QString m_fileName;
    int m_costMsecs;
    int m_count;
    QTime m_timing;
    bool m_abortFlag;
    bool m_parsed;
    QMap<QString, QVariant> m_recordCount;
    QMap<QString, QVariant> m_authorIndex;
    QMap<QString, QVariant> m_titleIndex;
    
    void parseRecords();
    void parseContent(QStringRef recordName);
    void save();
};

#endif // PARSER_H
