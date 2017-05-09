#include "DockWidgetManager.h"
#include "DockWidget.h"
#include <QMainWindow>
#include <QTabBar>
#include <QVariant>
#include <QList>
#include <QAction>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QCursor>
#include <QAction>
#include <QMenu>


DockWidgetManager::DockWidgetManager(QMainWindow* mainWindow) : QObject(mainWindow),
    _mainWindow(mainWindow),
    _tabBarList(new QList<QTabBar*>),
    _pressedTabBar(nullptr),
    _pressedTabIndex(-1),
    _clickPosition()
{	
    connect(_mainWindow, SIGNAL(restoringState()), this, SLOT(onRestoringState()));
}


DockWidgetManager::~DockWidgetManager()
{
    delete _tabBarList;
}


DockWidget* DockWidgetManager::createDock(QWidget* parent)
{	
    DockWidget* dock = new DockWidget(parent);
    connect(dock, SIGNAL(topLevelChanged(bool)), this, SLOT(onDockTopLevelChanged(bool)));
    connect(dock->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(onDockViewActionChanged(bool)));
	return dock;
}

void DockWidgetManager::registerDock(DockWidget* dock)
{
    connect(dock, SIGNAL(topLevelChanged(bool)), this, SLOT(onDockTopLevelChanged(bool)));
    connect(dock->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(onDockViewActionChanged(bool)));
}

void DockWidgetManager::onDockTopLevelChanged(bool topLevel)
{
	// Obtain the dock widget that has changed top level.	
    DockWidget* dock = qobject_cast<DockWidget*>(sender());
	
	// If there's another dock widget being tabified or untabified with this one, we need to handle its title bar too.
    DockWidget* secondDock = nullptr;
	TopLevelChange secondDockChange;
	
	// Identify the type of top level change that this dock widget went through.
	TopLevelChange change;				
	QTabBar* tabBar = nullptr;
	
    if (topLevel)
	{
		// This dock became a floating window.
        change = TOP_LEVEL_CHANGE_FLOATING;
		
        // If this dock left a tab bar that had just two pages, we need to restore the title bar of
        // its old neighbour dock as this neighbour dock will now be docked alone.
        if (dock->_lastTabBar)
		{
            if (dock->_lastTabBar->count() == 2)
			{
				// Identify the dock being left behind.
                DockWidget* tempDock = dockFromTab(dock->_lastTabBar, 0);
				
                if (dock == tempDock)
                {
                    secondDock = dockFromTab(dock->_lastTabBar, 1);
                }
				else
                {
					secondDock = tempDock;				
                }
                secondDockChange = TOP_LEVEL_CHANGE_DOCKED;
			}
		}		
	}
	else
	{
		// This dock is plugged onto the main window.		
		// Check if it's alone, or if it's tabified with other docks.
        const QList<QDockWidget*> tabifiedList = _mainWindow->tabifiedDockWidgets(dock);
		
        if (!tabifiedList.isEmpty())
		{
			// It's tabified with others. Identify the tab bar representing this dock.
            QList<QTabBar*> allTabBars = _mainWindow->findChildren<QTabBar*>();
            Q_FOREACH(QTabBar* tempTabBar, allTabBars)
			{
				// Ignore tab bars with less than one page.
				// The docking system only displays tab bars with more than one page.
                if (tempTabBar->count() > 1)
				{
                    // Only process tab bars used by our detachable dock widgets. Dock widgets can
                    // only be tabified with other dock widgets, so we use this to filter the tab bars.
                    if (dockFromTab(tempTabBar, 0))
					{
                        // If there's a valid dock pointer in this tab bar.
						// Find if there's a tab page representing this particular dock that changed top level.
                        quintptr dockHandle = reinterpret_cast<quintptr>(dock);
						
                        for (int index = 0; index < tempTabBar->count(); ++index)
						{
                            // We use the tab data values that Qt internally populates with 'quintptr' dock handles.
                            if (tempTabBar->tabData(index).value<quintptr>() == dockHandle)
							{	
								// We found the tab bar that is representing this tabified dock widget.
                                change = TOP_LEVEL_CHANGE_TABIFIED;
								tabBar = tempTabBar;
                                dock->_lastTabBar = tabBar;
								
                                // See if there's a second dock involved that needs its title bar also removed, as it
                                // doesn't emit a top level change itself. This is needed when a new tab bar is created.
                                if (tabBar->count() == 2)
								{
                                    secondDock = dockFromTab(tabBar, 1 - index);
                                    secondDock->_lastTabBar = tabBar;
                                    secondDockChange = TOP_LEVEL_CHANGE_TABIFIED;
								}
								break;
							}
						}
                    }
				}				
			}
            // There might happen a case where all of our tabified neighbours are hidden.  We then treat this dock as if it's docked alone.
            if (!tabBar)
                change = TOP_LEVEL_CHANGE_DOCKED;
		}
		else 
		{
			// It's just docked alone.
            change = TOP_LEVEL_CHANGE_DOCKED;
		}
	}
	
	// With the necessary data, manage the dock widget, the possible second dock and the possible tab bar involved.
    topLevelUpdateDock(dock, change);
	
    if (secondDock)
        topLevelUpdateDock(secondDock, secondDockChange);
	
    if (tabBar)
        registerTabBar(tabBar);
}


