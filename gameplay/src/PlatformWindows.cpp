#include "Base.h"
#include "PlatformWindows.h"
#include "Game.h"
#include "Graphics.h"
#include "vk/GraphicsVK.h"
#include "d3d12/GraphicsD3D12.h"

namespace gameplay
{

extern Platform* getPlatform()
{
    static Platform* platform = new PlatformWindows();
    return platform;
}

Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
        if (config->graphics.compare(GP_GRAPHICS_D3D12) == 0)
        {
            _graphics = new GraphicsD3D12();
            _graphics->_api = Graphics::API_D3D12;
        }
        else
        {
            _graphics = new GraphicsVK();
            _graphics->_api = Graphics::API_VK;
        }
    }
    return _graphics;
}

PlatformWindows::PlatformWindows() :
    _hwnd(nullptr),
    _instance(nullptr),
    _graphics(nullptr)
{
}

PlatformWindows::~PlatformWindows()
{
}

bool PlatformWindows::startup(int argc, char** argv)
{
	// Get the application connection instance
	_instance = GetModuleHandle(0);

    // Get the game config
    std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();

	// Determine the window width and height
    int width = config->width;
    int height = config->height;
	if (width == 0 || height == 0)
	{
		width = GP_GRAPHICS_WIDTH;
		height = GP_GRAPHICS_HEIGHT;
	}

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)PlatformWindows::onMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _instance;
    wc.hIcon = LoadIcon(_instance, IDI_APPLICATION);
    wc.hIconSm = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = L"gameplay";
	RegisterClassEx(&wc);

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Set the window style
    DWORD style, styleEx;
    if (config->fullscreen)
    {
        style = WS_POPUP;
        styleEx = WS_EX_APPWINDOW;
    }
    else
    {
		style = WS_OVERLAPPEDWINDOW;
        styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }

    // Create the window
    std::wstring title_wstr = std::wstring(config->title.begin(), config->title.end());
    _hwnd = CreateWindowEx(styleEx, 
						   L"gameplay", 
						   title_wstr.c_str(), 
						   style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
						   0, 0, 
						   windowRect.right - windowRect.left, 
		                   windowRect.bottom - windowRect.top, 
						   nullptr, nullptr, _instance, nullptr);
    if (!_hwnd)
        GP_ERROR("Failed to create window.");

    // Enumerate all supported display settings and match
    if (config->fullscreen)
    {
        bool modeSupported = false;
        DWORD modeNum = 0;
        DEVMODE devMode;
        memset(&devMode, 0, sizeof(DEVMODE));
        devMode.dmSize = sizeof(DEVMODE);
        devMode.dmDriverExtra = 0;
        while (EnumDisplaySettings(nullptr, modeNum++, &devMode) != 0)
        {
            if (devMode.dmPelsWidth == width &&
                devMode.dmPelsHeight == height &&
                devMode.dmBitsPerPel == 32)
            {
                modeSupported = true;
                break;
            }
        }
        if (!modeSupported)
        {
            width = 0;
            height = 0;
            windowRect.right = windowRect.left + width;
            windowRect.bottom = windowRect.top + height;
        }

        DEVMODE dm = {};
        dm.dmSize = sizeof(dm);
        dm.dmPelsWidth = width;
        dm.dmPelsHeight = height;
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            GP_ERROR("Failed to start game in full-screen mode with resolution %dx%d.", width, height);
    }


    // CEnter the window postiion
	if (!config->fullscreen)
	{
		GetWindowRect(_hwnd, &windowRect);
		const int screenX = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		const int screenY = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(_hwnd, 0, screenX, screenY, -1, -1, SWP_NOZORDER | SWP_NOSIZE);
	}

    // Initialize the graphics system
    gameplay::Graphics::getGraphics()->onInitialize((unsigned long)_hwnd, (unsigned long)_instance);

    // Show the window
    ShowWindow(_hwnd, SW_SHOW);

    // Find any connected gamepads
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        if (XInputGetState(i, &_gamepadState[i]) == NO_ERROR)
        {
            if (!_gamepadsConnected[i])
            {
                _gamepadsConnected[i] = true;
                gameplay::getPlatform()->onGamepadEvent(gameplay::Platform::GAMEPAD_EVENT_CONNECTED, i);
            }
        }
		else
		{
			_gamepadsConnected[i] = false;
		}
    }

	// Initialize the game
	Game::getInstance()->onInitialize();
    return true;
}

