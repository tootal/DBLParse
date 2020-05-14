#pragma once

#include <QPair>
#include <QByteArray>

class Hash
{
public:
    template <typename T, T mod, T seed>
    static T
    hash(const QByteArray &s) {
        T ans{};
        for (int i = 0; i < s.size(); i++) {
            ans = ans * seed + s.at(i);
        }
        return ans & mod;
    }
    static quint16
    hash1(const QByteArray &s) {
        return hash<quint16, 0x3ff, 131>(s);
    }
    static quint32
    hash2(const QByteArray &s) {
        return hash<quint32, 0xffffffff, 13131>(s);
    }
private:
    Hash();
};