void DockWidgetManager::onDockViewActionChanged(bool visible)
{
    // Obtain the handle to the dock widget that has changed visibility from the user data of its toggle view QAction.
    QAction* action = qobject_cast< QAction* >(sender());
    DockWidget* dock = reinterpret_cast<DockWidget*>(action->data().value<quintptr>());
	
    // Only process docks that have been tabified at some moment in the past.
    if (!dock->_lastTabBar)
        return;
	
    if (visible)
	{
		// This dock widget has reappeared. See if it is tabified right now with other dock widgets.
        QList<QDockWidget*> tabifiedList = _mainWindow->tabifiedDockWidgets(dock);
		
        if (!tabifiedList.isEmpty())
		{			
			// React differently depending on the number of visible tabs.
            switch(dock->_lastTabBar->count())
			{
				case 2:
				{
                    // If its tab bar has only two docks, remove the title bar of its neighbour dock as it was docked
                    // alone before and it won't emit a top level change.  Obtain whatever dock is in tab zero.
                    DockWidget* firstDock = dockFromTab(dock->_lastTabBar, 0);
					
					// Hide the title bar of the neighbour dock.
                    if (firstDock == dock)
                    {
                        topLevelUpdateDock(dockFromTab(dock->_lastTabBar, 1), TOP_LEVEL_CHANGE_TABIFIED);
                    }
					else
                    {
                        topLevelUpdateDock(firstDock, TOP_LEVEL_CHANGE_TABIFIED);
                    }
					break;
				}		
				case 1:
                {
                    // If its tab bar has only one tab, which is what happens when all the neighbours
                    // are hidden, then we consider this dock as docked alone.
                    topLevelUpdateDock(dock, TOP_LEVEL_CHANGE_DOCKED);
					
					break;
                }
				default:
                    // The following is needed in case this dock has been hidden before it was left
                    // alone by an old tab bar neighbour dock that was floated.
                    topLevelUpdateDock(dock, TOP_LEVEL_CHANGE_TABIFIED);
					break;
			}
		}
		else
		{
            // This dock was hidden while tabified, but now its tab bar isn't visible while this dock is reappearing.
            // Restore its default title bar as it will be considered as docked alone for now.
            topLevelUpdateDock(dock, TOP_LEVEL_CHANGE_DOCKED);
		}		
	}
	else
	{
		// This dock widget is being hidden.
		// See if its latest tab bar has only two dock widgets, and restore the default title bar of the dock being left there alone.
        if (dock->_lastTabBar->count() == 2)
		{
            DockWidget* firstDock = dockFromTab( dock->_lastTabBar, 0);
			
			// Show the title bar of the neighbour dock.
			
			if ( firstDock == dock )
                topLevelUpdateDock(dockFromTab(dock->_lastTabBar, 1), TOP_LEVEL_CHANGE_DOCKED);
			else
                topLevelUpdateDock(firstDock, TOP_LEVEL_CHANGE_DOCKED);
		}
	}
}

