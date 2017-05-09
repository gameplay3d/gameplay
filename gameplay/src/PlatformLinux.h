#ifdef __linux__

#pragma once

#include "Platform.h"
#include <xcb/xcb.h>
#include <linux/joystick.h>
#include <stdarg.h>

namespace gameplay
{

/**
 * Defines a linux platform impl.
 */
class PlatformLinux : public Platform
{
public:

	/**
	 * Constructor.
	 */
	PlatformLinux();

	/**
	 * Destructor
	 */
	~PlatformLinux();

	/**
	 * @see Platform::startup
	 */
	bool startup(int argc, char** argv);

	/**
	 * @see Platform::shutdown
	 */
	void shutdown();

	/**
	 * @see Platform::enterMessagePump
	 */
	int enterMessagePump();

	/**
	 * @see Platform::getWindow
	 */
	unsigned long getWindow();

	/**
	 * @see Platform::getConnection
	 */
	unsigned long getConnection();

	/**
	 * @see Platform::isGamepadButtonPressed
	 */
	bool isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex);

	/**
	 * @see Platform::getGamepadAxisValues
	 */
	void getGamepadAxisValues(float* leftVertical, float* leftHorizontal, 
                              float* rightVertical, float* rightHorizontal, 
                              size_t gamepadIndex);

	/**
	 * @see Platform::getGamepadTriggerValues
	 */
	void getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex);

	/**
	 * @see Platform::getAccelerometerValues
	 */
	void getAccelerometerValues(float* pitch, float* roll);

	/**
	 * @see Platform::getSensorValues
	 */
	void getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ);

private:

	void handleEvent(const xcb_generic_event_t* evt);
	KeyboardKey translateKey(xcb_keycode_t xcbKeyCode, bool shiftDown);

	xcb_connection_t* _connection;
	xcb_screen_t* _screen;
	xcb_window_t _window;
	xcb_intern_atom_reply_t* _windowDeleteReply;
	bool _quit;
	Graphics* _graphics;
};

}

#endif
