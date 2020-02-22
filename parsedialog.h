#ifndef PARSEDIALOG_H
#define PARSEDIALOG_H

#include <QObject>

class ParseDialog : public QObject
{
    Q_OBJECT
public:
    explicit ParseDialog(QObject *parent = nullptr);
    
signals:
    
};

#endif // PARSEDIALOG_H