DockWidget* DockWidgetManager::dockFromTab(const QTabBar* tabBar, const int index)
{
    return reinterpret_cast<DockWidget*>(tabBar->tabData(index).value<quintptr>());
}

void DockWidgetManager::topLevelUpdateDock(DockWidget* dock, TopLevelChange change)
{
    bool useDefault = true;
    switch(change)
	{
        case TOP_LEVEL_CHANGE_FLOATING:
			// This dock is a floating window. Restore the default title bar.
            useDefault = true;
			break;
			
        case TOP_LEVEL_CHANGE_DOCKED:
			// This dock is plugged onto the main window and alone. Restore the default title bar.
            useDefault = true;
			break;
			
        case TOP_LEVEL_CHANGE_TABIFIED:
			
			// This dock is tabified with others. Remove the default title bar.
            useDefault = false;
			break;			
	}
    dock->changeTitleBar(useDefault);
}


void DockWidgetManager::registerTabBar(QTabBar* tabBar)
{
	// Check if this tab bar is already registered with this manager object.
    if (!_tabBarList->contains(tabBar))
	{
        // This is a newly formed tab bar, Add close buttons to all tabs. This is optional.
        tabBar->setTabsClosable(true);
        // CRASHES with error "cyclic repaint detected" on Windows XP when you drag the tabs.
        tabBar->setMovable(true);
        // So it uses the context menu of the main window.
        tabBar->setContextMenuPolicy(Qt::NoContextMenu);
        // Allow the manager to filter the events received by this tab bar to allow for tab detaching.
		tabBar->installEventFilter( this );
		// Receive tab close requests.
        connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
		// Add to the list to prevent installing the event filter and connecting the tab close signal again.
        _tabBarList->append(tabBar);
	}	
}

