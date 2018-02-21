#include "Base.h"
#ifdef GP_PLATFORM_LINUX
#include "Platform.h"
#include "Input.h"

#include <bx/bx.h>
#include <bx/mutex.h>
#include <bx/handlealloc.h>
#include <bx/os.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include <bgfx/platform.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <fcntl.h>

#define JS_EVENT_BUTTON 0x01 /* button pressed/released */
#define JS_EVENT_AXIS   0x02 /* joystick moved */
#define JS_EVENT_INIT   0x80 /* initial state of device */

namespace gameplay
{

void setX11Window(void* display, uint32_t window, void* glx = nullptr)
{
	bgfx::PlatformData pd;
	pd.ndt          = display;
	pd.nwh          = (void*)(uintptr_t)window;
	pd.context      = glx;
	pd.backBuffer   = nullptr;
	pd.backBufferDS = nullptr;
	bgfx::setPlatformData(pd);
}

struct JoystickEvent
{
	uint32_t time;   /* event timestamp in milliseconds */
	int16_t  value;  /* value */
	uint8_t  type;   /* event type */
	uint8_t  number; /* axis/button number */
};

static gameplay::Input::Key __translateButton[] =
{
	gameplay::Input::Key::KEY_GAMEPAD_A,
	gameplay::Input::Key::KEY_GAMEPAD_B,
	gameplay::Input::Key::KEY_GAMEPAD_X,
	gameplay::Input::Key::KEY_GAMEPAD_Y,
	gameplay::Input::Key::KEY_GAMEPAD_SHOULDER_LEFT,
	gameplay::Input::Key::KEY_GAMEPAD_SHOULDER_RIGHT,
	gameplay::Input::Key::KEY_GAMEPAD_BACK,
	gameplay::Input::Key::KEY_GAMEPAD_START,
	gameplay::Input::Key::KEY_GAMEPAD_GUIDE,
	gameplay::Input::Key::KEY_GAMEPAD_THUMB_LEFT,
	gameplay::Input::Key::KEY_GAMEPAD_THUMB_RIGHT
};

static gameplay::Input::GamepadAxis __translateAxis[] =
{
	gameplay::Input::GAMEPAD_AXIS_LEFT_X,
	gameplay::Input::GAMEPAD_AXIS_LEFT_Y,
	gameplay::Input::GAMEPAD_AXIS_LEFT_Z,
	gameplay::Input::GAMEPAD_AXIS_RIGHT_X,
	gameplay::Input::GAMEPAD_AXIS_RIGHT_Y,
	gameplay::Input::GAMEPAD_AXIS_RIGHT_Z
};

struct AxisDpadRemap
{
	gameplay::Input::Key first;
	gameplay::Input::Key second;
};

static AxisDpadRemap __axisDpad[] =
{
	{ gameplay::Input::KEY_GAMEPAD_LEFT, gameplay::Input::KEY_GAMEPAD_RIGHT },
	{ gameplay::Input::KEY_GAMEPAD_UP,   gameplay::Input::KEY_GAMEPAD_DOWN  },
	{ gameplay::Input::KEY_NONE,         gameplay::Input::KEY_NONE        	},
	{ gameplay::Input::KEY_GAMEPAD_LEFT, gameplay::Input::KEY_GAMEPAD_RIGHT },
	{ gameplay::Input::KEY_GAMEPAD_UP,   gameplay::Input::KEY_GAMEPAD_DOWN  },
	{ gameplay::Input::KEY_NONE,         gameplay::Input::KEY_NONE         	},
};


struct Joystick
{
	Joystick()
		: fd(-1)
	{
	}

	void init()
	{
		fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);

		bx::memSet(value, 0, sizeof(value) );

