#include "SceneSortFilterProxyModel.h"
#include <QFileSystemModel>


SceneSortFilterProxyModel::SceneSortFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
}

SceneSortFilterProxyModel::~SceneSortFilterProxyModel()
{
}

bool SceneSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (sourceModel()->hasChildren(index))
        return true;
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
