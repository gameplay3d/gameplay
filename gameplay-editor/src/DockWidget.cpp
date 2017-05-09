#include "DockWidget.h"
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QVariant>
#include <QHBoxLayout>
#include <QPoint>
#include <QStyleOption>
#include <QPainter>
#include <QCloseEvent>

DockWidget::DockWidget(QWidget* parent) : QDockWidget(parent),
    _emptyTitleBar(new QWidget(this)),
    _lastTabBar(nullptr)
{	
    QAction* action = toggleViewAction();
    action->setData(QVariant(reinterpret_cast<quintptr>(this)));
    setTitleBarWidget(_emptyTitleBar);
}

DockWidget::~DockWidget()
{
}

void DockWidget::changeTitleBar(bool useDefault)
{
    if (useDefault)
        setTitleBarWidget(0);
	else
        setTitleBarWidget(_emptyTitleBar);

}

void DockWidget::centerTitle(QPoint p)
{
    int centreTitleX = frameGeometry().width() / 2;
    int centreTitleY = style()->pixelMetric(QStyle::PM_TitleBarHeight, 0, this) / 2;
    QPoint centreTitlePos = p - QPoint(centreTitleX, centreTitleY);
    move(centreTitlePos);
}

void DockWidget::closeEvent(QCloseEvent* event)
{
    event->accept();
    this->hide();

}
