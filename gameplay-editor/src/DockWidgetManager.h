#pragma once

#include <QObject>
#include <QList>
#include <QPoint>
#include <QtGlobal>

enum PlatformMouseEvent
{
    PLATFORM_MOUSE_EVENT_PRESS,
    PLATFORM_MOUSE_EVENT_RELEASE,
    PLATFORM_MOUSE_EVENT_MOVE
};

void sendPlatformMouseEvent(PlatformMouseEvent ev, int x = 0, int y = 0);


class QMainWindow;
class QTabBar;	
class DockWidget;

/**
 * Defines a manager of custom detachable docket widgets for a main window.
 */
class DockWidgetManager : public QObject
{
Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param mainWindow The main widget.
     */
    explicit DockWidgetManager(QMainWindow* mainWindow);

    /**
     * Destructor.
     */
    ~DockWidgetManager();

    /**
     * Top level change to the dock
     */
    enum TopLevelChange
    {
        TOP_LEVEL_CHANGE_FLOATING,
        TOP_LEVEL_CHANGE_DOCKED,
        TOP_LEVEL_CHANGE_TABIFIED
    };

    /**
     * Creates a dock widget and regsiters it to be managed.
     *
     * @param parent The parent widget.
     * @return The created docket widget.
     */
    DockWidget* createDock(QWidget* parent = 0);

    /**
     * Registers an existing dock to be managed.
     *
     * @param dock The dock to be registered.
     */
    void registerDock(DockWidget* dock);

    /**
     * Changes the title bar of a detachable dock widget based on the 'change' parameter.
     *
     * @param dock The dock widget being updated.
     * @param change The type of change that is occuring
     */
    void topLevelUpdateDock(DockWidget* dock, TopLevelChange change);

    /**
     * Registers a tab bar for event filtering and signaling.
     *
     * @param tabBar The tabBar being registered.
     */
    void registerTabBar(QTabBar* tabBar);

    /**
     * Event filter for the tab bars that allows for tab detaching, tearing, context menus etc.
     *
     * @param target The target of the event filter.
     * @param ev The event.
     * @return true if event is filtered, false if not.
     */
    bool eventFilter(QObject* target, QEvent* ev);

public slots:

    /**
     * Handles topLevelChanged signal widget.
     *
     * @param topLevel true if changed, false if not
     */
    void onDockTopLevelChanged(bool topLevel);

    /**
     * Handles triggered signals from the widget.
     *
     * @param visibility true if the view is now visible, false if not.
     */
    void onDockViewActionChanged(bool visibility);

    /**
     * Handles tabCloseRequested signals from a tab bar.
     *
     * @param index The index of the tab requested to be closed.
     */
    void onTabCloseRequested(int index);

    /**
     * Handlels the custom signal restoringState from the main window.
     */
    void onRestoringState();

private:

    DockWidget* dockFromTab(const QTabBar* tabBar, const int index);

    QMainWindow* _mainWindow;
    QList<QTabBar*>* _tabBarList;
    QTabBar* _pressedTabBar;
    int _pressedTabIndex;
    QPoint _clickPosition;
};
