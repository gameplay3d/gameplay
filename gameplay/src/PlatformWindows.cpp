#ifdef WIN32

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "Vector2.h"
#include "ScriptController.h"
#include <GL/wglew.h>
#include <windowsx.h>
#include <shellapi.h>
#ifdef USE_XINPUT
#include <XInput.h>
#endif

using gameplay::print;

// Window defaults
#define DEFAULT_RESOLUTION_X 1024
#define DEFAULT_RESOLUTION_Y 768
#define DEFAULT_COLOR_BUFFER_SIZE 32
#define DEFAULT_DEPTH_BUFFER_SIZE 24
#define DEFAULT_STENCIL_BUFFER_SIZE 8

static double __timeTicksPerMillis;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static HINSTANCE __hinstance = 0;
static HWND __attachToWindow = 0;
static HWND __hwnd = 0;
static HDC __hdc = 0;
static HGLRC __hrc = 0;
static bool __mouseCaptured = false;
static POINT __mouseCapturePoint = { 0, 0 };
static bool __multiSampling = false;
static bool __cursorVisible = true;
static unsigned int __gamepadsConnected = 0;

#ifdef USE_XINPUT
static const unsigned int XINPUT_BUTTON_COUNT = 14;
static const unsigned int XINPUT_JOYSTICK_COUNT = 2;
static const unsigned int XINPUT_TRIGGER_COUNT = 2;

static XINPUT_STATE __xInputState;
static bool __connectedXInput[4];

static float normalizeXInputJoystickAxis(int axisValue, int deadZone)
{
    int absAxisValue = abs(axisValue);

    if (absAxisValue < deadZone)
    {
        return 0.0f;
    }
    else
    {
        int value = axisValue;
        int maxVal;
        if (value < 0)
        {
            value = -1;
            maxVal = 32768;
        }
        else if (value > 0)
        {
            value = 1;
            maxVal = 32767;
        }
        else
        {
            return 0;
        }

        return value * (absAxisValue - deadZone) / (float)(maxVal - deadZone);
    }
}
#endif

