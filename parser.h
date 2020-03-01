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
    void setAction(const QString &action);
    void run() override;
    void parse();
    QString fileName() const;
    void setFileName(const QString &fileName);
    int costMsecs() const;
    int count() const;
    int authorCount() const;
    void clear();
    bool parsed() const;
    void abortParser();
    QList<QVariant> indexOfAuthor(const QString &author) const;
    QList<QVariant> indexOfTitle(const QString &title) const;
    const QMap<QString, QVariant>& recordCount() const;
    int maxAuthorLength() const;
    const QMap<QString, QVariant> &authorCharCount() const;
signals:
    void done(Parser *parser);
    void loadDone();
    void countChanged(double ratio);
    
private:
    QXmlStreamReader reader;
    QString m_fileName;
    int m_costMsecs;
    int m_count;
    int m_authorCount;
    QTime m_timing;
    bool m_abortFlag;
    bool m_parsed;
    int m_maxAuthorLength;
    QMap<QString, QVariant> m_authorCharCount;
    QString m_action;
    QMap<QString, QVariant> m_recordCount;
    QMap<QString, QVariant> m_authorIndex;
    QMap<QString, QVariant> m_titleIndex;
    
    void parseRecords();
    void parseContent(QStringRef recordName);
    void save();
    void load();
};

#endif // PARSER_H
