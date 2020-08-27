#include "authorstacmodel.h"

AuthorStacModel::AuthorStacModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    
}

int AuthorStacModel::rowCount(const QModelIndex &) const
{
    return 2;
}

int AuthorStacModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant AuthorStacModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return QString("Row %1, Column %2")
                .arg(index.row() + 1)
                .arg(index.column() + 1);
    }
    return QVariant();
}
