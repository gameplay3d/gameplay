#pragma once

#include <QDockWidget>


class DockWidgetManager;
class QTabBar;
class QPoint;

/**
 * Custom dock widget for detachable tab docks
 */
class DockWidget : public QDockWidget
{
    Q_OBJECT
    friend class DockWidgetManager;

public:

    /**
     * Constructor
     *
     * @param parent The parent widget.
     */
    explicit DockWidget(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~DockWidget();

protected:

    /**
     * Used to change the title bar of the dock widget to either the default one or the empty one.
     */
    void changeTitleBar(bool useDefault);

    /**
     * Used to center the title bar on the mouse before synthesizing a mouse event.
     */
    void centerTitle(QPoint p);

    /**
     * Used to hide the dock instead of closing it.
     */
    void closeEvent(QCloseEvent* event);

    QWidget* _emptyTitleBar;
    QTabBar* _lastTabBar;

};
