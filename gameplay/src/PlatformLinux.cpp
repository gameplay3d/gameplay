#include "Base.h"
#include "PlatformLinux.h"
#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "vk/GraphicsVK.h"

namespace gameplay
{

extern Platform* getPlatform()
{
    static Platform* platform = new PlatformLinux();
    return platform;
}


Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        _graphics = new GraphicsVK();
        _graphics->_api = Graphics::API_VK;
    }
    return _graphics;
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
    std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();

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
                      0, 0, config->width, config->height, config->fullscreen ? 0 : 1,
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
    gameplay::Graphics::getGraphics()->onInitialize((unsigned long)_window, (unsigned long)_connection);

	// Show the window
	xcb_map_window(_connection, _window);
	xcb_flush(_connection);

    gameplay::Game::getInstance()->onInitialize();

	return true;
}

void PlatformLinux::shutdown()
{
    gameplay::Game::getInstance()->onFinalize();
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
            gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_MOVE, x, y, 0);
			break;
		}
		case XCB_BUTTON_PRESS:
		{
			xcb_button_press_event_t* pressEvent = (xcb_button_press_event_t*)evt;
			float x = (float)pressEvent->event_x;
			float y = (float)pressEvent->event_y;
			if (pressEvent->detail == XCB_BUTTON_INDEX_1)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_PRESS_LEFT_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_2)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_PRESS_MIDDLE_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_3)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_PRESS_RIGHT_BUTTON, x, y, 0);

			break;
		}
		case XCB_BUTTON_RELEASE:
		{
			xcb_button_press_event_t* pressEvent = (xcb_button_press_event_t*)evt;
			float x = (float)pressEvent->event_x;
			float y = (float)pressEvent->event_y;
			if (pressEvent->detail == XCB_BUTTON_INDEX_1)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_RELEASE_LEFT_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_2)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_RELEASE_MIDDLE_BUTTON, x, y, 0);
			if (pressEvent->detail == XCB_BUTTON_INDEX_3)
                gameplay::getPlatform()->onMouseEvent(Platform::MOUSE_EVENT_RELEASE_RIGHT_BUTTON, x, y, 0);
			break;
		}
		case XCB_KEY_PRESS:
		{
			const xcb_key_release_event_t* keyEvent = (const xcb_key_release_event_t *)evt;
			//if (keyEvent->detail == KEY_SHIFT || keyEvent->detail == KEY_LSHIFT || keyEvent->detail == KEY_RSHIFT)
			//	shiftDown = true;
			//if (keyEvent->detail == KEY_CAPITAL)
			//	capsOn = !capsOn;
            gameplay::getPlatform()->onKeyEvent(Platform::KEY_EVENT_PRESS, translateKey(keyEvent->detail, shiftDown ^ capsOn));
			break;
		}
		case XCB_KEY_RELEASE:
		{
			const xcb_key_release_event_t* keyEvent = (const xcb_key_release_event_t*)evt;
			//if (keyEvent->detail == KEY_SHIFT || keyEvent->detail == KEY_LSHIFT || keyEvent->detail == KEY_RSHIFT)
			//	shiftDown = false;
            gameplay::getPlatform()->onKeyEvent(Platform::KEY_EVENT_RELEASE, translateKey(keyEvent->detail, shiftDown ^ capsOn));
			break;
		}
		case XCB_DESTROY_NOTIFY:
		{
			_quit = true;
			break;
		}
		case XCB_CONFIGURE_NOTIFY:
		{
            const xcb_configure_notify_event_t* cfgEvent = (const xcb_configure_notify_event_t*)evt;
            if ((game->getState() == Game::STATE_RUNNING) && ((cfgEvent->width != game->getWidth())) || (cfgEvent->height != game->getHeight()))
			{
                uint16_t destWidth = cfgEvent->width;
                uint16_t destHeight = cfgEvent->height;
                if ((destWidth > 0) && (destHeight > 0))
                {
                    gameplay::Graphics::getGraphics()->onResize(destWidth, destHeight);
                    game->onResize(destWidth, destHeight);
                }
            }
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

Input::Key PlatformLinux::translateKey(xcb_keycode_t xcbKeyCode, bool shiftDown)
{
	switch (xcbKeyCode)
	{
        // TODO
        return gameplay::Input::KEY_NONE;
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
