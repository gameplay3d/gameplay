#include "Base.h"
#ifdef GP_PLATFORM_WINDOWS
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

#include <windows.h>
#include <windowsx.h>
#include <xinput.h>

#ifndef XINPUT_GAMEPAD_GUIDE
#	define XINPUT_GAMEPAD_GUIDE 0x400
#endif
#ifndef XINPUT_DLL_A
#	define XINPUT_DLL_A "xinput.dll"
#endif

namespace gameplay
{

void setHwnd(::HWND window)
{
	bgfx::PlatformData pd;
	bx::memSet(&pd, 0, sizeof(pd));
	pd.nwh = window;
	bgfx::setPlatformData(pd);
}

typedef DWORD (WINAPI* PFN_XINPUT_GET_STATE)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef void  (WINAPI* PFN_XINPUT_ENABLE)(BOOL enable);

struct XInputRemap
{
	uint16_t  bit;
	gameplay::Input::Key key;
};

static XInputRemap __xinputRemap[] =
{
	{ XINPUT_GAMEPAD_DPAD_UP,        gameplay::Input::Key::KEY_GAMEPAD_UP				},
	{ XINPUT_GAMEPAD_DPAD_DOWN,      gameplay::Input::Key::KEY_GAMEPAD_DOWN				},
	{ XINPUT_GAMEPAD_DPAD_LEFT,      gameplay::Input::Key::KEY_GAMEPAD_LEFT				},
	{ XINPUT_GAMEPAD_DPAD_RIGHT,     gameplay::Input::Key::KEY_GAMEPAD_RIGHT			},
	{ XINPUT_GAMEPAD_START,          gameplay::Input::Key::KEY_GAMEPAD_START			},
	{ XINPUT_GAMEPAD_BACK,           gameplay::Input::Key::KEY_GAMEPAD_BACK				},
	{ XINPUT_GAMEPAD_LEFT_THUMB,     gameplay::Input::Key::KEY_GAMEPAD_THUMB_LEFT		},
	{ XINPUT_GAMEPAD_RIGHT_THUMB,    gameplay::Input::Key::KEY_GAMEPAD_THUMB_RIGHT		},
	{ XINPUT_GAMEPAD_LEFT_SHOULDER,  gameplay::Input::Key::KEY_GAMEPAD_SHOULDER_LEFT	},
	{ XINPUT_GAMEPAD_RIGHT_SHOULDER, gameplay::Input::Key::KEY_GAMEPAD_SHOULDER_RIGHT	},
	{ XINPUT_GAMEPAD_GUIDE,          gameplay::Input::Key::KEY_GAMEPAD_GUIDE			},
	{ XINPUT_GAMEPAD_A,              gameplay::Input::Key::KEY_GAMEPAD_A				},
	{ XINPUT_GAMEPAD_B,              gameplay::Input::Key::KEY_GAMEPAD_B				},
	{ XINPUT_GAMEPAD_X,              gameplay::Input::Key::KEY_GAMEPAD_X				},
	{ XINPUT_GAMEPAD_Y,              gameplay::Input::Key::KEY_GAMEPAD_Y				}
};


struct XInput
{
	XInput()
		: xinputdll(NULL)
	{
		bx::memSet(connected, 0, sizeof(connected) );
		bx::memSet(state, 0, sizeof(state) );

		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_X ] =
		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Y] = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_X] =
		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Y] = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Z ] =
		deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Z] = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

		bx::memSet(flip, 1, sizeof(flip) );
		flip[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Y] =
		flip[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Y] = -1;
	}

	void init()
	{
		xinputdll = bx::dlopen(XINPUT_DLL_A);

		if (xinputdll != NULL)
		{
			if (XInputGetState == NULL)
			{
				shutdown();
			}
		}
	}

	void shutdown()
	{
		if (xinputdll != NULL)
		{
			bx::dlclose(xinputdll);
			xinputdll = NULL;
		}
	}

	bool filter(gameplay::Input::GamepadAxis axis, int32_t old, int32_t* value)
	{
		const int32_t dz = deadzone[axis];
		int32_t new_value = *value;
		new_value = (new_value > dz) || ((new_value < -dz) ? new_value : 0);
		*value = new_value * flip[axis];
		return old != new_value;
	}

	void update(gameplay::Platform::EventQueue& eventQueue)
	{
		int64_t now = bx::getHPCounter();
		static int64_t next = now;

		if (now < next)
		{
			return;
		}

		const int64_t timerFreq = bx::getHPFrequency();
		next = now + timerFreq/60;

		if (xinputdll == NULL)
		{
			return;
		}

		gameplay::Platform::WindowHandle defaultWindow = { 0 };

		for (uint16_t i = 0; i < BX_COUNTOF(this->state); ++i)
		{
			XINPUT_STATE state;
			DWORD result = XInputGetState(i, &state);

			gameplay::Input::GamepadHandle gamepadHandle = { i };

			bool connected = ERROR_SUCCESS == result;
			if (connected != this->connected[i])
			{
				eventQueue.postGamepadEvent(defaultWindow, gamepadHandle, connected);
			}

			this->connected[i] = connected;

			if (connected &&  this->state[i].dwPacketNumber != state.dwPacketNumber)
			{
				XINPUT_GAMEPAD& gamepad = this->state[i].Gamepad;
				const uint16_t changed = gamepad.wButtons ^ state.Gamepad.wButtons;
				const uint16_t current = gamepad.wButtons;
				if (changed != 0)
				{
					for (uint32_t j = 0; j < BX_COUNTOF(__xinputRemap); ++j)
					{
						uint16_t bit = __xinputRemap[j].bit;
						if (bit & changed)
						{
							eventQueue.postKeyEvent(defaultWindow, __xinputRemap[j].key, 0, 0 == (current & bit) );
						}
					}
					gamepad.wButtons = state.Gamepad.wButtons;
				}

				if (gamepad.bLeftTrigger != state.Gamepad.bLeftTrigger)
				{
					int32_t value = state.Gamepad.bLeftTrigger;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Z, gamepad.bLeftTrigger, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Z, value);
					}
					gamepad.bLeftTrigger = state.Gamepad.bLeftTrigger;
				}

				if (gamepad.bRightTrigger != state.Gamepad.bRightTrigger)
				{
					int32_t value = state.Gamepad.bRightTrigger;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Z, gamepad.bRightTrigger, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Z, value);
					}
					gamepad.bRightTrigger = state.Gamepad.bRightTrigger;
				}

				if (gamepad.sThumbLX != state.Gamepad.sThumbLX)
				{
					int32_t value = state.Gamepad.sThumbLX;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_X, gamepad.sThumbLX, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_X, value);
					}
					gamepad.sThumbLX = state.Gamepad.sThumbLX;
				}

				if (gamepad.sThumbLY != state.Gamepad.sThumbLY)
				{
					int32_t value = state.Gamepad.sThumbLY;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Y, gamepad.sThumbLY, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_LEFT_Y, value);
					}
					gamepad.sThumbLY = state.Gamepad.sThumbLY;
				}

				if (gamepad.sThumbRX != state.Gamepad.sThumbRX)
				{
					int32_t value = state.Gamepad.sThumbRX;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_X, gamepad.sThumbRX, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_X, value);
					}
					gamepad.sThumbRX = state.Gamepad.sThumbRX;
				}

				if (gamepad.sThumbRY != state.Gamepad.sThumbRY)
				{
					int32_t value = state.Gamepad.sThumbRY;
					if (filter(gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Y, gamepad.sThumbRY, &value) )
					{
						eventQueue.postAxisEvent(defaultWindow, gamepadHandle, gameplay::Input::GamepadAxis::GAMEPAD_AXIS_RIGHT_Y, value);
					}
					gamepad.sThumbRY = state.Gamepad.sThumbRY;
				}
			}
		}
	}

	void* xinputdll;
	int32_t deadzone[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
	int8_t flip[gameplay::Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
	XINPUT_STATE state[GP_GAMEPADS_MAX];
	bool connected[GP_GAMEPADS_MAX];
};

XInput __xinput;

enum
{
	WM_USER_WINDOW_CREATE = WM_USER,
	WM_USER_WINDOW_DESTROY,
	WM_USER_WINDOW_SET_TITLE,
	WM_USER_WINDOW_SET_FLAGS,
	WM_USER_WINDOW_SET_POS,
	WM_USER_WINDOW_SET_SIZE,
	WM_USER_WINDOW_TOGGLE_FRAME,
	WM_USER_WINDOW_MOUSE_LOCK,
};

struct TranslateKeyModifiers
{
	int vk;
	gameplay::Input::KeyModifier keyModifier;
};

struct MainThreadEntry
{
	int argc;
	const char* const* argv;

	static int32_t threadFunc(bx::Thread* thread, void* userData);
};

static const TranslateKeyModifiers __translateKeyModifiers[8] =
{
	{ VK_LMENU,    gameplay::Input::KeyModifier::KEY_MODIFIER_LEFT_ALT    },
	{ VK_RMENU,    gameplay::Input::KeyModifier::KEY_MODIFIER_RIGHT_ALT   },
	{ VK_LCONTROL, gameplay::Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL   },
	{ VK_RCONTROL, gameplay::Input::KeyModifier::KEY_MODIFIER_RIGHT_CTRL  },
	{ VK_LSHIFT,   gameplay::Input::KeyModifier::KEY_MODIFIER_LEFT_SHIFT  },
	{ VK_RSHIFT,   gameplay::Input::KeyModifier::KEY_MODIFIER_RIGHT_SHIFT },
	{ VK_LWIN,     gameplay::Input::KeyModifier::KEY_MODIFIER_LEFT_META   },
	{ VK_RWIN,     gameplay::Input::KeyModifier::KEY_MODIFIER_RIGHT_META  },
};

static uint8_t translateKeyModifiers()
{
	uint8_t modifiers = 0;
	for (uint32_t i = 0; i < BX_COUNTOF(__translateKeyModifiers); ++i)
	{
		const TranslateKeyModifiers& tkm = __translateKeyModifiers[i];
		modifiers |= 0 > GetKeyState(tkm.vk) ? tkm.keyModifier : gameplay::Input::KeyModifier::KEY_MODIFIER_NONE;
	}
	return modifiers;
}

static uint8_t __translateKey[256];

static gameplay::Input::Key translateKey(WPARAM wparam)
{
	return ( gameplay::Input::Key )__translateKey[wparam & 0xff];
}

struct Msg
{
	Msg()
		: x(0),
		  y(0),
		  width(0),
		  height(0),
		  flags(0),
		  flagsEnabled(false)
	{
	}

	int32_t  x;
	int32_t  y;
	uint32_t width;
	uint32_t height;
	uint32_t flags;
	bool flagsEnabled;
	std::string title;
};


static void mouseCapture(HWND hwnd, bool capture)
{
	if (capture)
	{
		SetCapture(hwnd);
	}
	else
	{
		ReleaseCapture();
	}
}


struct Context
{
	Context()
		: mz(0), 
		  frame(true), 
		  mouseLock(NULL), 
		  init(false), 
		  exit(false)
	{
		bx::memSet(__translateKey, 0, sizeof(__translateKey));
		__translateKey[VK_ESCAPE]     = gameplay::Input::Key::KEY_ESC;
		__translateKey[VK_RETURN]     = gameplay::Input::Key::KEY_RETURN;
		__translateKey[VK_TAB]        = gameplay::Input::Key::KEY_TAB;
		__translateKey[VK_BACK]       = gameplay::Input::Key::KEY_BACKSPACE;
		__translateKey[VK_SPACE]      = gameplay::Input::Key::KEY_SPACE;
		__translateKey[VK_UP]         = gameplay::Input::Key::KEY_UP;
		__translateKey[VK_DOWN]       = gameplay::Input::Key::KEY_DOWN;
		__translateKey[VK_LEFT]       = gameplay::Input::Key::KEY_LEFT;
		__translateKey[VK_RIGHT]      = gameplay::Input::Key::KEY_RIGHT;
		__translateKey[VK_INSERT]     = gameplay::Input::Key::KEY_INSERT;
		__translateKey[VK_DELETE]     = gameplay::Input::Key::KEY_DELETE;
		__translateKey[VK_HOME]       = gameplay::Input::Key::KEY_HOME;
		__translateKey[VK_END]        = gameplay::Input::Key::KEY_END;
		__translateKey[VK_PRIOR]      = gameplay::Input::Key::KEY_PAGE_UP;
		__translateKey[VK_NEXT]       = gameplay::Input::Key::KEY_PAGE_DOWN;
		__translateKey[VK_SNAPSHOT]   = gameplay::Input::Key::KEY_PRINT;
		__translateKey[VK_OEM_PLUS]   = gameplay::Input::Key::KEY_PLUS;
		__translateKey[VK_OEM_MINUS]  = gameplay::Input::Key::KEY_MINUS;
		__translateKey[VK_OEM_4]      = gameplay::Input::Key::KEY_LEFT_BRACKET;
		__translateKey[VK_OEM_6]      = gameplay::Input::Key::KEY_RIGHT_BRACKET;
		__translateKey[VK_OEM_1]      = gameplay::Input::Key::KEY_SEMICOLON;
		__translateKey[VK_OEM_7]      = gameplay::Input::Key::KEY_QUOTE;
		__translateKey[VK_OEM_COMMA]  = gameplay::Input::Key::KEY_COMMA;
		__translateKey[VK_OEM_PERIOD] = gameplay::Input::Key::KEY_PERIOD;
		__translateKey[VK_DECIMAL]    = gameplay::Input::Key::KEY_PERIOD;
		__translateKey[VK_OEM_2]      = gameplay::Input::Key::KEY_SLASH;
		__translateKey[VK_OEM_5]      = gameplay::Input::Key::KEY_BACKSLASH;
		__translateKey[VK_OEM_3]      = gameplay::Input::Key::KEY_TILDE;
		__translateKey[VK_F1]         = gameplay::Input::Key::KEY_F1;
		__translateKey[VK_F2]         = gameplay::Input::Key::KEY_F2;
		__translateKey[VK_F3]         = gameplay::Input::Key::KEY_F3;
		__translateKey[VK_F4]         = gameplay::Input::Key::KEY_F4;
		__translateKey[VK_F5]         = gameplay::Input::Key::KEY_F5;
		__translateKey[VK_F6]         = gameplay::Input::Key::KEY_F6;
		__translateKey[VK_F7]         = gameplay::Input::Key::KEY_F7;
		__translateKey[VK_F8]         = gameplay::Input::Key::KEY_F8;
		__translateKey[VK_F9]         = gameplay::Input::Key::KEY_F9;
		__translateKey[VK_F10]        = gameplay::Input::Key::KEY_F10;
		__translateKey[VK_F11]        = gameplay::Input::Key::KEY_F11;
		__translateKey[VK_F12]        = gameplay::Input::Key::KEY_F12;
		__translateKey[VK_NUMPAD0]    = gameplay::Input::Key::KEY_NUMPAD0;
		__translateKey[VK_NUMPAD1]    = gameplay::Input::Key::KEY_NUMPAD1;
		__translateKey[VK_NUMPAD2]    = gameplay::Input::Key::KEY_NUMPAD2;
		__translateKey[VK_NUMPAD3]    = gameplay::Input::Key::KEY_NUMPAD3;
		__translateKey[VK_NUMPAD4]    = gameplay::Input::Key::KEY_NUMPAD4;
		__translateKey[VK_NUMPAD5]    = gameplay::Input::Key::KEY_NUMPAD5;
		__translateKey[VK_NUMPAD6]    = gameplay::Input::Key::KEY_NUMPAD6;
		__translateKey[VK_NUMPAD7]    = gameplay::Input::Key::KEY_NUMPAD7;
		__translateKey[VK_NUMPAD8]    = gameplay::Input::Key::KEY_NUMPAD8;
		__translateKey[VK_NUMPAD9]    = gameplay::Input::Key::KEY_NUMPAD9;
		__translateKey[uint8_t('0')]  = gameplay::Input::Key::KEY_0;
		__translateKey[uint8_t('1')]  = gameplay::Input::Key::KEY_1;
		__translateKey[uint8_t('2')]  = gameplay::Input::Key::KEY_2;
		__translateKey[uint8_t('3')]  = gameplay::Input::Key::KEY_3;
		__translateKey[uint8_t('4')]  = gameplay::Input::Key::KEY_4;
		__translateKey[uint8_t('5')]  = gameplay::Input::Key::KEY_5;
		__translateKey[uint8_t('6')]  = gameplay::Input::Key::KEY_6;
		__translateKey[uint8_t('7')]  = gameplay::Input::Key::KEY_7;
		__translateKey[uint8_t('8')]  = gameplay::Input::Key::KEY_8;
		__translateKey[uint8_t('9')]  = gameplay::Input::Key::KEY_9;
		__translateKey[uint8_t('A')]  = gameplay::Input::Key::KEY_A;
		__translateKey[uint8_t('B')]  = gameplay::Input::Key::KEY_B;
		__translateKey[uint8_t('C')]  = gameplay::Input::Key::KEY_C;
		__translateKey[uint8_t('D')]  = gameplay::Input::Key::KEY_D;
		__translateKey[uint8_t('E')]  = gameplay::Input::Key::KEY_E;
		__translateKey[uint8_t('F')]  = gameplay::Input::Key::KEY_F;
		__translateKey[uint8_t('G')]  = gameplay::Input::Key::KEY_G;
		__translateKey[uint8_t('H')]  = gameplay::Input::Key::KEY_H;
		__translateKey[uint8_t('I')]  = gameplay::Input::Key::KEY_I;
		__translateKey[uint8_t('J')]  = gameplay::Input::Key::KEY_J;
		__translateKey[uint8_t('K')]  = gameplay::Input::Key::KEY_K;
		__translateKey[uint8_t('L')]  = gameplay::Input::Key::KEY_L;
		__translateKey[uint8_t('M')]  = gameplay::Input::Key::KEY_M;
		__translateKey[uint8_t('N')]  = gameplay::Input::Key::KEY_N;
		__translateKey[uint8_t('O')]  = gameplay::Input::Key::KEY_O;
		__translateKey[uint8_t('P')]  = gameplay::Input::Key::KEY_P;
		__translateKey[uint8_t('Q')]  = gameplay::Input::Key::KEY_Q;
		__translateKey[uint8_t('R')]  = gameplay::Input::Key::KEY_R;
		__translateKey[uint8_t('S')]  = gameplay::Input::Key::KEY_S;
		__translateKey[uint8_t('T')]  = gameplay::Input::Key::KEY_T;
		__translateKey[uint8_t('U')]  = gameplay::Input::Key::KEY_U;
		__translateKey[uint8_t('V')]  = gameplay::Input::Key::KEY_V;
		__translateKey[uint8_t('W')]  = gameplay::Input::Key::KEY_W;
		__translateKey[uint8_t('X')]  = gameplay::Input::Key::KEY_X;
		__translateKey[uint8_t('Y')]  = gameplay::Input::Key::KEY_Y;
		__translateKey[uint8_t('Z')]  = gameplay::Input::Key::KEY_Z;
	}

	int32_t run(int argc, const char* const* argv)
	{
		SetDllDirectoryA(".");

		__xinput.init();

		HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

		WNDCLASSEXA wnd;
		bx::memSet(&wnd, 0, sizeof(wnd) );
		wnd.cbSize = sizeof(wnd);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = wndProc;
		wnd.hInstance = instance;
		wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.lpszClassName = "gameplay";
		wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassExA(&wnd);

		windowAlloc.alloc();
		hwnd[0] = CreateWindowExA(WS_EX_ACCEPTFILES,
								  "gameplay", "",
								  WS_OVERLAPPEDWINDOW|WS_VISIBLE,
								  0 , 0, 
								  GP_GRAPHICS_WIDTH, GP_GRAPHICS_HEIGHT,
								  NULL, NULL,
								  instance, 0);

		flags[0] = 0 | GP_WINDOW_FLAG_ASPECT_RATIO | GP_WINDOW_FLAG_FRAME;

		setHwnd(hwnd[0]);

		adjust(hwnd[0], GP_GRAPHICS_WIDTH, GP_GRAPHICS_HEIGHT, true);
		clear(hwnd[0]);

		width     = GP_GRAPHICS_WIDTH;
		height    = GP_GRAPHICS_HEIGHT;
		oldWidth  = GP_GRAPHICS_WIDTH;
		oldHeight = GP_GRAPHICS_HEIGHT;

		MainThreadEntry mte;
		mte.argc = argc;
		mte.argv = argv;

		bgfx::renderFrame();

		bx::Thread thread;
		thread.init(mte.threadFunc, &mte);
		init = true;

		eventQueue.postSizeEvent(findHandle(hwnd[0]), width, height);

		MSG msg;
		msg.message = WM_NULL;

		while (!exit)
		{
			bgfx::renderFrame();

			__xinput.update(eventQueue);
			WaitForInputIdle(GetCurrentProcess(), 16);

			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		while (bgfx::RenderFrame::NoContext != bgfx::renderFrame() ) {};

		thread.shutdown();

		DestroyWindow(hwnd[0]);

		__xinput.shutdown();

		return thread.getExitCode();
	}

	LRESULT process(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
	{
		if (init)
		{
			switch (id)
			{
			case WM_USER_WINDOW_CREATE:
				{
					Msg* msg = (Msg*)lparam;
					HWND wnd = CreateWindowA("gameplay", 
											  msg->title.c_str(),
											  WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
											  msg->x, msg->y, 
											  msg->width, msg->height, NULL , NULL, 
											  (HINSTANCE)GetModuleHandle(NULL), 0);
					clear(wnd);

					this->hwnd[wparam]  = wnd;
					this->flags[wparam] = msg->flags;
					gameplay::Platform::WindowHandle handle = { (uint16_t)wparam };
					eventQueue.postSizeEvent(handle, msg->width, msg->height);
					eventQueue.postWindowEvent(handle, hwnd);

					delete msg;
				}
				break;

			case WM_USER_WINDOW_DESTROY:
				{
					gameplay::Platform::WindowHandle handle = { (uint16_t)wparam };
					eventQueue.postWindowEvent(handle);
					DestroyWindow(this->hwnd[wparam]);
					this->hwnd[wparam] = 0;

					if (handle.idx == 0)
					{
						exit = true;
						eventQueue.postExitEvent();
					}
				}
				break;

			case WM_USER_WINDOW_SET_TITLE:
				{
					Msg* msg = (Msg*)lparam;
					SetWindowTextA(this->hwnd[wparam], msg->title.c_str());
					delete msg;
				}
				break;

			case WM_USER_WINDOW_SET_FLAGS:
				{
					Msg* msg = (Msg*)lparam;

					if (msg->flagsEnabled)
					{
						flags[wparam] |= msg->flags;
					}
					else
					{
						flags[wparam] &= ~msg->flags;
					}
					delete msg;
				}
				break;

			case WM_USER_WINDOW_SET_POS:
				{
					Msg* msg = (Msg*)lparam;
					SetWindowPos(this->hwnd[wparam], 0, 
								 msg->x, msg->y, 0, 0, 
								 SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE);
					delete msg;
				}
				break;

			case WM_USER_WINDOW_SET_SIZE:
				{
					uint32_t width  = GET_X_LPARAM(lparam);
					uint32_t height = GET_Y_LPARAM(lparam);
					adjust(this->hwnd[wparam], width, height, true);
				}
				break;

			case WM_USER_WINDOW_TOGGLE_FRAME:
				{
					if (frame)
					{
						oldWidth  = width;
						oldHeight = height;
					}
					adjust(this->hwnd[wparam], oldWidth, oldHeight, !frame);
				}
				break;

			case WM_USER_WINDOW_MOUSE_LOCK:
				setMouseLock(this->hwnd[wparam], !!lparam);
				break;

			case WM_DESTROY:
				break;

			case WM_QUIT:
			case WM_CLOSE:
				destroyWindow(findHandle(hwnd));
				return 0;

			case WM_SIZING:
				{
					gameplay::Platform::WindowHandle handle = findHandle(hwnd);

					if (gameplay::Platform::isWindowValid(handle)
					&&  GP_WINDOW_FLAG_ASPECT_RATIO & flags[handle.idx])
					{
						RECT& rect = *(RECT*)lparam;
						uint32_t width  = rect.right  - rect.left - frameWidth;
						uint32_t height = rect.bottom - rect.top  - frameHeight;

						// Recalculate size according to aspect ratio
						switch (wparam)
						{
						case WMSZ_LEFT:
						case WMSZ_RIGHT:
							{
								float aspectRatio = 1.0f/this->aspectRatio;
								width  = bx::uint32_max(GP_GRAPHICS_WIDTH / 4, width);
								height = uint32_t(float(width)*aspectRatio);
							}
							break;

						default:
							{
								float aspectRatio = this->aspectRatio;
								height = bx::uint32_max(GP_GRAPHICS_HEIGHT / 4, height);
								width  = uint32_t(float(height)*aspectRatio);
							}
							break;
						}

						// Recalculate position using different anchor points
						switch (wparam)
						{
						case WMSZ_TOPLEFT:
							rect.left = rect.right - width - frameWidth;
							rect.top = rect.bottom - height - frameHeight;
							break;

						case WMSZ_TOP:
						case WMSZ_TOPRIGHT:
							rect.right = rect.left + width + frameWidth;
							rect.top = rect.bottom - height - frameHeight;
							break;

						case WMSZ_LEFT:
						case WMSZ_BOTTOMLEFT:
							rect.left = rect.right - width - frameWidth;
							rect.bottom = rect.top + height + frameHeight;
							break;

						default:
							rect.right  = rect.left + width  + frameWidth;
							rect.bottom = rect.top  + height + frameHeight;
							break;
						}
						eventQueue.postSizeEvent(findHandle(hwnd), width, height);
					}
				}
				return 0;

			case WM_SIZE:
				{
					gameplay::Platform::WindowHandle handle = findHandle(hwnd);
					if (gameplay::Platform::isWindowValid(handle) )
					{
						uint32_t width  = GET_X_LPARAM(lparam);
						uint32_t height = GET_Y_LPARAM(lparam);

						this->width  = width;
						this->height = height;
						eventQueue.postSizeEvent(handle, width, height);
					}
				}
				break;

			case WM_SYSCOMMAND:
				switch (wparam)
				{
				case SC_MINIMIZE:
				case SC_RESTORE:
					{
						HWND parent = GetWindow(hwnd, GW_OWNER);
						if (NULL != parent)
						{
							PostMessage(parent, id, wparam, lparam);
						}
					}
				}
				break;

			case WM_MOUSEMOVE:
				{
					int32_t mx = GET_X_LPARAM(lparam);
					int32_t my = GET_Y_LPARAM(lparam);

					if (hwnd == mouseLock)
					{
						mx -= this->mx;
						my -= this->my;

						if (0 == mx
						&&  0 == my)
						{
							break;
						}
						setMousePos(hwnd, this->mx, this->my);
					}
					eventQueue.postMouseEvent(findHandle(hwnd), mx, my, mz);
				}
				break;

			case WM_MOUSEWHEEL:
				{
					POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
					ScreenToClient(hwnd, &pt);
					int32_t mx = pt.x;
					int32_t my = pt.y;
					this->mz += GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
					eventQueue.postMouseEvent(findHandle(hwnd), mx, my, this->mz);
				}
				break;

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
				{
					mouseCapture(hwnd, id == WM_LBUTTONDOWN);
					int32_t mx = GET_X_LPARAM(lparam);
					int32_t my = GET_Y_LPARAM(lparam);
					eventQueue.postMouseEvent(findHandle(hwnd), mx, my, this->mz, gameplay::Input::MouseButton::MOUSE_BUTTON_LEFT, id == WM_LBUTTONDOWN);
				}
				break;

			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
				{
					mouseCapture(hwnd, id == WM_MBUTTONDOWN);
					int32_t mx = GET_X_LPARAM(lparam);
					int32_t my = GET_Y_LPARAM(lparam);
					eventQueue.postMouseEvent(findHandle(hwnd), mx, my, this->mz, gameplay::Input::MouseButton::MOUSE_BUTTON_MIDDLE, id == WM_MBUTTONDOWN);
				}
				break;

			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:
				{
					mouseCapture(hwnd, id == WM_RBUTTONDOWN);
					int32_t mx = GET_X_LPARAM(lparam);
					int32_t my = GET_Y_LPARAM(lparam);
					eventQueue.postMouseEvent(findHandle(hwnd), mx, my, this->mz, gameplay::Input::MouseButton::MOUSE_BUTTON_RIGHT, id == WM_RBUTTONDOWN);
				}
				break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				{
					uint8_t keyModifiers = translateKeyModifiers();
					gameplay::Input::Key key = translateKey(wparam);

					gameplay::Platform::WindowHandle handle = findHandle(hwnd);

					if (gameplay::Input::Key::KEY_PRINT == key &&  0x3 == ( (uint32_t)(lparam)>>30) )
					{
						// VK_SNAPSHOT doesn't generate keydown event. Fire on down event when previous
						// key state bit is set to 1 and transition state bit is set to 1.
						//
						// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646280%28v=vs.85%29.aspx
						eventQueue.postKeyEvent(handle, key, keyModifiers, true);
					}
					eventQueue.postKeyEvent(handle, key, keyModifiers, id == WM_KEYDOWN || id == WM_SYSKEYDOWN);
				}
				break;

			case WM_CHAR:
				{
					uint8_t utf8[4] = {};
					uint8_t len = (uint8_t)WideCharToMultiByte(CP_UTF8, 0,
															  (LPCWSTR)&wparam, 1,
															  (LPSTR)utf8, BX_COUNTOF(utf8),
															  NULL, NULL);
					if (len != 0)
					{
						gameplay::Platform::WindowHandle handle = findHandle(hwnd);
						eventQueue.postCharEvent(handle, len, utf8);
					}
				}
				break;

			case WM_DROPFILES:
				{
					HDROP drop = (HDROP)wparam;
					char tmp[bx::kMaxFilePath];
					uint32_t result = DragQueryFileA(drop, 0, tmp, sizeof(tmp) );
					BX_UNUSED(result);
					gameplay::Platform::WindowHandle handle = findHandle(hwnd);
					eventQueue.postDropFileEvent(handle, tmp);
				}
				break;

			default:
				break;
			}
		}

		return DefWindowProc(hwnd, id, wparam, lparam);
	}

	gameplay::Platform::WindowHandle findHandle(HWND hwnd)
	{
		bx::MutexScope scope(lock);
		for (uint16_t i = 0, num = windowAlloc.getNumHandles(); i < num; ++i)
		{
			uint16_t idx = windowAlloc.getHandleAt(i);
			if (hwnd == this->hwnd[idx])
			{
				gameplay::Platform::WindowHandle handle = { idx };
				return handle;
			}
		}

		gameplay::Platform::WindowHandle invalidWindow = { UINT16_MAX };
		return invalidWindow;
	}

	void clear(HWND hwnd)
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0) );
		HDC hdc = GetDC(hwnd);
		SelectObject(hdc, brush);
		FillRect(hdc, &rect, brush);
	}

	void adjust(HWND hwnd, uint32_t width, uint32_t height, bool windowFrame)
	{
		this->width  = width;
		this->height = height;
		this->aspectRatio = float(width) / float(height);

		ShowWindow(hwnd, SW_SHOWNORMAL);
		RECT rect;
		RECT newrect = {0, 0, (LONG)width, (LONG)height};
		DWORD style = WS_POPUP|WS_SYSMENU;

		if (this->frame)
		{
			GetWindowRect(hwnd, &this->rect);
			this->style = GetWindowLong(hwnd, GWL_STYLE);
		}

		if (windowFrame)
		{
			rect = this->rect;
			style = this->style;
		}
		else
		{
			HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(monitor, &mi);
			newrect = mi.rcMonitor;
			rect = mi.rcMonitor;
			this->aspectRatio = float(newrect.right  - newrect.left) / float(newrect.bottom - newrect.top);
		}

		SetWindowLong(hwnd, GWL_STYLE, style);
		uint32_t prewidth  = newrect.right - newrect.left;
		uint32_t preheight = newrect.bottom - newrect.top;
		AdjustWindowRect(&newrect, style, FALSE);
		this->frameWidth  = (newrect.right  - newrect.left) - prewidth;
		this->frameHeight = (newrect.bottom - newrect.top ) - preheight;
		UpdateWindow(hwnd);

		if (rect.left == -32000
		||  rect.top  == -32000)
		{
			rect.left = 0;
			rect.top  = 0;
		}

		int32_t left   = rect.left;
		int32_t top    = rect.top;
		int32_t w  = (newrect.right-newrect.left);
		int32_t h = (newrect.bottom-newrect.top);

		if (!windowFrame)
		{
			float aspectRatio = 1.0f / this->aspectRatio;
			w = bx::uint32_max(GP_GRAPHICS_WIDTH / 4, w);
			h = uint32_t(float(width) * aspectRatio);

			left = newrect.left+(newrect.right -newrect.left - width)/2;
			top = newrect.top +(newrect.bottom-newrect.top - height)/2;
		}

		SetWindowPos(hwnd, HWND_TOP, left, top, w, h, SWP_SHOWWINDOW);
		ShowWindow(hwnd, SW_RESTORE);

		this->frame = windowFrame;
	}

	void setMousePos(HWND hwnd, int32_t mx, int32_t my)
	{
		POINT pt = { mx, my };
		ClientToScreen(hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void setMouseLock(HWND hwnd, bool lock)
	{
		if (hwnd != this->mouseLock)
		{
			if (lock)
			{
				this->mx = this->width / 2;
				this->my = this->height /2;
				ShowCursor(false);
				setMousePos(hwnd, this->mx, this->my);
			}
			else
			{
				setMousePos(hwnd, this->mx, this->my);
				ShowCursor(true);
			}
			this->mouseLock = hwnd;
		}
	}

	static LRESULT CALLBACK wndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

	gameplay::Platform::EventQueue eventQueue;
	bx::Mutex lock;
	bx::HandleAllocT<GP_WINDOWS_MAX> windowAlloc;
	HWND hwnd[GP_WINDOWS_MAX];
	uint32_t flags[GP_WINDOWS_MAX];
	RECT rect;
	DWORD style;
	uint32_t width;
	uint32_t height;
	uint32_t oldWidth;
	uint32_t oldHeight;
	uint32_t frameWidth;
	uint32_t frameHeight;
	float aspectRatio;
	int32_t mx;
	int32_t my;
	int32_t mz;
	bool frame;
	HWND mouseLock;
	bool init;
	bool exit;
};

static Context __ctx;


extern void print(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        GP_SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
}

const gameplay::Platform::Event* poll()
{
	return __ctx.eventQueue.poll();
}

const gameplay::Platform::Event* poll(gameplay::Platform::WindowHandle handle)
{
	return __ctx.eventQueue.poll(handle);
}

void release(const gameplay::Platform::Event* evt)
{
	__ctx.eventQueue.release(evt);
}

LRESULT CALLBACK Context::wndProc(HWND _hwnd, UINT _id, WPARAM _wparam, LPARAM _lparam)
{
	return __ctx.process(_hwnd, _id, _wparam, _lparam);
}

gameplay::Platform::WindowHandle createWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags, const char* title)
{
	bx::MutexScope scope(__ctx.lock);
	gameplay::Platform::WindowHandle handle = { __ctx.windowAlloc.alloc() };

	if ( handle.idx != UINT16_MAX)
	{
		Msg* msg = new Msg;
		msg->x = x;
		msg->y = y;
		msg->width  = width;
		msg->height = height;
		msg->title  = title;
		msg->flags  = flags;
		PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_CREATE, handle.idx, (LPARAM)msg);
	}
	return handle;
}

