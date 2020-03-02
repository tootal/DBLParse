#ifndef STRINGREF_H
#define STRINGREF_H

#include <QtGlobal>
#include <QByteArray>
#include <QDebug>

struct StringRef{
    const char *s;
    qint64 n;
    StringRef(const char *s_, qint64 n_):s(s_), n(n_){}
    char operator[](qint64 x) const {
        Q_ASSERT(x >= 0 && x < n);
        return s[x];
    }
    StringRef mid(qint64 pos, qint64 len = -1) const {
        Q_ASSERT(pos >= 0 && pos < n);
        Q_ASSERT(pos + len <= n);
        if(len == -1){
            return StringRef(s + pos, n - pos);
        }else{
            return StringRef(s + pos, len);
        }
    }
    bool startsWith(const char *str, qint64 from = 0) const {
        const char *t = s + from;
        while(*str != 0){
            if(*str != *t) return false;
            ++str;
            ++t;
        }
        return true;
    }
    qint64 indexOf(const char *str, qint64 from = 0) const {
        const char *t = s + from;
        for(int i = 0; t[i] != 0; ++i){
            bool flag = true;
            for(const char *x = str; *x != 0; ++x){
                if(*x != t[i + x - str]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                return from + i;
            }
        }
        return -1;
    }
    friend QDebug operator<<(QDebug debug, const StringRef &str){
        debug << QByteArray::fromRawData(str.s, static_cast<int>(str.n));
        return debug;
    }
};



#endif // STRINGREF_H