bool DockWidgetManager::eventFilter(QObject* target, QEvent* event)
{
	// Only tab bars used by the internal dock system pass through this filter.
    QTabBar* tabBar = static_cast<QTabBar*>(target);
	
	// Only accept mouse events.
	QEvent::Type eventType = event->type();
    if (eventType != QEvent::MouseMove &&
        eventType != QEvent::MouseButtonPress &&
        eventType != QEvent::MouseButtonRelease &&
        eventType != QEvent::MouseButtonDblClick)
    {
		return false;
    }
	// Process the mouse event.
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	bool consumed = false;
	
	// Mouse press.
    if (eventType == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton)
	{
		// Mark the tab bar and tab page being clicked, and record the click position.
        _pressedTabBar = tabBar;
        _pressedTabIndex = tabBar->tabAt(mouseEvent->pos());
        _clickPosition = mouseEvent->pos();
	}
	
	// Mouse move.
    if (eventType == QEvent::MouseMove && _pressedTabBar)
	{			
        /*	There's a dock tab bar being left-button-pressed and moved on, so we check for the tab detaching condition.
			There are a couple of ways to consider a detaching state. One way is to check for the mouse moving
			outside of the tab page that was clicked, like this:
			
            if (_pressedTabIndex != tabBar->tabAt(mouseEvent->pos()))

			The implementation below is the "drag distance" method, which detaches a dock when the mouse has
            moved a certain application-specific distance.
        */
        if ((mouseEvent->pos() - _clickPosition).manhattanLength() > QApplication::startDragDistance())
		{			
			// The user wants to detach a dock tab.
            if (_pressedTabIndex != -1)
			{				
				consumed = true;			
				
                // It is essential to release the mouse button that is currently holding on to the tab bar
                // so that the dock widget can be dragged. This needs to be done with an OS level event.
                sendPlatformMouseEvent(PlatformMouseEvent::PLATFORM_MOUSE_EVENT_RELEASE, 0, 0);
				
                // Detach the dock that the user wants.
                // This restores the default title bar from the top level change, and also
                // restores the title bar of its tab bar neighbour if they were in a two-page tab bar
                DockWidget* detachingDock = dockFromTab(_pressedTabBar, _pressedTabIndex);
                detachingDock->setFloating(true);
				
				// The left mouse button is being held right now, so force the dock to be dragged by the cursor.
                QPoint cursorPosition = QCursor::pos();
                detachingDock->centerTitle(cursorPosition);
												
				// Synthesize an OS mouse press to activate the dragging behaviour of the dock.
                // Necesssary to avoid an animation glitch when floating and force-dragging the dock widget.
                QApplication::processEvents();
                // Necessary to avoid fast mouse movements in the event queue disrupting the dragging.
                QCursor::setPos(cursorPosition);

                sendPlatformMouseEvent(PlatformMouseEvent::PLATFORM_MOUSE_EVENT_RELEASE, 0, 0);
			}
		}				
	}
	
	// Mouse double click.
    if (eventType == QEvent::MouseButtonDblClick && mouseEvent->button() == Qt::LeftButton)
	{
		// As an alternative method of detaching a tabified dock.
        _pressedTabIndex = tabBar->tabAt(mouseEvent->pos());
        if (_pressedTabIndex != -1)
		{
			consumed = true;
			// Detach the dock tab being double-clicked.
            DockWidget* detachingDock = dockFromTab(tabBar, _pressedTabIndex);
            detachingDock->setFloating(true);
            detachingDock->centerTitle(mouseEvent->globalPos());
            // Optional call, this makes the dock window highlighted.
            detachingDock->activateWindow();
		}
	}
		
	// Mouse release.
    if (eventType == QEvent::MouseButtonRelease)
	{		
        _pressedTabBar = nullptr;
        _pressedTabIndex = -1;
		// Allow right-click releases on tab bars to pop up the main window context menu, it's very convenient.	
        if (mouseEvent->button() == Qt::RightButton)
        {
            _mainWindow->createPopupMenu()->popup(mouseEvent->globalPos());
        }
	}
	return consumed;
}


void DockWidgetManager::onRestoringState()
{
    // The main window has just restored its state, and this disrupts our custom detachable tab system.
    QList<DockWidget*> allDetachableDocks = _mainWindow->findChildren<DockWidget*>();
	
	// New tab bars for docks may have been created under this new state.
    QList<QTabBar*> allTabBars = _mainWindow->findChildren<QTabBar*>();
    Q_FOREACH(QTabBar* tabBar, allTabBars)
	{
        if (tabBar->count() > 0)
		{
			// If there's a valid dock pointer in this tab bar, try to register it.
			// If it's already registered nothing happens.
            if (dockFromTab(tabBar, 0))
                registerTabBar(tabBar);
		}
	}
	
	// Re-establish the detachable dock and tab bar relationships.
	// These tab bars are the ones used by the internal dock system.
    Q_FOREACH(QTabBar* tabBar, *_tabBarList)
	{
        for (int index = 0; index < tabBar->count(); ++index)
		{
            DockWidget* tempDock = dockFromTab(tabBar, index);
            tempDock->_lastTabBar = tabBar;
            topLevelUpdateDock(tempDock, TOP_LEVEL_CHANGE_TABIFIED);
            // This dock is now handled, remove it from the list.
            allDetachableDocks.removeOne(tempDock);
		}
	}
	
    // Re-establish the look and feel of the remaining detachable docks.
    // They are either TOP_LEVEL_CHANGE_DOCKED or TOP_LEVEL_CHANGE_FLOATING.
    Q_FOREACH(DockWidget* dock, allDetachableDocks)
	{
        dock->_lastTabBar = nullptr;
        // TOP_LEVEL_CHANGE_DOCKED or TOP_LEVEL_CHANGE_FLOATING have the same result
        topLevelUpdateDock(dock, TOP_LEVEL_CHANGE_DOCKED);
	}
}

