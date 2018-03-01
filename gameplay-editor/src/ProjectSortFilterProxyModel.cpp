#include "ProjectSortFilterProxyModel.h"
#include <QFileSystemModel>


ProjectSortFilterProxyModel::ProjectSortFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
}

ProjectSortFilterProxyModel::~ProjectSortFilterProxyModel()
{
}

bool ProjectSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{    
    if (sortColumn() == 0) 
    {
        QFileSystemModel* fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        bool ascending = sortOrder() == Qt::AscendingOrder ? true : false;
        QFileInfo leftFileInfo = fsm->fileInfo(left);
        QFileInfo rightFileInfo = fsm->fileInfo(right);
        // If DotAndDot move in the beginning
        if (sourceModel()->data(left).toString() == "..")
            return ascending;
        if (sourceModel()->data(right).toString() == "..")
            return !ascending;
        // Move directories to top
        if (!leftFileInfo.isDir() && rightFileInfo.isDir())
            return !ascending;
        if (leftFileInfo.isDir() && !rightFileInfo.isDir())
            return ascending;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

bool ProjectSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow,0, sourceParent);
    if (sourceModel()->hasChildren(index))
        return true;
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
