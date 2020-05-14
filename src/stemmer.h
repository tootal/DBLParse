#pragma once

#include <QByteArrayList>
#include <QHash>

class Stemmer
{
public:
    static const QByteArray noNeedChars;
    static const QByteArrayList commonwords;
    static const QHash<QByteArray, QByteArray> lemmatization;
    static QByteArrayList
    stem(QByteArray content) {
//        content = removeTag(content);
        content = removeChar(content);
        
        QByteArrayList list = content.split(' ');
        QByteArrayList ws;
        for (auto &i : list) {
            if (i.size() <= 1) continue;
            if (i.size() == 2 && !i.isUpper()) continue;
            i = i.toLower();
            if (commonwords.contains(i)) continue;
            if (lemmatization.contains(i))
                i = lemmatization[i];
            ws.append(i);
        }
        return ws;
    }
private:
    static QByteArray
    removeTag(QByteArray content) {
        return content;
    }
    static QByteArray
    removeChar(QByteArray content) {
        for (char c : noNeedChars) {
            content.replace(c, ' ');
        }
        return content;
    }
};