		deadzone[gameplay::Input::GAMEPAD_AXIS_LEFT_X]  =
		deadzone[gameplay::Input::GAMEPAD_AXIS_LEFT_Y]  = 7849;
		deadzone[gameplay::Input::GAMEPAD_AXIS_RIGHT_X] =
		deadzone[gameplay::Input::GAMEPAD_AXIS_RIGHT_Y] = 8689;
		deadzone[gameplay::Input::GAMEPAD_AXIS_LEFT_Z]  =
		deadzone[gameplay::Input::GAMEPAD_AXIS_RIGHT_Z] = 30;
	}

	void shutdown()
	{
		if (-1 != fd)
		{
			close(fd);
		}
	}

	bool filter(gameplay::Input::GamepadAxis axis, int32_t* value)
	{
		const int32_t old = this->value[axis];
		const int32_t dz = deadzone[axis];
		int32_t new_value = *value;
		new_value = (new_value > dz) || ((new_value < -dz) ? new_value : 0);
		this->value[axis] = new_value;
		*value = new_value;
		return old != new_value;
	}

	bool update(gameplay::Platform::EventQueue& eventQueue)
	{
		if (fd == -1)
		{
			return false;
		}

		JoystickEvent event;
		int32_t bytes = read(fd, &event, sizeof(JoystickEvent));
		if (bytes != sizeof(JoystickEvent))
		{
			return false;
		}

		gameplay::Platform::WindowHandle windowHandle = { 0 };
		gameplay::Input::GamepadHandle gamepadHandle = { 0 };

		if (event.type & JS_EVENT_BUTTON)
		{
			if (event.number < BX_COUNTOF(__translateButton) )
			{
				eventQueue.postKeyEvent(windowHandle, __translateButton[event.number], 0, 0 != event.value);
			}
		}
		else if (event.type & JS_EVENT_AXIS)
		{
			if (event.number < BX_COUNTOF(__translateAxis) )
			{
				gameplay::Input::GamepadAxis axis = __translateAxis[event.number];
				int32_t value = event.value;
				if (filter(axis, &value) )
				{
					eventQueue.postAxisEvent(windowHandle, gamepadHandle, axis, value);

					if (__axisDpad[axis].first != gameplay::Input::KEY_NONE)
					{
						if (this->value[axis] == 0)
						{
							eventQueue.postKeyEvent(windowHandle, __axisDpad[axis].first,  0, false);
							eventQueue.postKeyEvent(windowHandle, __axisDpad[axis].second, 0, false);
						}
						else
						{
							eventQueue.postKeyEvent(windowHandle, this->value[axis] < 0 ? __axisDpad[axis].first : __axisDpad[axis].second, 0, true);
						}
					}

				}
			}
		}

		return true;
	}

	int fd;
	int32_t value[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
	int32_t deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
};


static Joystick __joystick;
static uint8_t __translateKey[512];

static void initTranslateKey(uint16_t xk, gameplay::Input::Key key)
{
	xk += 256;
	BX_CHECK(xk < BX_COUNTOF(__translateKey), "Out of bounds %d.", xk);
	__translateKey[xk&0x1ff] = (uint8_t)key;
}

gameplay::Input::Key fromXk(uint16_t xk)
{
	xk += 256;
	return 512 > xk ? (gameplay::Input::Key)__translateKey[xk] : gameplay::Input::KEY_NONE;
}

struct MainThreadEntry
{
	int32_t argc;
	const char* const* argv;
	static int32_t threadFunc(bx::Thread* thread, void* userData);
};


struct Msg
{
	Msg()
		: x(0)
		, y(0)
		, width(0)
		, height(0)
		, flags(0)
	{
	}

