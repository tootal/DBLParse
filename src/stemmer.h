#pragma once

#include <QByteArrayList>

class Stemmer
{
public:
    static const QByteArray noNeedChars;
    static const QByteArrayList commonwords;
    static QByteArrayList
    stem(QByteArray content) {
//        content = removeTag(content);
        content = removeChar(content);
        
        QByteArrayList list = content.split(' ');
        QByteArrayList ws;
        for (auto &i : list) {
            if (i.size() <= 2) continue;
            i = i.toLower();
            if (commonwords.contains(i)) continue;
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

const QByteArray Stemmer::noNeedChars = ":,.?()";

const QByteArrayList Stemmer::commonwords = {
    "are", "all", "any", "been", "both",
    "each", "either", "one", "two", "three",
    "four", "five", "six", "seven", "eight",
    "nine", "ten", "none", "little", "few",
    "many", "much", "other", "another", "some",
    "every", "nobody", "anybody", "somebody",
    "everybody", "when", "under", "uuml",
    "first", "second", "third", "forth", "fifth",
    "sixth", "seventh", "above", "over", "below",
    "under", "beside", "behind", "the", "after",
    "from", "since", "for", "which", "next",
    "where", "how", "who", "there", "was", "der",
    "were", "did", "done", "this", "that",
    "last", "brfore", "because", "against",
    "except", "beyond", "along", "among", "but",
    "towards", "you", "multi", "time", "von",
    "your", "his", "her", "she", "its", "they",
    "them", "and", "has", "have", "had",
    "would", "then", "too", "our", "off",
    "into", "weel", "can", "being", "zur",
    "been", "having", "even", "these", "those",
    "ours", "with", "use", "using", "used",
    "the", "based", "problem", "problems", "systems",
    "methods", "ways", "ideas", "learning", "information",
    "works", "solve", "solving", "solved", "old", "new",
    "analysis", "data", "big", "small", "large",
    "their", "between", "method"
};
