#include "authorstacmodel.h"

AuthorStacModel::AuthorStacModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    
}

int AuthorStacModel::rowCount(const QModelIndex &) const
{
    return 5;
}

int AuthorStacModel::columnCount(const QModelIndex &) const
{
    return 2;
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

QVariant AuthorStacModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case 0:
                return tr("Author Name");
            case 1:
                return tr("Number of Publications");
            }
        }
    } else if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole) {
            return section + 1;
        }
    }
    return QVariant();
}