	int32_t  x;
	int32_t  y;
	uint32_t width;
	uint32_t height;
	uint32_t flags;
	std::string title;
};


struct Context
{
	Context()
		: keyModifiers(gameplay::Input::KEY_MODIFIER_NONE)
		, exit(false)
	{
		bx::memSet(__translateKey, 0, sizeof(__translateKey) );
		initTranslateKey(XK_Escape,       gameplay::Input::KEY_ESC);
		initTranslateKey(XK_Return,       gameplay::Input::KEY_RETURN);
		initTranslateKey(XK_Tab,          gameplay::Input::KEY_TAB);
		initTranslateKey(XK_BackSpace,    gameplay::Input::KEY_BACKSPACE);
		initTranslateKey(XK_space,        gameplay::Input::KEY_SPACE);
		initTranslateKey(XK_Up,           gameplay::Input::KEY_UP);
		initTranslateKey(XK_Down,         gameplay::Input::KEY_DOWN);
		initTranslateKey(XK_Left,         gameplay::Input::KEY_LEFT);
		initTranslateKey(XK_Right,        gameplay::Input::KEY_RIGHT);
		initTranslateKey(XK_Insert,       gameplay::Input::KEY_INSERT);
		initTranslateKey(XK_Delete,       gameplay::Input::KEY_DELETE);
		initTranslateKey(XK_Home,         gameplay::Input::KEY_HOME);
		initTranslateKey(XK_KP_End,       gameplay::Input::KEY_END);
		initTranslateKey(XK_Page_Up,      gameplay::Input::KEY_PAGE_UP);
		initTranslateKey(XK_Page_Down,    gameplay::Input::KEY_PAGE_DOWN);
		initTranslateKey(XK_Print,        gameplay::Input::KEY_PRINT);
		initTranslateKey(XK_equal,        gameplay::Input::KEY_PLUS);
		initTranslateKey(XK_minus,        gameplay::Input::KEY_MINUS);
		initTranslateKey(XK_bracketleft,  gameplay::Input::KEY_LEFT_BRACKET);
		initTranslateKey(XK_bracketright, gameplay::Input::KEY_RIGHT_BRACKET);
		initTranslateKey(XK_semicolon,    gameplay::Input::KEY_SEMICOLON);
		initTranslateKey(XK_apostrophe,   gameplay::Input::KEY_QUOTE);
		initTranslateKey(XK_comma,        gameplay::Input::KEY_COMMA);
		initTranslateKey(XK_period,       gameplay::Input::KEY_PERIOD);
		initTranslateKey(XK_slash,        gameplay::Input::KEY_SLASH);
		initTranslateKey(XK_backslash,    gameplay::Input::KEY_BACKSLASH);
		initTranslateKey(XK_grave,        gameplay::Input::KEY_TILDE);
		initTranslateKey(XK_F1,           gameplay::Input::KEY_F1);
		initTranslateKey(XK_F2,           gameplay::Input::KEY_F2);
		initTranslateKey(XK_F3,           gameplay::Input::KEY_F3);
		initTranslateKey(XK_F4,           gameplay::Input::KEY_F4);
		initTranslateKey(XK_F5,           gameplay::Input::KEY_F5);
		initTranslateKey(XK_F6,           gameplay::Input::KEY_F6);
		initTranslateKey(XK_F7,           gameplay::Input::KEY_F7);
		initTranslateKey(XK_F8,           gameplay::Input::KEY_F8);
		initTranslateKey(XK_F9,           gameplay::Input::KEY_F9);
		initTranslateKey(XK_F10,          gameplay::Input::KEY_F10);
		initTranslateKey(XK_F11,          gameplay::Input::KEY_F11);
		initTranslateKey(XK_F12,          gameplay::Input::KEY_F12);
		initTranslateKey(XK_KP_Insert,    gameplay::Input::KEY_NUMPAD0);
		initTranslateKey(XK_KP_End,       gameplay::Input::KEY_NUMPAD1);
		initTranslateKey(XK_KP_Down,      gameplay::Input::KEY_NUMPAD2);
		initTranslateKey(XK_KP_Page_Down, gameplay::Input::KEY_NUMPAD3);
		initTranslateKey(XK_KP_Left,      gameplay::Input::KEY_NUMPAD4);
		initTranslateKey(XK_KP_Begin,     gameplay::Input::KEY_NUMPAD5);
		initTranslateKey(XK_KP_Right,     gameplay::Input::KEY_NUMPAD6);
		initTranslateKey(XK_KP_Home,      gameplay::Input::KEY_NUMPAD7);
		initTranslateKey(XK_KP_Up,        gameplay::Input::KEY_NUMPAD8);
		initTranslateKey(XK_KP_Page_Up,   gameplay::Input::KEY_NUMPAD9);
		initTranslateKey('0',             gameplay::Input::KEY_0);
		initTranslateKey('1',             gameplay::Input::KEY_1);
		initTranslateKey('2',             gameplay::Input::KEY_2);
		initTranslateKey('3',             gameplay::Input::KEY_3);
		initTranslateKey('4',             gameplay::Input::KEY_4);
		initTranslateKey('5',             gameplay::Input::KEY_5);
		initTranslateKey('6',             gameplay::Input::KEY_6);
		initTranslateKey('7',             gameplay::Input::KEY_7);
		initTranslateKey('8',             gameplay::Input::KEY_8);
		initTranslateKey('9',             gameplay::Input::KEY_9);
		initTranslateKey('a',             gameplay::Input::KEY_A);
		initTranslateKey('b',             gameplay::Input::KEY_B);
		initTranslateKey('c',             gameplay::Input::KEY_C);
		initTranslateKey('d',             gameplay::Input::KEY_D);
		initTranslateKey('e',             gameplay::Input::KEY_E);
		initTranslateKey('f',             gameplay::Input::KEY_F);
		initTranslateKey('g',             gameplay::Input::KEY_G);
		initTranslateKey('h',             gameplay::Input::KEY_H);
		initTranslateKey('i',             gameplay::Input::KEY_I);
		initTranslateKey('j',             gameplay::Input::KEY_J);
		initTranslateKey('k',             gameplay::Input::KEY_K);
		initTranslateKey('l',             gameplay::Input::KEY_L);
		initTranslateKey('m',             gameplay::Input::KEY_M);
		initTranslateKey('n',             gameplay::Input::KEY_N);
		initTranslateKey('o',             gameplay::Input::KEY_O);
		initTranslateKey('p',             gameplay::Input::KEY_P);
		initTranslateKey('q',             gameplay::Input::KEY_Q);
		initTranslateKey('r',             gameplay::Input::KEY_R);
		initTranslateKey('s',             gameplay::Input::KEY_S);
		initTranslateKey('t',             gameplay::Input::KEY_T);
		initTranslateKey('u',             gameplay::Input::KEY_U);
		initTranslateKey('v',             gameplay::Input::KEY_V);
		initTranslateKey('w',             gameplay::Input::KEY_W);
		initTranslateKey('x',             gameplay::Input::KEY_X);
		initTranslateKey('y',             gameplay::Input::KEY_Y);
		initTranslateKey('z',             gameplay::Input::KEY_Z);

		mx = 0;
		my = 0;
		mz = 0;
	}

