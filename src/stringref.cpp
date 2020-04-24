#include "stringref.h"

// public:
quint32 StringRef::s_len;

// private:
char *StringRef::s_data;

void StringRef::init(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    s_data = new char[static_cast<quint64>(file.size())];
    s_len = static_cast<quint32>(file.read(s_data, file.size()));
    file.close();
}

void StringRef::clean()
{
    delete s_data;
}

char &StringRef::operator[](quint32 x) const
{
    Q_ASSERT(0 <= x && x < r - l);
    return s_data[l + x];
}

bool StringRef::operator<(const StringRef &s) const
{
    quint32 len = r - l;
    if(s.r - s.l < len) len = s.r - s.l;
    for(quint32 i = 0; i < len; ++i){
        if(s_data[l + i] != s_data[s.l + i]){
            return s_data[l + i] < s_data[s.l + i];
        }
    }
    return len != s.r - s.l;
}

StringRef StringRef::mid(quint32 pos) const
{
    Q_ASSERT(0 <= pos && pos < r - l);
    return {l + pos, r};
}

StringRef StringRef::mid(quint32 pos, quint32 len) const
{
    Q_ASSERT(0 <= pos && pos < r - l);
    Q_ASSERT(pos + len <= r - l);
    return {l + pos, l + pos + len};
}

bool StringRef::startsWith(const char *str, quint32 from) const
{
    quint32 x = l + from;
    while(*str != 0){
        if(*str != s_data[x]) return false;
        ++str;
        ++x;
        if(x >= r) return false;
    }
    return true;
}

int StringRef::startsWith(const QStringList &strs, quint32 from) const
{
    for(int i = 0; i < strs.length(); ++i){
        if(startsWith(strs[i].toUtf8(), from)){
            return i;
        }
    }
    return -1;
}

qint32 StringRef::indexOf(const char *str, quint32 from) const
{
    for(quint32 t = l + from; t < r; ++t){
        bool flag = true;
        for(const char *x = str; *x != 0; ++x){
            if(*x != s_data[t + x - str]){
                flag = false;
                break;
            }
        }
        if(flag){
            return static_cast<qint32>(t - l);
        }
    }
    return -1;
}

QString StringRef::toString() const
{
    return QByteArray::fromRawData(s_data + l, static_cast<int>(r - l));
}


QDebug operator<<(QDebug debug, const StringRef &s){
    debug << s.toString();
    return debug;
}

