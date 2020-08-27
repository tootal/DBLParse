#include "authorstacmodel.h"

#include "parser.h"

AuthorStacModel::AuthorStacModel(const QVector<AuthorStac> &authorStacs, 
                                 QObject *parent)
    : QAbstractTableModel(parent),
      authorStacs(authorStacs)
{
    
}

int AuthorStacModel::rowCount(const QModelIndex &) const
{
    return qMin(100, authorStacs.size());
}

int AuthorStacModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant AuthorStacModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return authorStacs.at(index.row()).author;
        case 1:
            return authorStacs.at(index.row()).stac;
        }
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