static gameplay::Keyboard::Key getKey(WPARAM win32KeyCode, bool shiftDown)
{
    switch (win32KeyCode)
    {
    case VK_PAUSE:
        return gameplay::Keyboard::KEY_PAUSE;
    case VK_SCROLL:
        return gameplay::Keyboard::KEY_SCROLL_LOCK;
    case VK_PRINT:
        return gameplay::Keyboard::KEY_PRINT;
    case VK_ESCAPE:
        return gameplay::Keyboard::KEY_ESCAPE;
    case VK_BACK:
        return gameplay::Keyboard::KEY_BACKSPACE;
    case VK_TAB:
        return shiftDown ? gameplay::Keyboard::KEY_BACK_TAB : gameplay::Keyboard::KEY_TAB;
    case VK_RETURN:
        return gameplay::Keyboard::KEY_RETURN;
    case VK_CAPITAL:
        return gameplay::Keyboard::KEY_CAPS_LOCK;
    case VK_SHIFT:
        return gameplay::Keyboard::KEY_SHIFT;
    case VK_CONTROL:
        return gameplay::Keyboard::KEY_CTRL;
    case VK_MENU:
        return gameplay::Keyboard::KEY_ALT;
    case VK_APPS:
        return gameplay::Keyboard::KEY_MENU;
    case VK_LSHIFT:
        return gameplay::Keyboard::KEY_SHIFT;
    case VK_RSHIFT:
        return gameplay::Keyboard::KEY_SHIFT;
    case VK_LCONTROL:
        return gameplay::Keyboard::KEY_CTRL;
    case VK_RCONTROL:
        return gameplay::Keyboard::KEY_CTRL;
    case VK_LMENU:
        return gameplay::Keyboard::KEY_ALT;
    case VK_RMENU:
        return gameplay::Keyboard::KEY_ALT;
    case VK_LWIN:
    case VK_RWIN:
        return gameplay::Keyboard::KEY_HYPER;
    case VK_BROWSER_SEARCH:
        return gameplay::Keyboard::KEY_SEARCH;
    case VK_INSERT:
        return gameplay::Keyboard::KEY_INSERT;
    case VK_HOME:
        return gameplay::Keyboard::KEY_HOME;
    case VK_PRIOR:
        return gameplay::Keyboard::KEY_PG_UP;
    case VK_DELETE:
        return gameplay::Keyboard::KEY_DELETE;
    case VK_END:
        return gameplay::Keyboard::KEY_END;
    case VK_NEXT:
        return gameplay::Keyboard::KEY_PG_DOWN;
    case VK_LEFT:
        return gameplay::Keyboard::KEY_LEFT_ARROW;
    case VK_RIGHT:
        return gameplay::Keyboard::KEY_RIGHT_ARROW;
    case VK_UP:
        return gameplay::Keyboard::KEY_UP_ARROW;
    case VK_DOWN:
        return gameplay::Keyboard::KEY_DOWN_ARROW;
    case VK_NUMLOCK:
        return gameplay::Keyboard::KEY_NUM_LOCK;
    case VK_ADD:
        return gameplay::Keyboard::KEY_KP_PLUS;
    case VK_SUBTRACT:
        return gameplay::Keyboard::KEY_KP_MINUS;
    case VK_MULTIPLY:
        return gameplay::Keyboard::KEY_KP_MULTIPLY;
    case VK_DIVIDE:
        return gameplay::Keyboard::KEY_KP_DIVIDE;
    case VK_NUMPAD7:
        return gameplay::Keyboard::KEY_KP_HOME;
    case VK_NUMPAD8:
        return gameplay::Keyboard::KEY_KP_UP;
    case VK_NUMPAD9:
        return gameplay::Keyboard::KEY_KP_PG_UP;
    case VK_NUMPAD4:
        return gameplay::Keyboard::KEY_KP_LEFT;
    case VK_NUMPAD5:
        return gameplay::Keyboard::KEY_KP_FIVE;
    case VK_NUMPAD6:
        return gameplay::Keyboard::KEY_KP_RIGHT;
    case VK_NUMPAD1:
        return gameplay::Keyboard::KEY_KP_END;
    case VK_NUMPAD2:
        return gameplay::Keyboard::KEY_KP_DOWN;
    case VK_NUMPAD3:
        return gameplay::Keyboard::KEY_KP_PG_DOWN;
    case VK_NUMPAD0:
        return gameplay::Keyboard::KEY_KP_INSERT;
    case VK_DECIMAL:
        return gameplay::Keyboard::KEY_KP_DELETE;
    case VK_F1:
        return gameplay::Keyboard::KEY_F1;
    case VK_F2:
        return gameplay::Keyboard::KEY_F2;
    case VK_F3:
        return gameplay::Keyboard::KEY_F3;
    case VK_F4:
        return gameplay::Keyboard::KEY_F4;
    case VK_F5:
        return gameplay::Keyboard::KEY_F5;
    case VK_F6:
        return gameplay::Keyboard::KEY_F6;
    case VK_F7:
        return gameplay::Keyboard::KEY_F7;
    case VK_F8:
        return gameplay::Keyboard::KEY_F8;
    case VK_F9:
        return gameplay::Keyboard::KEY_F9;
    case VK_F10:
        return gameplay::Keyboard::KEY_F10;
    case VK_F11:
        return gameplay::Keyboard::KEY_F11;
    case VK_F12:
        return gameplay::Keyboard::KEY_F12;
    case VK_SPACE:
        return gameplay::Keyboard::KEY_SPACE;
    case 0x30:
        return shiftDown ? gameplay::Keyboard::KEY_RIGHT_PARENTHESIS : gameplay::Keyboard::KEY_ZERO;
    case 0x31:
        return shiftDown ? gameplay::Keyboard::KEY_EXCLAM : gameplay::Keyboard::KEY_ONE;
    case 0x32:
        return shiftDown ? gameplay::Keyboard::KEY_AT : gameplay::Keyboard::KEY_TWO;
    case 0x33:
        return shiftDown ? gameplay::Keyboard::KEY_NUMBER : gameplay::Keyboard::KEY_THREE;
    case 0x34:
        return shiftDown ? gameplay::Keyboard::KEY_DOLLAR : gameplay::Keyboard::KEY_FOUR;
    case 0x35:
        return shiftDown ? gameplay::Keyboard::KEY_PERCENT : gameplay::Keyboard::KEY_FIVE;
    case 0x36:
        return shiftDown ? gameplay::Keyboard::KEY_CIRCUMFLEX : gameplay::Keyboard::KEY_SIX;
    case 0x37:
        return shiftDown ? gameplay::Keyboard::KEY_AMPERSAND : gameplay::Keyboard::KEY_SEVEN;
    case 0x38:
        return shiftDown ? gameplay::Keyboard::KEY_ASTERISK : gameplay::Keyboard::KEY_EIGHT;
    case 0x39:
        return shiftDown ? gameplay::Keyboard::KEY_LEFT_PARENTHESIS : gameplay::Keyboard::KEY_NINE;
    case VK_OEM_PLUS:
        return shiftDown ? gameplay::Keyboard::KEY_EQUAL : gameplay::Keyboard::KEY_PLUS;
    case VK_OEM_COMMA:
        return shiftDown ? gameplay::Keyboard::KEY_LESS_THAN : gameplay::Keyboard::KEY_COMMA;
    case VK_OEM_MINUS:
        return shiftDown ? gameplay::Keyboard::KEY_UNDERSCORE : gameplay::Keyboard::KEY_MINUS;
    case VK_OEM_PERIOD:
        return shiftDown ? gameplay::Keyboard::KEY_GREATER_THAN : gameplay::Keyboard::KEY_PERIOD;
    case VK_OEM_1:
        return shiftDown ? gameplay::Keyboard::KEY_COLON : gameplay::Keyboard::KEY_SEMICOLON;
    case VK_OEM_2:
        return shiftDown ? gameplay::Keyboard::KEY_QUESTION : gameplay::Keyboard::KEY_SLASH;
    case VK_OEM_3:
        return shiftDown ? gameplay::Keyboard::KEY_TILDE : gameplay::Keyboard::KEY_GRAVE;
    case VK_OEM_4:
        return shiftDown ? gameplay::Keyboard::KEY_LEFT_BRACE : gameplay::Keyboard::KEY_LEFT_BRACKET;
    case VK_OEM_5:
        return shiftDown ? gameplay::Keyboard::KEY_BAR : gameplay::Keyboard::KEY_BACK_SLASH;
    case VK_OEM_6:
        return shiftDown ? gameplay::Keyboard::KEY_RIGHT_BRACE : gameplay::Keyboard::KEY_RIGHT_BRACKET;
    case VK_OEM_7:
        return shiftDown ? gameplay::Keyboard::KEY_QUOTE : gameplay::Keyboard::KEY_APOSTROPHE;
    case 0x41:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_A : gameplay::Keyboard::KEY_A;
    case 0x42:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_B : gameplay::Keyboard::KEY_B;
    case 0x43:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_C : gameplay::Keyboard::KEY_C;
    case 0x44:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_D : gameplay::Keyboard::KEY_D;
    case 0x45:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_E : gameplay::Keyboard::KEY_E;
    case 0x46:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_F : gameplay::Keyboard::KEY_F;
    case 0x47:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_G : gameplay::Keyboard::KEY_G;
    case 0x48:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_H : gameplay::Keyboard::KEY_H;
    case 0x49:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_I : gameplay::Keyboard::KEY_I;
    case 0x4A:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_J : gameplay::Keyboard::KEY_J;
    case 0x4B:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_K : gameplay::Keyboard::KEY_K;
    case 0x4C:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_L : gameplay::Keyboard::KEY_L;
    case 0x4D:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_M : gameplay::Keyboard::KEY_M;
    case 0x4E:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_N : gameplay::Keyboard::KEY_N;
    case 0x4F:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_O : gameplay::Keyboard::KEY_O;
    case 0x50:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_P : gameplay::Keyboard::KEY_P;
    case 0x51:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_Q : gameplay::Keyboard::KEY_Q;
    case 0x52:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_R : gameplay::Keyboard::KEY_R;
    case 0x53:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_S : gameplay::Keyboard::KEY_S;
    case 0x54:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_T : gameplay::Keyboard::KEY_T;
    case 0x55:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_U : gameplay::Keyboard::KEY_U;
    case 0x56:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_V : gameplay::Keyboard::KEY_V;
    case 0x57:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_W : gameplay::Keyboard::KEY_W;
    case 0x58:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_X : gameplay::Keyboard::KEY_X;
    case 0x59:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_Y : gameplay::Keyboard::KEY_Y;
    case 0x5A:
        return shiftDown ? gameplay::Keyboard::KEY_CAPITAL_Z : gameplay::Keyboard::KEY_Z;
    default:
        return gameplay::Keyboard::KEY_NONE;
    }
}

