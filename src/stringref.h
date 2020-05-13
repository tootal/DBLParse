#pragma once

#include <QtGlobal>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QDataStream>

class StringRef{
public:
    quint32 l, r; // reference to s_data[l..r)
    
private:
    static char *s_data;
    
public:
    static quint32 s_len;
    
    static void init(const QString &fileName);
    
    static void clean();
    
public:
    StringRef(quint32 l_ = 0, quint32 r_ = 0):l(l_), r(r_){}
    
    char &operator[](quint32 x) const;
    
    bool operator<(const StringRef &s) const;
    
    StringRef mid(quint32 pos) const;
    
    StringRef mid(quint32 pos, quint32 len) const;
    
    bool startsWith(const char *str, quint32 from = 0) const;
    
    // if startsWith(strs[i], from) return i else return -1
    int startsWith(const QStringList &strs, quint32 from = 0) const;
    
    qint32 indexOf(const char *str, quint32 from = 0) const;
    
    QString toString() const;
    
    friend QDebug operator<<(QDebug debug, const StringRef &s);
};

QDataStream &operator<<(QDataStream &out, const StringRef &ref);

QDataStream &operator>>(QDataStream &in, StringRef &ref);
