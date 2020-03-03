#ifndef PARSER_H
#define PARSER_H

#include <QThread>
#include <QMap>
#include <QTime>
#include <QXmlStreamReader>
#include <QDebug>

#include "util.h"

class QFile;

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
    static char *s_data;
    
public:
    struct StringRef{
        quint32 l, r; // reference to s_data[l..r)
        StringRef(){}
        StringRef(quint32 l_, quint32 r_):l(l_), r(r_){}
        char &operator[](quint32 x) const;
        StringRef mid(quint32 pos) const;
        StringRef mid(quint32 pos, quint32 len) const;
        bool startsWith(const char *str, quint32 from = 0) const;
        qint32 indexOf(const char *str, quint32 from = 0) const;
    };
    friend QDebug operator<<(QDebug debug, const StringRef &s){
        debug << QByteArray::fromRawData(s_data + s.l, static_cast<int>(s.r - s.l));
        return debug;
    }

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
    static StringRef* s_authorIndex;
    static StringRef* s_titleIndex;
    static int s_authorIndexs;
    static int s_titleIndexs;
    
    void parseRecords();
    void parseContent(QStringRef recordName);
    void save();
    void load();
    
    // treat child element as text, after reading, from equal to the
    // last position of the end element
    static StringRef readElementText(const StringRef &r, quint32 &from);

};

#endif // PARSER_H