static void UpdateCapture(LPARAM lParam)
{
    if ((lParam & MK_LBUTTON) || (lParam & MK_MBUTTON) || (lParam & MK_RBUTTON))
        SetCapture(__hwnd);
    else
        ReleaseCapture();
}

static void WarpMouse(int clientX, int clientY)
{
    POINT p = { clientX, clientY };
    ClientToScreen(__hwnd, &p);
    SetCursorPos(p.x, p.y);
}


/**
 * Gets the width and height of the screen in pixels.
 */
static void getDesktopResolution(int& width, int& height)
{
   RECT desktop;
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   width = desktop.right;
   height = desktop.bottom;
}

LRESULT CALLBACK __WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static gameplay::Game* game = gameplay::Game::getInstance();

    if (!game->isInitialized() || hwnd != __hwnd)
    {
        // Ignore messages that are not for our game window.
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    static bool shiftDown = false;
    static bool capsOn = false;

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(__hwnd);
        return 0;

    case WM_DESTROY:
        gameplay::Platform::shutdownInternal();
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        UpdateCapture(wParam);
        if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON, x, y, 0))
        {
            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, x, y, 0, true);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON, x, y, 0))
        {
            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, x, y, 0, true);
        }
        UpdateCapture(wParam);
        return 0;
    }
    case WM_RBUTTONDOWN:
        UpdateCapture(wParam);
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_RIGHT_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
        break;

    case WM_RBUTTONUP:
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_RIGHT_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
        UpdateCapture(wParam);
        break;

    case WM_MBUTTONDOWN:
        UpdateCapture(wParam);
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_MIDDLE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
        break;

    case WM_MBUTTONUP:
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_MIDDLE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
        UpdateCapture(wParam);
        break;

    case WM_MOUSEMOVE:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (__mouseCaptured)
        {
            // If the incoming position is the mouse capture point, ignore this event
            // since this is the event that warped the cursor back.
            if (x == __mouseCapturePoint.x && y == __mouseCapturePoint.y)
                break;

            // Convert to deltas
            x -= __mouseCapturePoint.x;
            y -= __mouseCapturePoint.y;

            // Warp mouse back to center of screen.
            WarpMouse(__mouseCapturePoint.x, __mouseCapturePoint.y);
        }

        // Allow Game::mouseEvent a chance to handle (and possibly consume) the event.
        if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, x, y, 0))
        {
            if ((wParam & MK_LBUTTON) == MK_LBUTTON)
            {
                // Mouse move events should be interpreted as touch move only if left mouse is held and the game did not consume the mouse event.
                gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0, true);
                return 0;
            }
        }
        break;
    }

    case WM_MOUSEWHEEL:
        tagPOINT point;
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);
        ScreenToClient(__hwnd, &point);
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL, point.x, point.y, GET_WHEEL_DELTA_WPARAM(wParam) / 120);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
            shiftDown = true;

        if (wParam == VK_CAPITAL)
            capsOn = !capsOn;

        // Suppress key repeats.
        if ((lParam & 0x40000000) == 0)
            gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_PRESS, getKey(wParam, shiftDown ^ capsOn));
        break;
        
    case WM_KEYUP:
        if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
            shiftDown = false;

        gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, getKey(wParam, shiftDown ^ capsOn));
        break;

    case WM_CHAR:
        // Suppress key repeats.
        if ((lParam & 0x40000000) == 0)
            gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, wParam);
        break;

    case WM_UNICHAR:
        // Suppress key repeats.
        if ((lParam & 0x40000000) == 0)
            gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, wParam);
        break;

    case WM_SETFOCUS:
        break;

    case WM_KILLFOCUS:
        break;

    case WM_SIZE:
        // Window was resized.
        gameplay::Platform::resizeEventInternal((unsigned int)(short)LOWORD(lParam), (unsigned int)(short)HIWORD(lParam));
        break;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam); 
}


