#pragma once

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>

class Reader
{
public:
    static constexpr int BUFF_SZ = 1 << 22; // 4M
    QFile file;
    QByteArray buffer;
    QByteArray lastBuffer;
    quint32 begin;
    quint32 end;
    int p; // current position
    int blocks;
    QString error;
    QByteArrayList authors;
    QByteArray title;
    int year;
    Reader(const QString &fileName)
        :begin(0), end(0), p(0), blocks(0), year(0) {
        file.setFileName(fileName);
        if (file.open(QFile::ReadOnly)) {
            buffer = file.read(BUFF_SZ);
            p = buffer.indexOf("<dblp>");
            if (p == -1) error = QObject::tr("File is not a dblp file.");
            else p += 6;
        } else {
            error = QObject::tr("File open failed: %1.").arg(fileName);
        }
    }
    bool next() {
        if (!error.isEmpty()) return false;
        indexOf('<');
        nextChar();
        if (buffer[p] == '/') return false;
        begin = BUFF_SZ * blocks + p - 1;
        title.clear();
        authors.clear();
        year = 0;
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
                authors.append(substr(t, p));
                p += 9;
            } else if (c1 == 't' && c2 == 'i') { // <title>
                indexOf('>');
                nextChar();
                int t = p;
                indexOf("</title>");
                title = substr(t, p);
                p += 8;
            } else if (c1 == 'y' && c2 == 'e') { // <year>
                indexOf('>');
                nextChar();
                int t = p;
                indexOf("</year>");
                year = substr(t, p).toInt();
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
        end = BUFF_SZ * blocks + p;
        return true;
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
        if (p == -1) error = QObject::tr("File is incorrect.");
    }
    void nextChar() {
        if (p == BUFF_SZ - 1) nextBlock();
        else p++;
    }
    void nextBlock() {
        lastBuffer = buffer;
        blocks++;
        buffer = file.read(BUFF_SZ);
        if (buffer.isEmpty()) error = QObject::tr("End of file exception.");
        p = 0;
    }
    QByteArray substr(int l, int r) {
        if (r < l) return lastBuffer.right(BUFF_SZ - l) + buffer.left(r);
        else return buffer.mid(l, r - l);
    }
};
