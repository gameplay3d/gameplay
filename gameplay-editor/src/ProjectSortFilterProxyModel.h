#pragma once

#include <QSortFilterProxyModel>

/**
 * Defines the sorting filter used for the project.
 */
class ProjectSortFilterProxyModel : public QSortFilterProxyModel
{
public:

    /**
     * Constructor.
     *
     * @param parent The parent object.
     */
    explicit ProjectSortFilterProxyModel(QObject* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectSortFilterProxyModel();

    /**
     * @see QSortFilterProxyModel::lessThan
     */
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const;

    /**
     * @see QSortFilterProxyModel::filterAcceptsRow
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
};
