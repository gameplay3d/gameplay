#include "ProjectWizardItemDelegate.h"
#include <QPainter>

#define FONT_NAME           QFont("Tahoma", 14, QFont::Bold)
#define FONT_PATH           QFont("Tahoma", 12, QFont::Normal)
#define HEIGHT_PADDING      10


ProjectWizardItemDelegate::ProjectWizardItemDelegate(QObject* parent)
{
}

ProjectWizardItemDelegate::~ProjectWizardItemDelegate()
{
}

void ProjectWizardItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QRect adjusted = option.rect;
     if(option.state & QStyle::State_Selected)
     {
         painter->fillRect(option.rect, option.palette.highlight());
         painter->setPen(option.palette.highlightedText().color());
     }
     else
     {
         painter->setPen(option.palette.text().color());
     }
     // Name
     QString title = index.data(Qt::DisplayRole).toString();
     adjusted = option.rect.adjusted(10, 0, 0, -18);
     painter->setFont(FONT_NAME);
     painter->drawText(adjusted.left(), adjusted.top(), 
                       adjusted.width(), adjusted.height(),
                       Qt::AlignBottom|Qt::AlignLeft, title, &adjusted);
     // Path
     QString description = index.data(Qt::UserRole).toString();
     adjusted = option.rect.adjusted(10, 20, 0, 0);
     painter->setFont(FONT_PATH);
     painter->drawText(adjusted.left(), adjusted.top(), 
                       adjusted.width(), adjusted.height(),
                       Qt::AlignLeft, description, &adjusted);
}

QSize ProjectWizardItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.decorationSize.width(), 40);
}
