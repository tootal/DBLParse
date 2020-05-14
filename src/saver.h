#pragma once

#include <QtGlobal>
#include <QVector>

#include "parser.h"

class Saver
{
public:
    // Each file cache: 16KB
    // All memory used: 16MB
    static constexpr int DEFAULT_SIZE = 1 << 11;
    Saver(int maxCacheSize = DEFAULT_SIZE)
        :maxSize(maxCacheSize) {
        
    }
    ~Saver() {
        
    }
    bool save(quint16 flag, HashIndex index) {
        buffer[flag].append(index);
        if (buffer[flag].size() > maxSize) 
            return save(flag);
        return true;
    }
    void saveAll() {
        for (quint16 flag = 0; flag < 1024; flag++)
            save(flag);
    }
private:
    QVector<HashIndex> buffer[1024];
    int maxSize;
    bool save(quint16 flag) {
        QFile file(QString("data/title/%1").arg(flag));
        if (!file.open(QFile::Append)) return false;
        QDataStream s(&file);
        for (auto i : buffer[flag]) {
//            file.write((char*)&i.hash, 4);
            s << i.hash << i.pos;
        }
        file.close();
        buffer[flag].clear();
        return true;
    }
};