namespace gameplay
{

struct WindowCreationParams
{
    RECT rect;
    std::wstring windowName;
    bool fullscreen;
    bool resizable;
    int samples;
};

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
        SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
}

Platform::Platform(Game* game)
    : _game(game)
{
}

Platform::~Platform()
{
    if (__hwnd)
    {
        DestroyWindow(__hwnd);
        __hwnd = 0;
    }
}

bool createWindow(WindowCreationParams* params, HWND* hwnd, HDC* hdc)
{
    bool fullscreen = false;
    bool resizable = false;
    RECT rect = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
    std::wstring windowName;
    if (params)
    {
        windowName = params->windowName;
        memcpy(&rect, &params->rect, sizeof(RECT));
        fullscreen = params->fullscreen;
        resizable = params->resizable;
    }

    // Set the window style.
    DWORD style, styleEx;
    if (fullscreen)
    {
        style = WS_POPUP;
        styleEx = WS_EX_APPWINDOW;
    }
    else
    {
        if (resizable)
            style = WS_OVERLAPPEDWINDOW;
        else
            style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;
        styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }
    style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    // Adjust the window rectangle so the client size is the requested size.
    AdjustWindowRectEx(&rect, style, FALSE, styleEx);

    // Create the native Windows window.
    *hwnd = CreateWindowEx(styleEx, L"gameplay", windowName.c_str(), style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, __hinstance, NULL);
    if (*hwnd == NULL)
    {
        GP_ERROR("Failed to create window.");
        return false;
    }

    // Get the drawing context.
    *hdc = GetDC(*hwnd);
    if (*hdc == NULL)
    {
        GP_ERROR("Failed to get device context.");
        return false;
    }

    // Center the window
    GetWindowRect(*hwnd, &rect);
    const int screenX = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
    const int screenY = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
    SetWindowPos(*hwnd, *hwnd, screenX, screenY, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    return true;
}

bool initializeGL(WindowCreationParams* params)
{
    // Create a temporary window and context to we can initialize GLEW and get access
    // to additional OpenGL extension functions. This is a neccessary evil since the
    // function for querying GL extensions is a GL extension itself.
    HWND hwnd = NULL;
    HDC hdc = NULL;

    if (params)
    {
        if (!createWindow(NULL, &hwnd, &hdc))
            return false;
    }
    else
    {
        hwnd = __hwnd;
        hdc = __hdc;
    }

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = DEFAULT_COLOR_BUFFER_SIZE;
    pfd.cDepthBits = DEFAULT_DEPTH_BUFFER_SIZE;
    pfd.cStencilBits = DEFAULT_STENCIL_BUFFER_SIZE;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0)
    {
        DestroyWindow(hwnd);
        GP_ERROR("Failed to choose a pixel format.");
        return false;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        DestroyWindow(hwnd);
        GP_ERROR("Failed to set the pixel format.");
        return false;
    }

    HGLRC tempContext = wglCreateContext(hdc);
    if (!tempContext)
    {
        DestroyWindow(hwnd);
        GP_ERROR("Failed to create temporary context for initialization.");
        return false;
    }
    wglMakeCurrent(hdc, tempContext);

    // Initialize GLEW
    if (GLEW_OK != glewInit())
    {
        wglDeleteContext(tempContext);
        DestroyWindow(hwnd);
        GP_ERROR("Failed to initialize GLEW.");
        return false;
    }

    // Choose pixel format using wglChoosePixelFormatARB, which allows us to specify
    // additional attributes such as multisampling.
    //
    // Note: Keep multisampling attributes at the start of the attribute lists since code below
    // assumes they are array elements 0 through 3.
    int attribList[] = {
        WGL_SAMPLES_ARB, params ? params->samples : 0,
        WGL_SAMPLE_BUFFERS_ARB, params ? (params->samples > 0 ? 1 : 0) : 0,
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, DEFAULT_COLOR_BUFFER_SIZE,
        WGL_DEPTH_BITS_ARB, DEFAULT_DEPTH_BUFFER_SIZE,
        WGL_STENCIL_BITS_ARB, DEFAULT_STENCIL_BUFFER_SIZE,
        0
    };
    __multiSampling = params && params->samples > 0;

    UINT numFormats;
    if ( !wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) || numFormats == 0)
    {
        bool valid = false;
        if (params && params->samples > 0)
        {
            GP_WARN("Failed to choose pixel format with WGL_SAMPLES_ARB == %d. Attempting to fallback to lower samples setting.", params->samples);
            while (params->samples > 0)
            {
                params->samples /= 2;
                attribList[1] = params->samples;
                attribList[3] = params->samples > 0 ? 1 : 0;
                if (wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) && numFormats > 0)
                {
                    valid = true;
                    GP_WARN("Found pixel format with WGL_SAMPLES_ARB == %d.", params->samples);
                    break;
                }
            }

            __multiSampling = params->samples > 0;
        }

        if (!valid)
        {
            wglDeleteContext(tempContext);
            DestroyWindow(hwnd);
            GP_ERROR("Failed to choose a pixel format.");
            return false;
        }
    }

    // Create new/final window if needed
    if (params)
    {
        DestroyWindow(hwnd);
        hwnd = NULL;
        hdc = NULL;

        if (!createWindow(params, &__hwnd, &__hdc))
        {
            wglDeleteContext(tempContext);
            return false;
        }
    }

    // Set final pixel format for window
    if (!SetPixelFormat(__hdc, pixelFormat, &pfd))
    {
        GP_ERROR("Failed to set the pixel format: %d.", (int)GetLastError());
        return false;
    }

    // Create our new GL context
    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        0
    };

    if (!(__hrc = wglCreateContextAttribsARB(__hdc, 0, attribs) ) )
    {
        wglDeleteContext(tempContext);
        GP_ERROR("Failed to create OpenGL context.");
        return false;
    }

    // Delete the old/temporary context and window
    wglDeleteContext(tempContext);

    // Make the new context current
    if (!wglMakeCurrent(__hdc, __hrc) || !__hrc)
    {
        GP_ERROR("Failed to make the window current.");
        return false;
    }

    // Vertical sync.
    wglSwapIntervalEXT(__vsync ? 1 : 0);

    return true;
}