void PlatformWindows::shutdown()
{
}

int PlatformWindows::enterMessagePump()
{
    gameplay::Game* game = gameplay::Game::getInstance();
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                game->exit();
                return msg.wParam;
            }
        }
        else
        {
            for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
            {
                if (XInputGetState(i, &_gamepadState[i]) == NO_ERROR && !_gamepadsConnected[i])
                {                    
                    _gamepadsConnected[i] = true;
                    gameplay::getPlatform()->onGamepadEvent(gameplay::Platform::GAMEPAD_EVENT_CONNECTED, i);
                }
                else if (XInputGetState(i, &_gamepadState[i]) != NO_ERROR && _gamepadsConnected[i])
                {
                    _gamepadsConnected[i] = false;
                    gameplay::getPlatform()->onGamepadEvent(gameplay::Platform::GAMEPAD_EVENT_DISCONNECTED, i);
                }
            }
            game->onFrame();            
        }
    }
    return 0;
}

LRESULT __stdcall PlatformWindows::onMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	static POINT mouseCapturePoint = { 0, 0 };
	static bool shiftDown = false;
	static bool capsOn = false;
	static bool resizing = false;
	static DWORD systemColorsPrev[2];
	static int systemElements[2] = {COLOR_WINDOW, COLOR_ACTIVECAPTION};

	Game* game = gameplay::Game::getInstance();

	switch (msg)
	{
		case WM_DESTROY:
		{
			game->exit();
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			game->exit();
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			updateCapture(wParam);
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_PRESS_LEFT_BUTTON, x, y, 0);
			return 0;
		}
		case WM_LBUTTONUP:
		{
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_RELEASE_LEFT_BUTTON, x, y, 0);;
			updateCapture(wParam);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			updateCapture(lParam);
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_PRESS_RIGHT_BUTTON, x, y, 0);
			break;
		}
		case WM_RBUTTONUP:
		{
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_RELEASE_RIGHT_BUTTON, x, y, 0);
			updateCapture(wParam);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			updateCapture(wParam);
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_PRESS_MIDDLE_BUTTON, x, y, 0);
			break;
		}
		case WM_MBUTTONUP:
		{
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_RELEASE_MIDDLE_BUTTON, x, y, 0);
			updateCapture(wParam);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (game->isMouseCapture())
			{
				if (x == mouseCapturePoint.x && y == mouseCapturePoint.y)
					break;
				x -= mouseCapturePoint.x;
				y -= mouseCapturePoint.y;
				warpMouse(mouseCapturePoint.x, mouseCapturePoint.y);
			}
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_MOVE, x, y, 0);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			tagPOINT point;
			ScreenToClient((HWND)gameplay::getPlatform()->getWindow(), &point);
            gameplay::getPlatform()->onMouseEvent(gameplay::Platform::MOUSE_EVENT_WHEEL, point.x, point.y, GET_WHEEL_DELTA_WPARAM(wParam) / 120);
			break;
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
				shiftDown = true;
			if (wParam == VK_CAPITAL)
				capsOn = !capsOn;
            gameplay::getPlatform()->onKeyEvent(gameplay::Platform::KEY_EVENT_PRESS, translateKey(wParam, shiftDown ^ capsOn));
			break;
		}
		case WM_KEYUP:
		{
			if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
				shiftDown = false;
            gameplay::getPlatform()->onKeyEvent(gameplay::Platform::KEY_EVENT_RELEASE, translateKey(wParam, shiftDown ^ capsOn));
			break;
		}
		case WM_CHAR:
		{
            gameplay::getPlatform()->onKeyEvent(gameplay::Platform::KEY_EVENT_CHAR, translateKey(wParam, shiftDown ^ capsOn));
			break;
		}
		case WM_UNICHAR:
		{
            gameplay::getPlatform()->onKeyEvent(gameplay::Platform::KEY_EVENT_CHAR, translateKey(wParam, shiftDown ^ capsOn));
			break;
		}
		case WM_SIZE:
		{
            gameplay::Graphics* graphics = gameplay::Graphics::getGraphics();
            if (graphics && graphics->isResized() && wParam != SIZE_MINIMIZED)
			{
				if (resizing || wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
				{
					RECT clientRect = {};
					GetClientRect(hwnd, &clientRect);
					size_t width = clientRect.right - clientRect.left;
					size_t height = clientRect.bottom - clientRect.top;
                    graphics->onResize(width, height);
					game->onResize(width, height);
				}
			}
			break;
		}
		case WM_ENTERSIZEMOVE:
		{
			resizing = true;
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			resizing = false;
			break;
		}
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

unsigned long PlatformWindows::getWindow()
{
    return (unsigned long)_hwnd;
}

unsigned long PlatformWindows::getConnection()
{
    return (unsigned long)_instance;
}

Input::Key PlatformWindows::translateKey(WPARAM windowsKeyCode, bool shiftDown)
{
    switch (windowsKeyCode)
    {
		case VK_PAUSE:
			return gameplay::Input::KEY_PAUSE;
		case VK_SCROLL:
			return gameplay::Input::KEY_SCROLL_LOCK;
		case VK_PRINT:
			return gameplay::Input::KEY_PRINT;
		case VK_ESCAPE:
			return gameplay::Input::KEY_ESCAPE;
		case VK_BACK:
		case VK_F16:
			return gameplay::Input::KEY_BACKSPACE;
		case VK_TAB:
			return shiftDown ? gameplay::Input::KEY_BACK_TAB : gameplay::Input::KEY_TAB;
		case VK_RETURN:
			return gameplay::Input::KEY_RETURN;
		case VK_CAPITAL:
			return gameplay::Input::KEY_CAPS_LOCK;
		case VK_SHIFT:
			return gameplay::Input::KEY_SHIFT;
		case VK_CONTROL:
			return gameplay::Input::KEY_CTRL;
		case VK_MENU:
			return gameplay::Input::KEY_ALT;
		case VK_APPS:
			return gameplay::Input::KEY_MENU;
		case VK_LSHIFT:
			return gameplay::Input::KEY_SHIFT;
		case VK_RSHIFT:
			return gameplay::Input::KEY_SHIFT;
		case VK_LCONTROL:
			return gameplay::Input::KEY_CTRL;
		case VK_RCONTROL:
			return gameplay::Input::KEY_CTRL;
		case VK_LMENU:
			return gameplay::Input::KEY_ALT;
		case VK_RMENU:
			return gameplay::Input::KEY_ALT;
		case VK_LWIN:
		case VK_RWIN:
			return gameplay::Input::KEY_HYPER;
		case VK_BROWSER_SEARCH:
			return gameplay::Input::KEY_SEARCH;
		case VK_INSERT:
			return gameplay::Input::KEY_INSERT;
		case VK_HOME:
			return gameplay::Input::KEY_HOME;
		case VK_PRIOR:
			return gameplay::Input::KEY_PG_UP;
		case VK_DELETE:
			return gameplay::Input::KEY_DELETE;
		case VK_END:
			return gameplay::Input::KEY_END;
		case VK_NEXT:
			return gameplay::Input::KEY_PG_DOWN;
		case VK_LEFT:
			return gameplay::Input::KEY_LEFT_ARROW;
		case VK_RIGHT:
			return gameplay::Input::KEY_RIGHT_ARROW;
		case VK_UP:
			return gameplay::Input::KEY_UP_ARROW;
		case VK_DOWN:
			return gameplay::Input::KEY_DOWN_ARROW;
		case VK_NUMLOCK:
			return gameplay::Input::KEY_NUM_LOCK;
		case VK_ADD:
			return gameplay::Input::KEY_KP_PLUS;
		case VK_SUBTRACT:
			return gameplay::Input::KEY_KP_MINUS;
		case VK_MULTIPLY:
			return gameplay::Input::KEY_KP_MULTIPLY;
		case VK_DIVIDE:
			return gameplay::Input::KEY_KP_DIVIDE;
		case VK_NUMPAD7:
			return gameplay::Input::KEY_KP_HOME;
		case VK_NUMPAD8:
			return gameplay::Input::KEY_KP_UP;
		case VK_NUMPAD9:
			return gameplay::Input::KEY_KP_PG_UP;
		case VK_NUMPAD4:
			return gameplay::Input::KEY_KP_LEFT;
		case VK_NUMPAD5:
			return gameplay::Input::KEY_KP_FIVE;
		case VK_NUMPAD6:
			return gameplay::Input::KEY_KP_RIGHT;
		case VK_NUMPAD1:
			return gameplay::Input::KEY_KP_END;
		case VK_NUMPAD2:
			return gameplay::Input::KEY_KP_DOWN;
		case VK_NUMPAD3:
			return gameplay::Input::KEY_KP_PG_DOWN;
		case VK_NUMPAD0:
			return gameplay::Input::KEY_KP_INSERT;
		case VK_DECIMAL:
			return gameplay::Input::KEY_KP_DELETE;
		case VK_F1:
			return gameplay::Input::KEY_F1;
		case VK_F2:
			return gameplay::Input::KEY_F2;
		case VK_F3:
			return gameplay::Input::KEY_F3;
		case VK_F4:
			return gameplay::Input::KEY_F4;
		case VK_F5:
			return gameplay::Input::KEY_F5;
		case VK_F6:
			return gameplay::Input::KEY_F6;
		case VK_F7:
			return gameplay::Input::KEY_F7;
		case VK_F8:
			return gameplay::Input::KEY_F8;
		case VK_F9:
			return gameplay::Input::KEY_F9;
		case VK_F10:
			return gameplay::Input::KEY_F10;
		case VK_F11:
			return gameplay::Input::KEY_F11;
		case VK_F12:
			return gameplay::Input::KEY_F12;
		case VK_SPACE:
			return gameplay::Input::KEY_SPACE;
		case 0x30:
			return shiftDown ? gameplay::Input::KEY_RIGHT_PARENTHESIS : gameplay::Input::KEY_ZERO;
		case 0x31:
			return shiftDown ? gameplay::Input::KEY_EXCLAM : gameplay::Input::KEY_ONE;
		case 0x32:
			return shiftDown ? gameplay::Input::KEY_AT : gameplay::Input::KEY_TWO;
		case 0x33:
			return shiftDown ? gameplay::Input::KEY_NUMBER : gameplay::Input::KEY_THREE;
		case 0x34:
			return shiftDown ? gameplay::Input::KEY_DOLLAR : gameplay::Input::KEY_FOUR;
		case 0x35:
			return shiftDown ? gameplay::Input::KEY_PERCENT : gameplay::Input::KEY_FIVE;
		case 0x36:
			return shiftDown ? gameplay::Input::KEY_CIRCUMFLEX : gameplay::Input::KEY_SIX;
		case 0x37:
			return shiftDown ? gameplay::Input::KEY_AMPERSAND : gameplay::Input::KEY_SEVEN;
		case 0x38:
			return shiftDown ? gameplay::Input::KEY_ASTERISK : gameplay::Input::KEY_EIGHT;
		case 0x39:
			return shiftDown ? gameplay::Input::KEY_LEFT_PARENTHESIS : gameplay::Input::KEY_NINE;
		case VK_OEM_PLUS:
			return shiftDown ? gameplay::Input::KEY_EQUAL : gameplay::Input::KEY_PLUS;
		case VK_OEM_COMMA:
			return shiftDown ? gameplay::Input::KEY_LESS_THAN : gameplay::Input::KEY_COMMA;
		case VK_OEM_MINUS:
			return shiftDown ? gameplay::Input::KEY_UNDERSCORE : gameplay::Input::KEY_MINUS;
		case VK_OEM_PERIOD:
			return shiftDown ? gameplay::Input::KEY_GREATER_THAN : gameplay::Input::KEY_PERIOD;
		case VK_OEM_1:
			return shiftDown ? gameplay::Input::KEY_COLON : gameplay::Input::KEY_SEMICOLON;
		case VK_OEM_2:
			return shiftDown ? gameplay::Input::KEY_QUESTION : gameplay::Input::KEY_SLASH;
		case VK_OEM_3:
			return shiftDown ? gameplay::Input::KEY_TILDE : gameplay::Input::KEY_GRAVE;
		case VK_OEM_4:
			return shiftDown ? gameplay::Input::KEY_LEFT_BRACE : gameplay::Input::KEY_LEFT_BRACKET;
		case VK_OEM_5:
			return shiftDown ? gameplay::Input::KEY_BAR : gameplay::Input::KEY_BACK_SLASH;
		case VK_OEM_6:
			return shiftDown ? gameplay::Input::KEY_RIGHT_BRACE : gameplay::Input::KEY_RIGHT_BRACKET;
		case VK_OEM_7:
			return shiftDown ? gameplay::Input::KEY_QUOTE : gameplay::Input::KEY_APOSTROPHE;
		case 0x41:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_A : gameplay::Input::KEY_A;
		case 0x42:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_B : gameplay::Input::KEY_B;
		case 0x43:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_C : gameplay::Input::KEY_C;
		case 0x44:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_D : gameplay::Input::KEY_D;
		case 0x45:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_E : gameplay::Input::KEY_E;
		case 0x46:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_F : gameplay::Input::KEY_F;
		case 0x47:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_G : gameplay::Input::KEY_G;
		case 0x48:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_H : gameplay::Input::KEY_H;
		case 0x49:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_I : gameplay::Input::KEY_I;
		case 0x4A:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_J : gameplay::Input::KEY_J;
		case 0x4B:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_K : gameplay::Input::KEY_K;
		case 0x4C:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_L : gameplay::Input::KEY_L;
		case 0x4D:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_M : gameplay::Input::KEY_M;
		case 0x4E:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_N : gameplay::Input::KEY_N;
		case 0x4F:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_O : gameplay::Input::KEY_O;
		case 0x50:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_P : gameplay::Input::KEY_P;
		case 0x51:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_Q : gameplay::Input::KEY_Q;
		case 0x52:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_R : gameplay::Input::KEY_R;
		case 0x53:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_S : gameplay::Input::KEY_S;
		case 0x54:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_T : gameplay::Input::KEY_T;
		case 0x55:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_U : gameplay::Input::KEY_U;
		case 0x56:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_V : gameplay::Input::KEY_V;
		case 0x57:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_W : gameplay::Input::KEY_W;
		case 0x58:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_X : gameplay::Input::KEY_X;
		case 0x59:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_Y : gameplay::Input::KEY_Y;
		case 0x5A:
			return shiftDown ? gameplay::Input::KEY_CAPITAL_Z : gameplay::Input::KEY_Z;
		default:
			return gameplay::Input::KEY_NONE;
    }
}

void PlatformWindows::updateCapture(LPARAM param)
{
    HWND window = (HWND)gameplay::getPlatform()->getWindow();
    if ((param & MK_LBUTTON) || (param & MK_MBUTTON) || (param & MK_RBUTTON))
    {
        SetCapture(window);
    }
    else
    {
        ReleaseCapture();
    }
}

void PlatformWindows::warpMouse(int clientX, int clientY)
{
    HWND window = (HWND)gameplay::getPlatform()->getWindow();
    POINT point = { clientX, clientY };
    ClientToScreen(window, &point);
    SetCursorPos(point.x, point.y);
}

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

}
