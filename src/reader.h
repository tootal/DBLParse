#pragma once

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>

class Reader
{
public:
    static constexpr int BUFF_SZ = 1 << 22; // 4M
    Reader(const QString &fileName)
        :_begin(0), _end(0), p(0), blocks(0), _year(0), newBlockFlag(false) {
        file.setFileName(fileName);
        if (file.open(QFile::ReadOnly)) {
            buffer = file.read(BUFF_SZ);
            p = buffer.indexOf("<dblp>");
            p += 6;
        }
    }
    ~Reader() {
        file.close();
    }
    bool next() {
        newBlockFlag = false;
        indexOf('<');
        nextChar();
        if (buffer[p] == '/') return false;
        _begin = BUFF_SZ * blocks + p - 1;
        _title.clear();
        _authors.clear();
        _year = 0;
        for (;;) {
            indexOf('<');
            nextChar();
            char c1 = buffer[p];
            nextChar();
            char c2 = buffer[p];
            if (c1 == '/') { // </name>
                indexOf('>');
                nextChar();
                break;
            } else if (c1 == 'a' && c2 == 'u') { // <author>
                indexOf('>');
                nextChar();
                int t = p;
                indexOf("</author>");
                _authors.append(substr(t, p));
                p += 9;
            } else if (c1 == 't' && c2 == 'i') { // <title>
                indexOf('>');
                nextChar();
                int t = p;
                indexOf("</title>");
                _title = substr(t, p);
                p += 8;
            } else if (c1 == 'y' && c2 == 'e') { // <year>
                indexOf('>');
                nextChar();
                int t = p;
                indexOf("</year>");
                _year = substr(t, p).toInt();
                p += 7;
            } else { // other
                int s = 0;
                while (s >= 0) { // exclude nest tag
                    indexOf('<');
                    nextChar();
                    if (buffer[p] == '/') s--;
                    else s++;
                }
            }
        }
        _end = BUFF_SZ * blocks + p;
        return true;
    }
    quint32 begin() const {
        return _begin;
    }
    quint32 end() const {
        return _end;
    }
    quint32 size() const {
        return file.size();
    }
    QByteArrayList authors() const {
        return _authors;
    }
    bool hasTitle() const {
        return !_title.isEmpty();
    }
    QByteArray title() const {
        return _title;
    }
    bool hasYear() const {
        return _year != 0;
    }
    int year() const {
        return _year;
    }
    bool newBlock() const {
        return newBlockFlag;
    }
    
private:
    QFile file;
    QByteArray buffer;
    QByteArray lastBuffer;
    quint32 _begin;
    quint32 _end;
    int p; // current position
    int blocks;
    QByteArrayList _authors;
    QByteArray _title;
    int _year;
    bool newBlockFlag;
    
    
    void indexOf(char c) {
        for (;;) {
            while (p < BUFF_SZ && buffer.at(p) != c) ++p;
            if (p == BUFF_SZ) nextBlock();
            else return ;
        }
    }
    
    template <typename T>
    void indexOf(const T &c) {
        int t = buffer.indexOf(c, p);
        if (t == -1) {
            nextBlock();
            p = buffer.indexOf(c);
        } else {
            p = t;
        }
    }
    void nextChar() {
        if (p == BUFF_SZ - 1) nextBlock();
        else p++;
    }
    void nextBlock() {
        lastBuffer = buffer;
        blocks++;
        buffer = file.read(BUFF_SZ);
        p = 0;
        newBlockFlag = true;
    }
    QByteArray substr(int l, int r) {
        if (r < l) return lastBuffer.right(BUFF_SZ - l) + buffer.left(r);
        else return buffer.mid(l, r - l);
    }
};