	int32_t run(int argc, const char* const* argv)
	{
		XInitThreads();
		display = XOpenDisplay(0);

		int32_t screen = DefaultScreen(display);
		depth  = DefaultDepth(display, screen);
		visual = DefaultVisual(display, screen);
		root   = RootWindow(display, screen);

		bx::memSet(&windowAttrs, 0, sizeof(windowAttrs));
		windowAttrs.background_pixmap = 0;
		windowAttrs.border_pixel = 0;
		windowAttrs.event_mask = 0
				| ButtonPressMask
				| ButtonReleaseMask
				| ExposureMask
				| KeyPressMask
				| KeyReleaseMask
				| PointerMotionMask
				| StructureNotifyMask
				;

		windowAlloc.alloc();
		this->window[0] = XCreateWindow(display,
										root,
										0, 0, 1, 1, 0,
										depth,
										InputOutput,
										visual,
										CWBorderPixel|CWEventMask, 
										&windowAttrs);

		// Clear window to black.
		XSetWindowAttributes attr;
		bx::memSet(&attr, 0, sizeof(attr) );
		XChangeWindowAttributes(display, this->window[0], CWBackPixel, &attr);

		const char* wmDeleteWindowName = "WM_DELETE_WINDOW";
		Atom wmDeleteWindow;
		XInternAtoms(display, (char **)&wmDeleteWindowName, 1, False, &wmDeleteWindow);
		XSetWMProtocols(display, this->window[0], &wmDeleteWindow, 1);

		XMapWindow(display, this->window[0]);
		XStoreName(display, this->window[0], GP_ENGINE_NAME);

		XClassHint* hint = XAllocClassHint();
		hint->res_name  = (char*)GP_ENGINE_NAME;
		hint->res_class = (char*)GP_ENGINE_NAME;
		XSetClassHint(display, this->window[0], hint);
		XFree(hint);

		XIM im;
		im = XOpenIM(display, NULL, NULL, NULL);

		XIC ic;
		ic = XCreateIC(im, XNInputStyle, 0
					 | XIMPreeditNothing
					 | XIMStatusNothing, 
					   XNClientWindow, this->window[0], NULL);

		setX11Window(display, this->window[0]);

		MainThreadEntry mte;
		mte.argc = argc;
		mte.argv = argv;
		bx::Thread thread;
		thread.init(mte.threadFunc, &mte);

		gameplay::Platform::WindowHandle defaultWindow = { 0 };
		eventQueue.postSizeEvent(defaultWindow, 1, 1);

		__joystick.init();

		while (!exit)
		{
			bool joystick = __joystick.update(eventQueue);
			bool xpending = XPending(display);

			if (!xpending)
			{
				bx::sleep(joystick ? 8 : 16);
			}
			else
			{
				XEvent event;
				XNextEvent(display, &event);

				switch (event.type)
				{
					case Expose:
						break;

					case ClientMessage:
						if ( (Atom)event.xclient.data.l[0] == wmDeleteWindow)
						{
							eventQueue.postExitEvent();
						}
						break;

					case ButtonPress:
					case ButtonRelease:
						{
							const XButtonEvent& xbutton = event.xbutton;
							gameplay::Input::MouseButton mb = gameplay::Input::MOUSE_BUTTON_NONE;
							switch (xbutton.button)
							{
								case Button1: 
									mb = gameplay::Input::MOUSE_BUTTON_LEFT;
									break;
								case Button2:
									mb = gameplay::Input::MOUSE_BUTTON_MIDDLE; 
									break;
								case Button3:
									mb = gameplay::Input::MOUSE_BUTTON_RIGHT;
									break;
								case Button4: 
									++mz; 
									break;
								case Button5:
									--mz;
									break;
							}

							gameplay::Platform::WindowHandle handle = findHandle(xbutton.window);
							if (mb != gameplay::Input::MOUSE_BUTTON_NONE)
							{
								eventQueue.postMouseEvent(handle, 
														  xbutton.x, 
														  xbutton.y, 
														  mz,
														  mb, 
														  event.type == ButtonPress);
							}
							else
							{
								eventQueue.postMouseEvent(handle, mx, my, mz);
							}
						}
						break;

					case MotionNotify:
						{
							const XMotionEvent& xmotion = event.xmotion;
							gameplay::Platform::WindowHandle handle = findHandle(xmotion.window);

							mx = xmotion.x;
							my = xmotion.y;

							eventQueue.postMouseEvent(handle, mx, my, mz);
						}
						break;

					case KeyPress:
					case KeyRelease:
						{
							XKeyEvent& xkey = event.xkey;
							KeySym keysym = XLookupKeysym(&xkey, 0);
							switch (keysym)
							{
							case XK_Meta_L:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_LEFT_META,   KeyPress == event.type);
								break;
							case XK_Meta_R:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_RIGHT_META,  KeyPress == event.type);
								break;
							case XK_Control_L:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_LEFT_CTRL,   KeyPress == event.type);
								break;
							case XK_Control_R:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_RIGHT_CTRL,  KeyPress == event.type);
								break;
							case XK_Shift_L:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_LEFT_SHIFT,  KeyPress == event.type);
								break;
							case XK_Shift_R:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_RIGHT_SHIFT, KeyPress == event.type);
								break;
							case XK_Alt_L:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_LEFT_ALT,    KeyPress == event.type);
								break;
							case XK_Alt_R:
								setKeyModifier(gameplay::Input::KEY_MODIFIER_RIGHT_ALT,   KeyPress == event.type);
								break;
							default:
								{
									gameplay::Platform::WindowHandle handle = findHandle(xkey.window);
									if (KeyPress == event.type)
									{
										Status status = 0;
										uint8_t utf8[4];
										int len = Xutf8LookupString(ic, &xkey, (char*)utf8, sizeof(utf8), &keysym, &status);
										switch (status)
										{
										case XLookupChars:
										case XLookupBoth:
											if (len != 0)
											{
												eventQueue.postCharEvent(handle, len, utf8);
											}
											break;

										default:
											break;
										}
									}

									gameplay::Input::Key key = fromXk(keysym);
									if (key != gameplay::Input::KEY_NONE)
									{
										eventQueue.postKeyEvent(handle, key, keyModifiers, KeyPress == event.type);
									}
								}
								break;
							}
						}
						break;

					case ConfigureNotify:
						{
							const XConfigureEvent& xev = event.xconfigure;
							gameplay::Platform::WindowHandle handle = findHandle(xev.window);
							if (gameplay::Platform::isWindowValid(handle))
							{
								eventQueue.postSizeEvent(handle, xev.width, xev.height);
							}
						}
						break;
				}
			}
		}

		thread.shutdown();

		__joystick.shutdown();

		XDestroyIC(ic);
		XCloseIM(im);

		XUnmapWindow(display, this->window[0]);
		XDestroyWindow(display, this->window[0]);

		return thread.getExitCode();
	}

	void setKeyModifier(gameplay::Input::KeyModifier keyModifier, bool set)
	{
		keyModifiers &= ~keyModifier;
		keyModifiers |= set ? keyModifier : 0;
	}

	void createWindow(gameplay::Platform::WindowHandle handle, Msg* msg)
	{
		Window window = XCreateWindow(display, 
									  root, 
									  msg->x, 
									  msg->y, 
									  msg->width, 
									  msg->height, 0, 
									  depth, 
									  InputOutput, 
									  visual, 
									  CWBorderPixel|CWEventMask, 
									  &windowAttrs);
		this->window[handle.idx] = window;

		// Clear window to black.
		XSetWindowAttributes attr;
		bx::memSet(&attr, 0, sizeof(attr));
		XChangeWindowAttributes(display, window, CWBackPixel, &attr);

		const char* wmDeleteWindowName = "WM_DELETE_WINDOW";
		Atom wmDeleteWindow;
		XInternAtoms(display, (char **)&wmDeleteWindowName, 1, False, &wmDeleteWindow);
		XSetWMProtocols(display, window, &wmDeleteWindow, 1);

		XMapWindow(display, window);
		XStoreName(display, window, msg->title.c_str());

		XClassHint* hint = XAllocClassHint();
		hint->res_name  = (char*)msg->title.c_str();
		hint->res_class = (char*)GP_ENGINE_NAME;
		XSetClassHint(display, window, hint);
		XFree(hint);

		eventQueue.postSizeEvent(handle, msg->width, msg->height);

		union cast
		{
			void* p;
			::Window w;
		};

		cast c;
		c.w = window;
		eventQueue.postWindowEvent(handle, c.p);

		delete msg;
	}

	gameplay::Platform::WindowHandle findHandle(Window window)
	{
		bx::MutexScope scope(lock);
		for (uint32_t i = 0, num = windowAlloc.getNumHandles(); i < num; ++i)
		{
			uint16_t idx = windowAlloc.getHandleAt(i);
			if (window == this->window[idx])
			{
				gameplay::Platform::WindowHandle handle = { idx };
				return handle;
			}
		}

		gameplay::Platform::WindowHandle invalid = { UINT16_MAX };
		return invalid;
	}

	gameplay::Platform::EventQueue eventQueue;
	bx::Mutex lock;
	bx::HandleAllocT<GP_WINDOWS_MAX> windowAlloc;
	Visual* visual;
	Window root;
	XSetWindowAttributes windowAttrs;
	Display* display;
	Window window[GP_WINDOWS_MAX];
	uint32_t flags[GP_WINDOWS_MAX];
	int32_t mx;
	int32_t my;
	int32_t mz;
	int32_t depth;
	uint8_t keyModifiers;
	bool exit;
};


