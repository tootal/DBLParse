#include "record.h"

#include <QDebug>
#include <QRegularExpression>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>

#include "util.h"

Record::Record()
{
    
}

Record::Record(quint32 pos)
{
    get(pos);
}

void Record::get(quint32 pos)
{
    QByteArray s;
    // read content
    {
        QFile file(Util::getXmlFileName());
        file.open(QFile::ReadOnly);
        file.seek(pos);
        s = file.read(BUF_SZ);
        file.close();
    }
    int p = 0; // position
    while (s[p] != ' ') ++p;
    attr["name"] = QStringList{s.mid(1, p - 1)};
    int t = p + 1;
    while (s[p] != '>') ++p;
    // parse attributes
    {
        auto as = s.mid(t, p - t).split(' ');
        for (auto a : as) {
            auto tmp = a.split('=');
            auto &key = tmp[0];
            auto value = tmp[1];
            // remove quotes
            value = value.mid(1, value.length() - 2);
            attr[key] = QStringList{value};
        }
    }
    // parse tags
    for (;;) {
        while (s[p] != '<') ++p;
        t = p + 1;
        if (s[t] == '/') break;
        while (s[p] != '>' && s[p] != ' ') ++p;
        auto name = s.mid(t, p - t);
        // pass tag attributes
        while (s[p] != '>') ++p;
        t = p + 1;
        p = s.indexOf("</" + name, t);
        auto value = s.mid(t, p - t);
        if (attr.contains(name)) {
            attr[name].append(value);
        } else {
            attr[name] = QStringList{value};
        }
        ++p;
    }
}

QJsonObject Record::toJson() const
{
    QJsonObject o;
    for (const auto &i : attr.keys())
        o.insert(i, QJsonArray::fromStringList(attr[i]));
    return o;
}

QJsonObject Record::toJson(const QString &type) const
{
    QJsonObject o;
    if (type == "author") {
        o.insert("title", QJsonArray::fromStringList(attr["title"]));
        o.insert("author", QJsonArray::fromStringList(attr["author"]));
        o.insert("year", QJsonArray::fromStringList(attr["year"]));
    } else if (type == "title" || type == "keywords") {
        o.insert("title", QJsonArray::fromStringList(attr["title"]));
        o.insert("author", QJsonArray::fromStringList(attr["author"]));
        o.insert("mdate", QJsonArray::fromStringList(attr["mdate"]));
    } else {
        o = toJson();
    }
    return o;
    
}
