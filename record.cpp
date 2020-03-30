#include "record.h"
#include "util.h"

#include <QDebug>
#include <QRegularExpression>
#include <QJsonObject>

Record::Record(const QString &s, QObject *parent)
    : QObject(parent)
{
    m_src = s;
    QRegularExpression re(R"(<author.*?>(.+)<\/author>)");
    auto i = re.globalMatch(s);
    QStringList authors;
    while(i.hasNext()){
        QString author = i.next().captured(1);
        authors << author;
    }
    if(!authors.isEmpty()){
        m_attrs["authors"] = authors;
    }
    
    re.setPattern(R"(<editor.*?>(.+)<\/editor>)");
    i = re.globalMatch(s);
    QStringList editors;
    while(i.hasNext()){
        QString editor = i.next().captured(1);
        editors << editor;
    }
    if(!editors.isEmpty()){
        m_attrs["editors"] = editors;
    }
    
    re.setPattern(R"(<cite.*?>(.+)<\/cite>)");
    i = re.globalMatch(s);
    QStringList cites;
    while(i.hasNext()){
        QString cite = i.next().captured(1);
        cites << cite;
    }
    if(!cites.isEmpty()){
        m_attrs["cites"] = cites;
    }
    
    re.setPattern(R"#(mdate\s*=\s*"((?:\d{4})-(?:\d{2})-(?:\d{2}))")#");
    auto m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_attrs["mdate"] = m.captured(1);
    
    re.setPattern(R"(<(article|inproceedings|proceedings|book|incollection|phdthesis|mastersthesis|www|person|data))");
    m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_attrs["name"] = m.captured(1);
    
    re.setPattern(R"#(key="(.*?)")#");
    m = re.match(s);
    Q_ASSERT(m.hasMatch());
    m_attrs["key"] = m.captured(1);
    auto ee = Util::formatUrl(capture("ee"));
    if(!ee.isEmpty()) m_attrs["ee"] = ee;
    auto url = Util::formatUrl(capture("url"));
    if(!url.isEmpty()) m_attrs["url"] = url;
    
    static const QStringList attrsList = {
        "title",
        "year",
        "journal",
        "volume",
        "booktitle",
        "series",
        "pages",
        "note",
        "address",
        "number",
        "month",
        "cdrom",
        "publisher",
        "crossref",
        "isbn",
        "school",
        "chapter",
        "publnr"
    };
    
    foreach(auto attr, attrsList){
        auto v = capture(attr);
        if(!v.isEmpty()){
            m_attrs[attr] = v;
        }
    }
}

QString Record::capture(const QString &tag) const
{
    QRegularExpression re;
    re.setPattern(QString(R"(<%1.*?>(.+)<\/%1>)").arg(tag));
    auto m = re.match(m_src);
    return m.captured(1);
}

QVariant Record::attr(const QString &tag) const
{
    if(m_attrs.contains(tag)){
        return m_attrs[tag];
    }else{
        return QVariant();
    }
}

QJsonObject Record::toJson() const
{
//    qDebug() << m_attrs;
    auto ret = QJsonObject::fromVariantMap(m_attrs);
//    qDebug() << ret;
    return ret;
}