static Context __ctx;


extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

const gameplay::Platform::Event* poll()
{
	return __ctx.eventQueue.poll();
}

const gameplay::Platform::Event* poll(Platform::WindowHandle handle)
{
	return __ctx.eventQueue.poll(handle);
}

void release(const gameplay::Platform::Event* evt)
{
	__ctx.eventQueue.release(evt);
}

gameplay::Platform::WindowHandle createWindow(int32_t x,
											  int32_t y,
											  uint32_t width,
											  uint32_t height,
											  uint32_t flags,
											  const char* title)
{
	bx::MutexScope scope(__ctx.lock);
	gameplay::Platform::WindowHandle handle = { __ctx.windowAlloc.alloc() };

	if (gameplay::Platform::isWindowValid(handle))
	{
		Msg* msg = new Msg;
		msg->x = x;
		msg->y = y;
		msg->width = width;
		msg->height = height;
		msg->title = title;
		msg->flags = flags;
		__ctx.createWindow(handle, msg);
	}
	return handle;
}

void destroyWindow(gameplay::Platform::WindowHandle handle)
{
	if (gameplay::Platform::isWindowValid(handle))
	{
		__ctx.eventQueue.postWindowEvent(handle, nullptr);
		XUnmapWindow(__ctx.display, __ctx.window[handle.idx]);
		XDestroyWindow(__ctx.display, __ctx.window[handle.idx]);

		bx::MutexScope scope(__ctx.lock);
		__ctx.windowAlloc.free(handle.idx);
	}
}

