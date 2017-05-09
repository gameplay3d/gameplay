#ifndef PROJECTWIZARDITEM_H
#define PROJECTWIZARDITEM_H

#include <QStyledItemDelegate>


/**
 * Defines the visual style for list items in the project wizard.
 */
class ProjectWizardItemDelegate : public QStyledItemDelegate
{
public:
    /**
     * Constructor.
     *
     * @param parent The parent object.
     */
    explicit ProjectWizardItemDelegate(QObject* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectWizardItemDelegate();

    /**
     * @see QStyledItemDelegate::paint
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    /**
     * @see QStyledItemDelegate::sizeHint
     */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif
