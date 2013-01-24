#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "Vector2.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Touch.h"
#include "Gesture.h"
#include "Gamepad.h"

namespace gameplay
{

class Game;

/**
 * Defines a platform abstraction.
 * 
 * This class has only a few public methods for creating a platform 
 *
 */
class Platform
{
public:

    friend class Game;
    friend class Gamepad;
    friend class ScreenDisplayer;

    /**
     * Destructor.
     */
    ~Platform();

    /**
     * Creates a platform for the specified game which it will interact with.
     *
     * @param game The game to create a platform for.
     * @param attachToWindow The native window handle to optionally attach to.
     * 
     * @return The created platform interface.
     * @script{ignore}
     */
    static Platform* create(Game* game, void* attachToWindow = NULL);

    /**
     * Begins processing the platform messages.
     *
     * This method handles all OS window messages and drives the game loop.
     * It normally does not return until the application is closed.
     * 
     * If a attachToWindow is passed to Platform::create the message pump will instead attach
     * to or allow the attachToWindow to drive the game loop on the platform.
     *
     * @return The platform message pump return code.
     */
    int enterMessagePump();

private:
    
    /**
     * This method informs the platform that the game is shutting down 
     * and anything platform specific should be shutdown as well or halted
     * This function is called automatically when the game shutdown function is called
     */
    static void signalShutdown();

    /**
     * Indicates whether a programmatic exit is allowed on this platform.
     * Some platforms (eg. iOS) do not allow apps to exit programmatically.
     *
     * @return whether a programmatic exit is allowed on this platform.
     */
    static bool canExit();
    
    /**
     * Gets the display width.
     * 
     * @return The display width.
     */
    static unsigned int getDisplayWidth();
    
    /**
     * Gets the display height.
     * 
     * @return The display height.
     */
    static unsigned int getDisplayHeight();

    /**
     * Gets the absolute platform time starting from when the message pump was started.
     *
     * @return The absolute platform time. (in milliseconds)
     */
    static double getAbsoluteTime();

    /**
     * Sets the absolute platform time since the start of the message pump.
     *
     * @param time The time to set (in milliseconds).
     */
    static void setAbsoluteTime(double time);

    /**
     * Gets whether vertical sync is enabled for the game display.
     * 
     * @return true if vsync is enabled; false if not.
     */
    static bool isVsync();

    /**
     * Sets whether vertical sync is enable for the game display.
     *
     * @param enable true if vsync is enabled; false if not.
     */
    static void setVsync(bool enable);

    /**
     * Swaps the frame buffer on the device.
     */
    static void swapBuffers();

    /**
     * Sleeps synchronously for the given amount of time (in milliseconds).
     *
     * @param ms How long to sleep (in milliseconds).
     */
    static void sleep(long ms);

    /**
     * Set if multi-touch is enabled on the platform.
     *
     * Note that this method does nothing on platforms that do not
     * support multi-touch.
     */
    static void setMultiTouch(bool enabled);

   /**
    * Is multi-touch mode enabled.
    */
    static bool isMultiTouch();

    /**
     * Whether the platform has mouse support.
     */
    static bool hasMouse();
    
    /**
     * Enables or disabled mouse capture.
     *
     * When mouse capture is enabled, the platform cursor is hidden
     * and mouse event points are delivered as position deltas instead
     * of absolute positions.
     *
     * This is useful for games that wish to provide uninhibited mouse
     * movement, such as when implementing free/mouse look in an FPS
     * game.
     *
     * Disabling mouse capture moves the mouse back to the center of the
     * screen and shows the platform cursor.
     *
     * Note that this method does nothing on platforms that do not
     * support a mouse.
     *
     * @param captured True to enable mouse capture, false to disable it.
     */
    static void setMouseCaptured(bool captured);

    /**
     * Determines if mouse capture is currently enabled.
     */
    static bool isMouseCaptured();

    /**
     * Sets the visibility of the platform cursor.
     *
     * On platforms that support a visible cursor, this method
     * toggles the visibility of the cursor.
     *
     * @param visible true to show the platform cursor, false to hide it.
     */
    static void setCursorVisible(bool visible);

    /**
     * Determines whether the platform cursor is currently visible.
     *
     * @return true if the platform cursor is visible, false otherwise.
     */
    static bool isCursorVisible();

    /**
     * Gets the platform accelerometer values.
     * 
     * @param pitch The accelerometer pitch.
     * @param roll The accelerometer roll.
     */
    static void getAccelerometerValues(float* pitch, float* roll);
    
    /**
     * Shows or hides the virtual keyboard (if supported).
     *
     * @param display true when virtual keyboard needs to be displayed and false otherwise.
     */
    static void displayKeyboard(bool display);

    /**
     * Tests if the specified gesture is supported on the platform.
     *
     * @return true if it is supported; false if not supported.
     */
    static bool isGestureSupported(Gesture::GestureEvent evt);

    /**
     * Registers the platform for gesture recognition for the specified gesture event.
     *
     * @param evt The gesture event to register to start recognizing events for.
     */
    static void registerGesture(Gesture::GestureEvent evt);

    /**
     * Registers the platform for gesture recognition for the specified gesture event.
     *
     * @param evt The gesture event to register to start recognizing events for.
     */
    static void unregisterGesture(Gesture::GestureEvent evt);

    /**
     * Tests if the specified gesture is registered for gesture recognition for the specified gesture event
     *
     * @param evt The gesture event to register to start recognizing events for.
     */
    static bool isGestureRegistered(Gesture::GestureEvent evt);

    /**
     * Opens an URL in an external browser, if available.
     *
     * @param url URL to be opened.
     *
     * @return True if URL was opened successfully, false otherwise.
     */
    static bool launchURL(const char* url);

    /**
     * Constructor.
     */
    Platform(Game* game);

    /**
     * Constructor.
     */
    Platform(const Platform& copy);

public:

   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static void touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static void keyEventInternal(Keyboard::KeyEvent evt, int key);

   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static bool mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static void gamepadEventConnectedInternal(GamepadHandle handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                                              unsigned int vendorId, unsigned int productId, 
                                              const char* vendorString, const char* productString);
   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static void gamepadEventDisconnectedInternal(GamepadHandle handle);

    /**
     * Internal metehod used by Gamepad that polls the platform for the updated Gamepad
     * states such as joysticks, buttons and trigger values.
     *
     * @param gamepad The gamepad to be returned with the latest polled values populated.
     * @script{ignore}
     */
    static void pollGamepadState(Gamepad* gamepad);

   /**
     * Internal method used only from static code in various platform implementation.
     *
     * @script{ignore}
     */
    static void shutdownInternal();

private:

    Game* _game;                // The game this platform is interfacing with.
};

}

#include "Game.h"

#endif
