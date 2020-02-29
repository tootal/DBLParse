#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>

class Parser : public QObject
{
    Q_OBJECT
public:
    Parser();
    void parse(const QString &fileName);
    QString status() const;
    QString authorIndexFileName() const;
    void setAuthorIndexFileName(const QString &fileName);
    QString titleIndexFileName() const;
    void setTitleIndexFileName(const QString &fileName);
    int maxAuthorNameLength() const;
    QMap<QChar, int> authorNameCharCount() const;
    QTime costTime() const;
signals:
    void posChanged(double ratio);
    void parseDone();
    
private:
    QString m_fileName;
    QString m_status;
    QString m_authorIndexFileName;
    QString m_titleIndexFileName;
    int m_maxAuthorNameLength;
    QMap<QChar, int> m_authorNameCharCount;
    int m_costMsecs;
    
    static const int c_maxIndexBufferSize = 1<<20; // 1048576
};

#endif // PARSER_H