Platform* Platform::create(Game* game, void* attachToWindow)
{
    GP_ASSERT(game);

    FileSystem::setResourcePath("./");
    Platform* platform = new Platform(game);

    // Get the application module handle.
    __hinstance = ::GetModuleHandle(NULL);

    __attachToWindow = (HWND)attachToWindow;

    // Read window settings from config.
    WindowCreationParams params;
    params.fullscreen = false;
    params.resizable = false;
    params.rect.left = 0;
    params.rect.top = 0;
    params.rect.right = 0;
    params.rect.bottom = 0;
    params.samples = 0;
    if (game->getConfig())
    {
        Properties* config = game->getConfig()->getNamespace("window", true);
        if (config)
        {
            // Read window title.
            const char* title = config->getString("title");
            if (title)
            {
                int len = MultiByteToWideChar(CP_ACP, 0, title, -1, NULL, 0);
                wchar_t* wtitle = new wchar_t[len];
                MultiByteToWideChar(CP_ACP, 0, title, -1, wtitle, len);
                params.windowName = wtitle;
                SAFE_DELETE_ARRAY(wtitle);
            }

            // Read fullscreen state.
            params.fullscreen = config->getBool("fullscreen");
            // Read resizable state.
            params.resizable = config->getBool("resizable");
            // Read multisampling state.
            params.samples = config->getInt("samples");

            // Read window rect.
            int x = config->getInt("x");
            if (x != 0)
                params.rect.left = x;
            int y = config->getInt("y");
            if (y != 0)
                params.rect.top = y;
            int width = config->getInt("width");
            int height = config->getInt("height");

            if (width == 0 && height == 0 && params.fullscreen)
                getDesktopResolution(width, height);

            if (width != 0)
                params.rect.right = params.rect.left + width;
            if (height != 0)
                params.rect.bottom = params.rect.top + height;
        }
    }

    // If window size was not specified, set it to a default value
    if (params.rect.right == 0)
        params.rect.right = params.rect.left + DEFAULT_RESOLUTION_X;
    if (params.rect.bottom == 0)
        params.rect.bottom = params.rect.top + DEFAULT_RESOLUTION_Y;
    int width = params.rect.right - params.rect.left;
    int height = params.rect.bottom - params.rect.top;

    if (params.fullscreen)
    {
        // Enumerate all supposed display settings
        bool modeSupported = false;
        DWORD modeNum = 0;
        DEVMODE devMode;
        memset(&devMode, 0, sizeof(DEVMODE));
        devMode.dmSize = sizeof(DEVMODE);
        devMode.dmDriverExtra = 0;
        while (EnumDisplaySettings(NULL, modeNum++, &devMode) != 0)
        {
            // Is mode supported?
            if (devMode.dmPelsWidth == width &&
                devMode.dmPelsHeight == height &&
                devMode.dmBitsPerPel == DEFAULT_COLOR_BUFFER_SIZE)
            {
                modeSupported = true;
                break;
            }
        }

        // If the requested mode is not supported, fall back to a safe default
        if (!modeSupported)
        {
            width = DEFAULT_RESOLUTION_X;
            height = DEFAULT_RESOLUTION_Y;
            params.rect.right = params.rect.left + width;
            params.rect.bottom = params.rect.top + height;
        }
    }

    if (!__attachToWindow)
    {
        // Register our window class.
        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc    = (WNDPROC)__WndProc;
        wc.cbClsExtra     = 0;
        wc.cbWndExtra     = 0;
        wc.hInstance      = __hinstance;
        wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
        wc.hIconSm        = NULL;
        wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground  = NULL;  // No brush - we are going to paint our own background
        wc.lpszMenuName   = NULL;  // No default menu
        wc.lpszClassName  = L"gameplay";

        if (!::RegisterClassEx(&wc))
        {
            GP_ERROR("Failed to register window class.");
            goto error;
        }

        if (params.fullscreen)
        {
            DEVMODE dm;
            memset(&dm, 0, sizeof(dm));
            dm.dmSize= sizeof(dm);
            dm.dmPelsWidth  = width;
            dm.dmPelsHeight = height;
            dm.dmBitsPerPel = DEFAULT_COLOR_BUFFER_SIZE;
            dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try to set selected mode and get results. NOTE: CDS_FULLSCREEN gets rid of start bar.
            if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                params.fullscreen = false;
                GP_ERROR("Failed to start game in full-screen mode with resolution %dx%d.", width, height);
                goto error;
            }
        }

        if (!initializeGL(&params))
            goto error;

        // Show the window.
        ShowWindow(__hwnd, SW_SHOW);
    }
    else
    {
        // Attach to a previous windows
        __hwnd = (HWND)__attachToWindow;
        __hdc = GetDC(__hwnd);

        SetWindowLongPtr(__hwnd, GWLP_WNDPROC, (LONG)(WNDPROC)__WndProc);

        if (!initializeGL(NULL))
            goto error;
    }

