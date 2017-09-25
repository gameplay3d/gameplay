#pragma once

#include "Input.h"

namespace gameplay
{

/**
 * Defines a platform abstraction.
 */
class Platform
{
public:

    /**
     * Defines platform key events.
     */
    enum KeyEvent
    {
        KEY_EVENT_PRESS,
        KEY_EVENT_RELEASE,
        KEY_EVENT_CHAR
    };

    /**
     * Defines platform mouse events
     */
     enum MouseEvent
    {
        MOUSE_EVENT_PRESS_LEFT_BUTTON,
        MOUSE_EVENT_RELEASE_LEFT_BUTTON,
        MOUSE_EVENT_PRESS_MIDDLE_BUTTON,
        MOUSE_EVENT_RELEASE_MIDDLE_BUTTON,
        MOUSE_EVENT_PRESS_RIGHT_BUTTON,
        MOUSE_EVENT_RELEASE_RIGHT_BUTTON,
        MOUSE_EVENT_MOVE,
        MOUSE_EVENT_WHEEL
    };

    /**
     * Defines platform gamepad events.
     */
    enum GamepadEvent
    {
        GAMEPAD_EVENT_CONNECTED,
        GAMEPAD_EVENT_DISCONNECTED
    };

	/**
	 * Starts up the platform and creates the initial windowing, input and graphics systems.
	 *
	 * @param argc The number of startup arguments.
	 * @param argv The array of startupu string values.
	 * @return true of the platform starts up  successfully, false if fails.
	 */
    virtual bool startup(int argc, char** argv) = 0;  

	/**
	 * Shuts down and cleans up any platform resource created and then exit application.
	 */
    virtual void shutdown() = 0; 

	/**
	 * Enters the application message pump and handles on platform messages.
	 *
	 * @return The exit code when the application closes.
	 */
    virtual int enterMessagePump() = 0;

	/**
	 * Gets the windows (native window handle/pointer).
	 *
	 * @return The window (native window handle/pointer).
	 */
    virtual unsigned long getWindow() = 0;

	/**
	 * Gets the native connection (native connection/context)
	 * 
	 * @return The connection (native connection/context).
	 */
    virtual unsigned long getConnection() = 0;

    /**
     * Event that is triggered on platform key events.
     *
     * @param event The key event triggerred.
     * @param key The key that is triggering the event.
     */
    void onKeyEvent(KeyEvent event, Input::Key key);

    /**
     * Event that is triggered on platform mouse events.
     *
     * @param event The mouse event triggered.
     * @param x The x position coordinate of the mouse in pixels. Left edge is zero.
     * @param y The y position coordinate of the mouse in pixels. Top edge is zero.
     * @param wheelDelta the number mouse mouse wheel ticks. Positive is up (forward), negative is down (backward).
     */
    void onMouseEvent(MouseEvent event, int x, int y, int wheelDelta);
    
    /**
     * Event that is triggered on platform gamepad events.
     *
     * @param event The gamepad event triggered.
     * @param index The gamepad index.
     */
    void onGamepadEvent(GamepadEvent event, size_t index);
};

}
