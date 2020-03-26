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
    void run() override;
    void parse();
    QString fileName() const;
    void setFileName(const QString &fileName);
    int costMsecs();
    static void clearIndex();
signals:
    void stateChanged(const QString &state);
    void done();
    
private:
    static char *s_data;
    
public:
    struct StringRef{
        quint32 l, r; // reference to s_data[l..r)
        StringRef(){}
        StringRef(quint32 l_, quint32 r_):l(l_), r(r_){}
        char &operator[](quint32 x) const;
        bool operator<(const StringRef &s) const;
        StringRef mid(quint32 pos) const;
        StringRef mid(quint32 pos, quint32 len) const;
        bool startsWith(const char *str, quint32 from = 0) const;
        // if startsWith(strs[i], from) return i else return -1
        int startsWith(const QStringList &strs, quint32 from = 0) const;
        qint32 indexOf(const char *str, quint32 from = 0) const;
        QString toString() const;
    };
    friend QDebug operator<<(QDebug debug, const StringRef &s){
        debug << QByteArray::fromRawData(s_data + s.l, static_cast<int>(s.r - s.l));
        return debug;
    }

private:
    QString m_fileName;
    int m_costMsecs;
    QTime m_timing;
    
    // treat child element as text, after reading, from equal to the
    // last position of the end element
    static StringRef readElementText(const StringRef &r, quint32 &from);
    
    // read element attribute, if key exists return its value of StringRef
    // else return empty StringRef
    static StringRef readElementAttr(const StringRef &r, quint32 from, const char *key);

};

#endif // PARSER_H