#ifdef USE_XINPUT
    // Initialize XInputGamepads.
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        if (XInputGetState(i, &__xInputState) == NO_ERROR)
        {
            if (!__connectedXInput[i])
            {
                // Gamepad is connected.
                Platform::gamepadEventConnectedInternal(i, XINPUT_BUTTON_COUNT, XINPUT_JOYSTICK_COUNT, XINPUT_TRIGGER_COUNT, 0, 0, "Microsoft", "XBox360 Controller");
                __connectedXInput[i] = true;
            }
        }
    }
#endif

    return platform;

error:

    exit(0);
    return NULL;
}

int Platform::enterMessagePump()
{
    GP_ASSERT(_game);

    // Get the initial time.
    LARGE_INTEGER tps;
    QueryPerformanceFrequency(&tps);
    __timeTicksPerMillis = (double)(tps.QuadPart / 1000L);
    LARGE_INTEGER queryTime;
    QueryPerformanceCounter(&queryTime);
    GP_ASSERT(__timeTicksPerMillis);
    __timeStart = queryTime.QuadPart / __timeTicksPerMillis;

    SwapBuffers(__hdc);

    if (_game->getState() != Game::RUNNING)
        _game->run();

    if (__attachToWindow)
        return 0;

    // Enter event dispatch loop.
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                gameplay::Platform::shutdownInternal();
                return msg.wParam;
            }
        }
        else
        {
#ifdef USE_XINPUT
            // Check for connected XInput gamepads.
            for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
            {
                if (XInputGetState(i, &__xInputState) == NO_ERROR && !__connectedXInput[i])
                {
                    // Gamepad was just connected.
                    Platform::gamepadEventConnectedInternal(i, XINPUT_BUTTON_COUNT, XINPUT_JOYSTICK_COUNT, XINPUT_TRIGGER_COUNT, 0, 0, "Microsoft", "XBox360 Controller");
                    __connectedXInput[i] = true;
                }
                else if (XInputGetState(i, &__xInputState) != NO_ERROR && __connectedXInput[i])
                {
                    // Gamepad was just disconnected.
                    __connectedXInput[i] = false;
                    Platform::gamepadEventDisconnectedInternal(i);
                }
            }
#endif
            _game->frame();
            SwapBuffers(__hdc);
        }

        // If we are done, then exit.
        if (_game->getState() == Game::UNINITIALIZED)
            break;
    }
    return 0;
}