void setWindowPos(gameplay::Platform::WindowHandle handle, int32_t x, int32_t y)
{
	Display* display = __ctx.display;
	Window   window  = __ctx.window[handle.idx];
	XMoveWindow(display, window, x, y);
}

void setWindowSize(gameplay::Platform::WindowHandle handle, uint32_t width, uint32_t height)
{
	Display* display = __ctx.display;
	Window   window  = __ctx.window[handle.idx];
	XResizeWindow(display, window, int32_t(width), int32_t(height) );
}

void setWindowTitle(gameplay::Platform::WindowHandle handle, const char* title)
{
	Display* display = __ctx.display;
	Window window  = __ctx.window[handle.idx];
	XStoreName(display, window, title);
}

void setWindowFlags(gameplay::Platform::WindowHandle handle, uint32_t flags, bool enabled)
{
	BX_UNUSED(handle, flags, enabled);
}

void toggleFullscreen(gameplay::Platform::WindowHandle handle)
{
	BX_UNUSED(handle);
}

void setMouseLock(gameplay::Platform::WindowHandle handle, bool lock)
{
	BX_UNUSED(handle, lock);
}

int32_t MainThreadEntry::threadFunc(bx::Thread*, void* userData)
{
	MainThreadEntry* self = (MainThreadEntry*)userData;
	int32_t result = main(self->argc, self->argv);
	__ctx.exit = true;
	return result;
}

}


int main(int argc, const char* const* argv)
{
	using namespace gameplay;
	return __ctx.run(argc, argv);
}

#endif