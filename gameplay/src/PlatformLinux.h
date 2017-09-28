#pragma once

#include "Platform.h"
#include "Input.h"
#include "Graphics.h"
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

private:

    void onMessage(const xcb_generic_event_t* evt);
    Input::Key translateKey(xcb_keycode_t xcbKeyCode, bool shiftDown);

	xcb_connection_t* _connection;
	xcb_screen_t* _screen;
	xcb_window_t _window;
	xcb_intern_atom_reply_t* _windowDeleteReply;
	bool _quit;
	Graphics* _graphics;
};

}