void destroyWindow(gameplay::Platform::WindowHandle handle)
{
	if (handle.idx != UINT16_MAX)
	{
		PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_DESTROY, handle.idx, 0);

		bx::MutexScope scope(__ctx.lock);
		__ctx.windowAlloc.free(handle.idx);
	}
}

void setWindowPos(gameplay::Platform::WindowHandle handle, int32_t x, int32_t y)
{
	Msg* msg = new Msg;
	msg->x = x;
	msg->y = y;
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_SET_POS, handle.idx, (LPARAM)msg);
}

void setWindowSize(gameplay::Platform::WindowHandle handle, uint32_t width, uint32_t height)
{
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_SET_SIZE, handle.idx, (height << 16) | (width & 0xffff) );
}

void setWindowTitle(gameplay::Platform::WindowHandle handle, const char* title)
{
	Msg* msg = new Msg;
	msg->title = title;
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_SET_TITLE, handle.idx, (LPARAM)msg);
}

void setWindowFlags(gameplay::Platform::WindowHandle handle, uint32_t flags, bool flagsEnabled)
{
	Msg* msg = new Msg;
	msg->flags = flags;
	msg->flagsEnabled = flagsEnabled;
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_SET_FLAGS, handle.idx, (LPARAM)msg);
}

void toggleFullscreen(gameplay::Platform::WindowHandle handle)
{
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_TOGGLE_FRAME, handle.idx, 0);
}

void setMouseLock(gameplay::Platform::WindowHandle handle, bool _lock)
{
	PostMessage(__ctx.hwnd[0], WM_USER_WINDOW_MOUSE_LOCK, handle.idx, _lock);
}


int32_t MainThreadEntry::threadFunc(bx::Thread*, void* userData)
{
	MainThreadEntry* self = (MainThreadEntry*)userData;
	int32_t result = main(self->argc, self->argv);
	PostMessage(__ctx.hwnd[0], WM_QUIT, 0, 0);
	return result;
}

}


int main(int argc, const char* const* argv)
{
	using namespace gameplay;
	return __ctx.run(argc, argv);
}

#endif

