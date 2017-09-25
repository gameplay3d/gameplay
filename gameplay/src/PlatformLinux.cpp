#include "Base.h"
#include "PlatformLinux.h"
#include "Game.h"
#include "Graphics.h"

namespace gameplay
{

extern Platform* getPlatform()
{
    static Platform* platform = new PlatformLinux();
    return platform;
}

PlatformLinux::PlatformLinux() :
	_connection(nullptr),
	_screen(nullptr),
	_window(0),
	_windowDeleteReply(nullptr),
	_quit(false),
	_graphics(nullptr)
{
}

PlatformLinux::~PlatformLinux()
{
}

bool PlatformLinux::startup(int argc, char** argv)
{
	// Get the game config
	Game::Config* config = Game::getInstance()->getConfig();

	// Create the xcb connection
	int screenCount;
	_connection = xcb_connect(nullptr, &screenCount);
	if (_connection == nullptr)
	{
		printf("Failed to create vulkan icd connection!\n");
		return false;
	}

	// Find the screen
	const xcb_setup_t* setup;
	setup = xcb_get_setup(_connection);	
	xcb_screen_iterator_t itr;
	itr = xcb_setup_roots_iterator(setup);
	for (int i = 0; i < screenCount; i++)
	{
		xcb_screen_next(&itr);
	}
	_screen = itr.data;

	// Create the window
	uint32_t windowAttribMask;
	uint32_t windowAttribs[32];
	_window = xcb_generate_id(_connection);
	windowAttribMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	windowAttribs[0] = _screen->black_pixel;
	windowAttribs[1] = XCB_EVENT_MASK_KEY_RELEASE |
					   XCB_EVENT_MASK_KEY_PRESS |
					   XCB_EVENT_MASK_EXPOSURE |
					   XCB_EVENT_MASK_STRUCTURE_NOTIFY |
					   XCB_EVENT_MASK_POINTER_MOTION |
					   XCB_EVENT_MASK_BUTTON_PRESS |
					   XCB_EVENT_MASK_BUTTON_RELEASE;
	xcb_create_window(_connection, 
					  XCB_COPY_FROM_PARENT, _window, _screen->root, 
					  config->x, config->y, config->width, config->height, config->fullscreen ? 0 : 1,
					  XCB_WINDOW_CLASS_INPUT_OUTPUT, _screen->root_visual,
					  windowAttribMask, windowAttribs);

	// Window destroy event
	xcb_intern_atom_cookie_t windowProtocolsCookie = xcb_intern_atom(_connection, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t* windowProtocolsReply = xcb_intern_atom_reply(_connection, windowProtocolsCookie, 0);
	xcb_intern_atom_cookie_t windowDeleteCookie = xcb_intern_atom(_connection, 0, 16, "WM_DELETE_WINDOW");
	_windowDeleteReply = xcb_intern_atom_reply(_connection, windowDeleteCookie, 0);
	xcb_change_property(_connection, 
						XCB_PROP_MODE_REPLACE, _window, (*windowProtocolsReply).atom, 
						XCB_ATOM_ATOM, 32, 1, &(*_windowDeleteReply).atom);

	// Set fullscreen mode
	if (config->fullscreen)
	{
		xcb_intern_atom_cookie_t windowStateCookie = xcb_intern_atom(_connection, 1, 13, "_NET_WM_STATE");
		xcb_intern_atom_reply_t* windowStateReply = xcb_intern_atom_reply(_connection, windowStateCookie, 0);
		xcb_intern_atom_cookie_t windowStateFullscreenCookie = xcb_intern_atom(_connection, 1, 24, "_NET_WM_STATE_FULLSCREEN");
		xcb_intern_atom_reply_t* windowStateFullscreenReply = xcb_intern_atom_reply(_connection, windowStateFullscreenCookie, 0);
		if (windowStateReply != XCB_NONE && windowStateFullscreenReply != XCB_NONE)
			xcb_change_property(_connection, 
								XCB_PROP_MODE_REPLACE, _window, (*windowStateReply).atom, 
								XCB_ATOM_ATOM, 32, 1, &(*windowStateFullscreenReply).atom);
		else
			GP_WARN("Fullscreen not supported.");
	}

	// Set the window title
	xcb_change_property(_connection, 
						XCB_PROP_MODE_REPLACE, _window, 
						XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, config->title.size(), config->title.c_str());
	
    // Intialize the graphics system
    Game::getGraphics()->onInitialize((unsigned long)_window, (unsigned long)_connection);

	// Show the window
	xcb_map_window(_connection, _window);
	xcb_flush(_connection);

	Game::getInstance()->onInitialize();

	return true;
}

void PlatformLinux::shutdown()
{
	if (_graphics)
		_graphics->onFinalize();

	Game::getInstance()->onFinalize();
	xcb_disconnect(_connection);
}

int PlatformLinux::enterMessagePump()
{
	while (!_quit)
	{
		xcb_generic_event_t* evt;
		while ((evt = xcb_poll_for_event(_connection)))
		{
			onMessage(evt);
			free(evt);
		}
	}
	return 0;
}

void PlatformLinux::onMessage(const xcb_generic_event_t* evt)
{
	Game* game = gameplay::Game::getInstance();

	static bool shiftDown = false;
	static bool capsOn = false;

	switch (evt->response_type & 0x7f)
	{
		case XCB_CLIENT_MESSAGE:
		{
			if ((*(xcb_client_message_event_t*)evt).data.data32[0] == (*_windowDeleteReply).atom)
				_quit = true;
			break;
		}
		case XCB_MOTION_NOTIFY:
		{
			xcb_motion_notify_event_t* motionEvent = (xcb_motion_notify_event_t*)evt;
			float x = (float)motionEvent->event_x;
			float y = (float)motionEvent->event_y;
			game->mouseEvent(Platform::MOUSE_EVENT_MOVE, x, y, 0);
			break;
		}
		case XCB_BUTTON_PRESS:
		{
			xcb_button_press_event_t* pressEvent = (xcb_button_press_event_t*)evt;
			float x = (float)pressEvent->event_x;
			float y = (float)pressEvent->event_y;
			if (pressEvent->detail == XCB_BUTTON_INDEX_1)
				game->mouseEvent(Platform::MOUSE_EVENT_PRESS_LEFT_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_2)
				game->mouseEvent(Platform::MOUSE_EVENT_PRESS_MIDDLE_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_3)
				game->mouseEvent(Platform::MOUSE_EVENT_PRESS_RIGHT_BUTTON, x, y, 0);

			break;
		}
		case XCB_BUTTON_RELEASE:
		{
			xcb_button_press_event_t* pressEvent = (xcb_button_press_event_t*)evt;
			float x = (float)pressEvent->event_x;
			float y = (float)pressEvent->event_y;
			if (pressEvent->detail == XCB_BUTTON_INDEX_1)
				game->mouseEvent(Platform::MOUSE_EVENT_RELEASE_LEFT_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_2)
				game->mouseEvent(Platform::MOUSE_EVENT_RELEASE_MIDDLE_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_3)
				game->mouseEvent(Platform::MOUSE_EVENT_RELEASE_RIGHT_BUTTON, x, y, 0);
			break;
		}
		case XCB_KEY_PRESS:
		{
			const xcb_key_release_event_t* keyEvent = (const xcb_key_release_event_t *)evt;
			//if (keyEvent->detail == KEY_SHIFT || keyEvent->detail == KEY_LSHIFT || keyEvent->detail == KEY_RSHIFT)
			//	shiftDown = true;
			//if (keyEvent->detail == KEY_CAPITAL)
			//	capsOn = !capsOn;
			game->keyEvent(Platform::KEYBOARD_EVENT_PRESS, translateKey(keyEvent->detail, shiftDown ^ capsOn));
			break;
		}
		case XCB_KEY_RELEASE:
		{
			const xcb_key_release_event_t* keyEvent = (const xcb_key_release_event_t*)evt;
			//if (keyEvent->detail == KEY_SHIFT || keyEvent->detail == KEY_LSHIFT || keyEvent->detail == KEY_RSHIFT)
			//	shiftDown = false;
			game->keyEvent(Platform::KEYBOARD_EVENT_RELEASE, translateKey(keyEvent->detail, shiftDown ^ capsOn));
			break;
		}
		case XCB_DESTROY_NOTIFY:
		{
			_quit = true;
			break;
		}
		case XCB_CONFIGURE_NOTIFY:
		{
			/*const xcb_configure_notify_event_t* cfgEvent = (const xcb_configure_notify_event_t*)evt;
			if ((game->getState() == Game::STATE_RUNNING) && ((cfgEvent->width != width) || (cfgEvent->height != height)))
			{
				destWidth = cfgEvent->width;
				destHeight = cfgEvent->height;
				
				// TODO: resize event
			}*/
			break;
		}

		default:
			break;
	}
}

unsigned long PlatformLinux::getWindow()
{
	return (unsigned long)_window;
}

unsigned long PlatformLinux::getConnection()
{
	return (unsigned long)_connection;
}

bool PlatformLinux::isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex)
{
}

void PlatformLinux::getGamepadAxisValues(float* leftVertical, float* leftHorizontal, float* rightVertical, float* rightHorizontal, size_t gamepadIndex)
{
}

void PlatformLinux::getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex)
{
}