void DockWidgetManager::onTabCloseRequested(int index)
{			
	// Obtain the signaling tab.
    const QTabBar* tabBar = qobject_cast<QTabBar*>(sender());
	
    // Obtain the dock represented by the tab being closed and trigger
    // its 'toggleViewAction' to close it. This in turn removes the tab page.
    const DockWidget* dock = dockFromTab(tabBar, index);
	
	QAction* closeAction = dock->toggleViewAction();
	closeAction->trigger();
}



#if defined(Q_OS_WIN)

#include <windows.h>
#include <winuser.h>

void sendPlatformMouseEvent(PlatformMouseEvent ev, int x, int y)
{
    switch (ev)
    {
        case PLATFORM_MOUSE_EVENT_PRESS:
        {
            // Send a Windows left mouse button press event using SendInput.
            INPUT pressEvent;
            pressEvent.type = INPUT_MOUSE;
            // X and Y are treated as relative
            pressEvent.mi.dx = x;
            pressEvent.mi.dy = y;
            pressEvent.mi.mouseData = 0;
            pressEvent.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            pressEvent.mi.time = 0;
            SendInput( 1, &pressEvent, sizeof(INPUT));
            break;
        }
        case PLATFORM_MOUSE_EVENT_RELEASE:
        {
            // Send a Windows left mouse button release event using SendInput.
            INPUT releaseEvent;
            releaseEvent.type = INPUT_MOUSE;
            releaseEvent.mi.dx = x;
            releaseEvent.mi.dy = y;
            releaseEvent.mi.mouseData = 0;
            releaseEvent.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            releaseEvent.mi.time = 0;
            SendInput( 1, &releaseEvent, sizeof(INPUT));
            break;
        }
        case PLATFORM_MOUSE_EVENT_MOVE:
            break;
    }
}
#elif defined(Q_OS_LINUX)
    // Include X11 specific headers here.

    void sendPlatformMouseEvent(PlatformMouseEvent ev, int x, int y)
    {
        // todo:
        // Synthesize the Linux (X11) left mouse events here. For example:
        // https://gist.github.com/pioz/726474
        switch (ev)
        {
        case PLATFORM_MOUSE_EVENT_PRESS:
            break;
        case PLATFORM_MOUSE_EVENT_RELEASE:
            break;
        case PLATFORM_MOUSE_EVENT_MOVE:
            break;
        }
    }
#elif defined(Q_OS_OSX)

#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#import <ApplicationServices/ApplicationServices.h>

void sendPlatformMouseEvent(PlatformMouseEvent ev, int x, int y)
{
    switch (ev)
    {
        case PLATFORM_MOUSE_EVENT_PRESS:
        {
            CGPoint location = CGPointMake(x, y);
            CGEventRef mouseDown = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, location, kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, mouseDown);
            CFRelease(mouseDown);
            break;
        }
        case PLATFORM_MOUSE_EVENT_RELEASE:
        {
            CGPoint location = CGPointMake(x, y);
            CGEventRef mouseUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, location, kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, mouseUp);
            CFRelease(mouseUp);
            break;
        }
        case PLATFORM_MOUSE_EVENT_MOVE:
        {
            CGPoint location = CGPointMake(x, y);
            if (CGWarpMouseCursorPosition(location) == kCGErrorSuccess)
                CGAssociateMouseAndMouseCursorPosition(true);
            break;
        }
    }
}
#else
#endif

