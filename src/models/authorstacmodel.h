#pragma once

#include <QAbstractTableModel>

struct AuthorStac;

class AuthorStacModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AuthorStacModel(const QVector<AuthorStac> &authorStacs, 
                    QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, 
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, 
                        Qt::Orientation orientation, 
                        int role = Qt::DisplayRole) const override;

private:
    const QVector<AuthorStac> &authorStacs;
};