void PlatformLinux::getAccelerometerValues(float* pitch, float* roll)
{
}

void PlatformLinux::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
}

Platform::KeyboardKey PlatformLinux::translateKey(xcb_keycode_t xcbKeyCode, bool shiftDown)
{
	switch (xcbKeyCode)
	{
		/*case KEY_PAUSE:
			return gameplay::Platform::KEYBOARD_KEY_PAUSE;
		case KEY_SCROLL:
			return gameplay::Platform::KEYBOARD_KEY_SCROLL_LOCK;
		case KEY_PRINT:
			return gameplay::Platform::KEYBOARD_KEY_PRINT;
		case KEY_ESCAPE:
			return gameplay::Platform::KEYBOARD_KEY_ESCAPE;
		case KEY_BACK:
		case KEY_F16:
			return gameplay::Platform::KEYBOARD_KEY_BACKSPACE;
		case KEY_TAB:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_BACK_TAB : gameplay::Platform::KEYBOARD_KEY_TAB;
		case KEY_RETURN:
			return gameplay::Platform::KEYBOARD_KEY_RETURN;
		case KEY_CAPITAL:
			return gameplay::Platform::KEYBOARD_KEY_CAPS_LOCK;
		case KEY_SHIFT:
			return gameplay::Platform::KEYBOARD_KEY_SHIFT;
		case KEY_CONTROL:
			return gameplay::Platform::KEYBOARD_KEY_CTRL;
		case KEY_MENU:
			return gameplay::Platform::KEYBOARD_KEY_ALT;
		case KEY_APPS:
			return gameplay::Platform::KEYBOARD_KEY_MENU;
		case KEY_LSHIFT:
			return gameplay::Platform::KEYBOARD_KEY_SHIFT;
		case KEY_RSHIFT:
			return gameplay::Platform::KEYBOARD_KEY_SHIFT;
		case KEY_LCONTROL:
			return gameplay::Platform::KEYBOARD_KEY_CTRL;
		case KEY_RCONTROL:
			return gameplay::Platform::KEYBOARD_KEY_CTRL;
		case KEY_LMENU:
			return gameplay::Platform::KEYBOARD_KEY_ALT;
		case KEY_RMENU:
			return gameplay::Platform::KEYBOARD_KEY_ALT;
		case KEY_LWIN:
		case KEY_RWIN:
			return gameplay::Platform::KEYBOARD_KEY_HYPER;
		case KEY_BROWSER_SEARCH:
			return gameplay::Platform::KEYBOARD_KEY_SEARCH;
		case KEY_INSERT:
			return gameplay::Platform::KEYBOARD_KEY_INSERT;
		case KEY_HOME:
			return gameplay::Platform::KEYBOARD_KEY_HOME;
		case KEY_PRIOR:
			return gameplay::Platform::KEYBOARD_KEY_PG_UP;
		case KEY_DELETE:
			return gameplay::Platform::KEYBOARD_KEY_DELETE;
		case KEY_END:
			return gameplay::Platform::KEYBOARD_KEY_END;
		case KEY_NEXT:
			return gameplay::Platform::KEYBOARD_KEY_PG_DOWN;
		case KEY_LEFT:
			return gameplay::Platform::KEYBOARD_KEY_LEFT_ARROW;
		case KEY_RIGHT:
			return gameplay::Platform::KEYBOARD_KEY_RIGHT_ARROW;
		case KEY_UP:
			return gameplay::Platform::KEYBOARD_KEY_UP_ARROW;
		case KEY_DOWN:
			return gameplay::Platform::KEYBOARD_KEY_DOWN_ARROW;
		case KEY_NUMLOCK:
			return gameplay::Platform::KEYBOARD_KEY_NUM_LOCK;
		case KEY_ADD:
			return gameplay::Platform::KEYBOARD_KEY_KP_PLUS;
		case KEY_SUBTRACT:
			return gameplay::Platform::KEYBOARD_KEY_KP_MINUS;
		case KEY_MULTIPLY:
			return gameplay::Platform::KEYBOARD_KEY_KP_MULTIPLY;
		case KEY_DIVIDE:
			return gameplay::Platform::KEYBOARD_KEY_KP_DIVIDE;
		case KEY_NUMPAD7:
			return gameplay::Platform::KEYBOARD_KEY_KP_HOME;
		case KEY_NUMPAD8:
			return gameplay::Platform::KEYBOARD_KEY_KP_UP;
		case KEY_NUMPAD9:
			return gameplay::Platform::KEYBOARD_KEY_KP_PG_UP;
		case KEY_NUMPAD4:
			return gameplay::Platform::KEYBOARD_KEY_KP_LEFT;
		case KEY_NUMPAD5:
			return gameplay::Platform::KEYBOARD_KEY_KP_FIVE;
		case KEY_NUMPAD6:
			return gameplay::Platform::KEYBOARD_KEY_KP_RIGHT;
		case KEY_NUMPAD1:
			return gameplay::Platform::KEYBOARD_KEY_KP_END;
		case KEY_NUMPAD2:
			return gameplay::Platform::KEYBOARD_KEY_KP_DOWN;
		case KEY_NUMPAD3:
			return gameplay::Platform::KEYBOARD_KEY_KP_PG_DOWN;
		case KEY_NUMPAD0:
			return gameplay::Platform::KEYBOARD_KEY_KP_INSERT;
		case KEY_DECIMAL:
			return gameplay::Platform::KEYBOARD_KEY_KP_DELETE;
		case KEY_F1:
			return gameplay::Platform::KEYBOARD_KEY_F1;
		case KEY_F2:
			return gameplay::Platform::KEYBOARD_KEY_F2;
		case KEY_F3:
			return gameplay::Platform::KEYBOARD_KEY_F3;
		case KEY_F4:
			return gameplay::Platform::KEYBOARD_KEY_F4;
		case KEY_F5:
			return gameplay::Platform::KEYBOARD_KEY_F5;
		case KEY_F6:
			return gameplay::Platform::KEYBOARD_KEY_F6;
		case KEY_F7:
			return gameplay::Platform::KEYBOARD_KEY_F7;
		case KEY_F8:
			return gameplay::Platform::KEYBOARD_KEY_F8;
		case KEY_F9:
			return gameplay::Platform::KEYBOARD_KEY_F9;
		case KEY_F10:
			return gameplay::Platform::KEYBOARD_KEY_F10;
		case KEY_F11:
			return gameplay::Platform::KEYBOARD_KEY_F11;
		case KEY_F12:
			return gameplay::Platform::KEYBOARD_KEY_F12;
		case KEY_SPACE:
			return gameplay::Platform::KEYBOARD_KEY_SPACE;
		case 0x30:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_RIGHT_PARENTHESIS : gameplay::Platform::KEYBOARD_KEY_ZERO;
		case 0x31:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_EXCLAM : gameplay::Platform::KEYBOARD_KEY_ONE;
		case 0x32:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_AT : gameplay::Platform::KEYBOARD_KEY_TWO;
		case 0x33:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_NUMBER : gameplay::Platform::KEYBOARD_KEY_THREE;
		case 0x34:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_DOLLAR : gameplay::Platform::KEYBOARD_KEY_FOUR;
		case 0x35:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_PERCENT : gameplay::Platform::KEYBOARD_KEY_FIVE;
		case 0x36:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CIRCUMFLEX : gameplay::Platform::KEYBOARD_KEY_SIX;
		case 0x37:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_AMPERSAND : gameplay::Platform::KEYBOARD_KEY_SEVEN;
		case 0x38:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_ASTERISK : gameplay::Platform::KEYBOARD_KEY_EIGHT;
		case 0x39:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_LEFT_PARENTHESIS : gameplay::Platform::KEYBOARD_KEY_NINE;
		case KEY_OEM_PLUS:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_EQUAL : gameplay::Platform::KEYBOARD_KEY_PLUS;
		case KEY_OEM_COMMA:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_LESS_THAN : gameplay::Platform::KEYBOARD_KEY_COMMA;
		case KEY_OEM_MINUS:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_UNDERSCORE : gameplay::Platform::KEYBOARD_KEY_MINUS;
		case KEY_OEM_PERIOD:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_GREATER_THAN : gameplay::Platform::KEYBOARD_KEY_PERIOD;
		case KEY_OEM_1:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_COLON : gameplay::Platform::KEYBOARD_KEY_SEMICOLON;
		case KEY_OEM_2:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_QUESTION : gameplay::Platform::KEYBOARD_KEY_SLASH;
		case KEY_OEM_3:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_TILDE : gameplay::Platform::KEYBOARD_KEY_GRAVE;
		case KEY_OEM_4:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_LEFT_BRACE : gameplay::Platform::KEYBOARD_KEY_LEFT_BRACKET;
		case KEY_OEM_5:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_BAR : gameplay::Platform::KEYBOARD_KEY_BACK_SLASH;
		case KEY_OEM_6:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_RIGHT_BRACE : gameplay::Platform::KEYBOARD_KEY_RIGHT_BRACKET;
		case KEY_OEM_7:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_QUOTE : gameplay::Platform::KEYBOARD_KEY_APOSTROPHE;
		case 0x41:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_A : gameplay::Platform::KEYBOARD_KEY_A;
		case 0x42:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_B : gameplay::Platform::KEYBOARD_KEY_B;
		case 0x43:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_C : gameplay::Platform::KEYBOARD_KEY_C;
		case 0x44:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_D : gameplay::Platform::KEYBOARD_KEY_D;
		case 0x45:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_E : gameplay::Platform::KEYBOARD_KEY_E;
		case 0x46:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_F : gameplay::Platform::KEYBOARD_KEY_F;
		case 0x47:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_G : gameplay::Platform::KEYBOARD_KEY_G;
		case 0x48:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_H : gameplay::Platform::KEYBOARD_KEY_H;
		case 0x49:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_I : gameplay::Platform::KEYBOARD_KEY_I;
		case 0x4A:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_J : gameplay::Platform::KEYBOARD_KEY_J;
		case 0x4B:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_K : gameplay::Platform::KEYBOARD_KEY_K;
		case 0x4C:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_L : gameplay::Platform::KEYBOARD_KEY_L;
		case 0x4D:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_M : gameplay::Platform::KEYBOARD_KEY_M;
		case 0x4E:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_N : gameplay::Platform::KEYBOARD_KEY_N;
		case 0x4F:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_O : gameplay::Platform::KEYBOARD_KEY_O;
		case 0x50:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_P : gameplay::Platform::KEYBOARD_KEY_P;
		case 0x51:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_Q : gameplay::Platform::KEYBOARD_KEY_Q;
		case 0x52:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_R : gameplay::Platform::KEYBOARD_KEY_R;
		case 0x53:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_S : gameplay::Platform::KEYBOARD_KEY_S;
		case 0x54:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_T : gameplay::Platform::KEYBOARD_KEY_T;
		case 0x55:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_U : gameplay::Platform::KEYBOARD_KEY_U;
		case 0x56:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_V : gameplay::Platform::KEYBOARD_KEY_V;
		case 0x57:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_W : gameplay::Platform::KEYBOARD_KEY_W;
		case 0x58:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_X : gameplay::Platform::KEYBOARD_KEY_X;
		case 0x59:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_Y : gameplay::Platform::KEYBOARD_KEY_Y;
		case 0x5A:
			return shiftDown ? gameplay::Platform::KEYBOARD_KEY_CAPITAL_Z : gameplay::Platform::KEYBOARD_KEY_Z;
		default:
		*/
			return gameplay::Platform::KEYBOARD_KEY_NONE;
	}
}

extern void print(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}

}