void Platform::signalShutdown() 
{
    // nothing to do  
}

bool Platform::canExit()
{
    return true;
}

unsigned int Platform::getDisplayWidth()
{
    static RECT rect;
    GetClientRect(__hwnd, &rect);
    return rect.right;
}

unsigned int Platform::getDisplayHeight()
{
    static RECT rect;
    GetClientRect(__hwnd, &rect);
    return rect.bottom;
}

double Platform::getAbsoluteTime()
{
    LARGE_INTEGER queryTime;
    QueryPerformanceCounter(&queryTime);
    GP_ASSERT(__timeTicksPerMillis);
    __timeAbsolute = queryTime.QuadPart / __timeTicksPerMillis;

    return __timeAbsolute - __timeStart;
}

void Platform::setAbsoluteTime(double time)
{
    __timeAbsolute = time;
}

bool Platform::isVsync()
{
    return __vsync;
}

void Platform::setVsync(bool enable)
{
     wglSwapIntervalEXT(enable ? 1 : 0);
    __vsync = enable;
}

void Platform::swapBuffers()
{
    if (__hdc)
        SwapBuffers(__hdc);
}

void Platform::sleep(long ms)
{
    Sleep(ms);
}

void Platform::setMultiSampling(bool enabled)
{
    if (enabled == __multiSampling)
    {
        return;
    }

    if (enabled)
    {
        glEnable(GL_MULTISAMPLE);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
    }

    __multiSampling = enabled;
}

bool Platform::isMultiSampling()
{
    return __multiSampling;
}

void Platform::setMultiTouch(bool enabled)
{
    // not supported
}

bool Platform::isMultiTouch()
{
    return false;
}

bool Platform::hasAccelerometer()
{
    return false;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = 0;
    *roll = 0;
}

void Platform::getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    if (accelX)
    {
        *accelX = 0;
    }

    if (accelY)
    {
        *accelY = 0;
    }

    if (accelZ)
    {
        *accelZ = 0;
    }

    if (gyroX)
    {
        *gyroX = 0;
    }

    if (gyroY)
    {
        *gyroY = 0;
    }

    if (gyroZ)
    {
        *gyroZ = 0;
    }
}

void Platform::getArguments(int* argc, char*** argv)
{
    if (argc)
        *argc = __argc;
    if (argv)
        *argv = __argv;
}

bool Platform::hasMouse()
{
    return true;
}

void Platform::setMouseCaptured(bool captured)
{
    if (captured != __mouseCaptured)
    {
        if (captured)
        {
            // Hide the cursor and warp it to the center of the screen
            __mouseCapturePoint.x = getDisplayWidth() / 2;
            __mouseCapturePoint.y = getDisplayHeight() / 2;

            ShowCursor(FALSE);
            WarpMouse(__mouseCapturePoint.x, __mouseCapturePoint.y);
        }
        else
        {
            // Restore cursor
            WarpMouse(__mouseCapturePoint.x, __mouseCapturePoint.y);
            ShowCursor(TRUE);
        }

        __mouseCaptured = captured;
    }
}

bool Platform::isMouseCaptured()
{
    return __mouseCaptured;
}

void Platform::setCursorVisible(bool visible)
{
    if (visible != __cursorVisible)
    {
        ShowCursor(visible ? TRUE : FALSE);
        __cursorVisible = visible;
    }
}

bool Platform::isCursorVisible()
{
    return __cursorVisible;
}

void Platform::displayKeyboard(bool display)
{
    // Do nothing.
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
}
    
bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return false;
}

#ifdef USE_XINPUT
void Platform::pollGamepadState(Gamepad* gamepad)
{
    GP_ASSERT(gamepad->_handle < XUSER_MAX_COUNT);

    if (XInputGetState(gamepad->_handle, &__xInputState) == NO_ERROR)
    {
        WORD buttons = __xInputState.Gamepad.wButtons;

        // Map XInput buttons to Gamepad::ButtonMappings enum.
        static const unsigned int xInputMapping[16] = {
            Gamepad::BUTTON_UP,    // 0x0001
            Gamepad::BUTTON_DOWN,  // 0x0002
            Gamepad::BUTTON_LEFT,  // 0x0004
            Gamepad::BUTTON_RIGHT, // 0x0008
            Gamepad::BUTTON_MENU2, // 0x0010
            Gamepad::BUTTON_MENU1, // 0x0020
            Gamepad::BUTTON_L3,    // 0x0040
            Gamepad::BUTTON_R3,    // 0x0080
            Gamepad::BUTTON_L1,    // 0x0100
            Gamepad::BUTTON_R1,    // 0x0200
            0,
            0,
            Gamepad::BUTTON_A,     // 0x1000
            Gamepad::BUTTON_B,     // 0x2000
            Gamepad::BUTTON_X,     // 0x4000
            Gamepad::BUTTON_Y      // 0x8000
        };

        const unsigned int *mapping = xInputMapping;
        unsigned int mappedButtons;
        for (mappedButtons = 0; buttons; buttons >>= 1, mapping++)
        {
            if (buttons & 1)
            {
                mappedButtons |= (1 << *mapping);
            }
        }
        gamepad->setButtons(mappedButtons);

        unsigned int i;
        for (i = 0; i < gamepad->_joystickCount; ++i)
        {
            GP_ASSERT(i < 2);

            float x;
            float y;
            switch (i)
            {
            case 0:
                x = normalizeXInputJoystickAxis(__xInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                y = normalizeXInputJoystickAxis(__xInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                break;
            case 1:
                x = normalizeXInputJoystickAxis(__xInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                y = normalizeXInputJoystickAxis(__xInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                break;
            }

            gamepad->setJoystickValue(i, x, y);
        }

        for (i = 0; i < gamepad->_triggerCount; ++i)
        {
            GP_ASSERT(i < 2);

            BYTE trigger;
            switch (i)
            {
            case 0:
                trigger = __xInputState.Gamepad.bLeftTrigger;
                break;
            case 1:
                trigger = __xInputState.Gamepad.bRightTrigger;
                break;
            }

            if (trigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            {
                gamepad->setTriggerValue(i, 0.0f);
            }
            else
            {
                gamepad->setTriggerValue(i, (float)trigger / 255.0f);
            }
        }
    }
}
#else
void Platform::pollGamepadState(Gamepad* gamepad) { }
#endif

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

bool Platform::launchURL(const char* url)
{
    if (url == NULL || *url == '\0')
        return false;
 
    // Success when result code > 32
    int len = MultiByteToWideChar(CP_ACP, 0, url, -1, NULL, 0);
    wchar_t* wurl = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, url, -1, wurl, len);
    int r = (int)ShellExecute(NULL, NULL, wurl, NULL, NULL, SW_SHOWNORMAL);
    SAFE_DELETE_ARRAY(wurl);
    return (r > 32);
}

}

#endif
