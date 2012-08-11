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
#ifdef USE_XINPUT
#include <XInput.h>
#endif

using gameplay::print;

// Default to 720p
static int __width = 1280;
static int __height = 720;

static double __timeTicksPerMillis;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __roll;
static float __pitch;
static HINSTANCE __hinstance = 0;
static HWND __attachToWindow = 0;
static HWND __hwnd = 0;
static HDC __hdc = 0;
static HGLRC __hrc = 0;
static bool __mouseCaptured = false;
static POINT __mouseCapturePoint = { 0, 0 };
static bool __cursorVisible = true;
static unsigned int __gamepadsConnected = 0;

#ifdef USE_XINPUT
struct XInputGamepad
{
    static const unsigned int BUTTON_COUNT = 14;
    static const unsigned int JOYSTICK_COUNT = 2;
    static const unsigned int TRIGGER_COUNT = 2;
    std::string id;
    int handle;
    bool connected;
    XINPUT_STATE state;
};

static XInputGamepad* __xinputGamepads = NULL;

static DWORD getXInputGamepadButtonMask(unsigned int buttonHandle)
{
    switch(buttonHandle)
    {
    case 0:
        return XINPUT_GAMEPAD_DPAD_UP;
    case 1:
        return XINPUT_GAMEPAD_DPAD_DOWN;
    case 2:
        return XINPUT_GAMEPAD_DPAD_LEFT;
    case 3:
        return XINPUT_GAMEPAD_DPAD_RIGHT;
    case 4:
        return XINPUT_GAMEPAD_START;
    case 5:
        return XINPUT_GAMEPAD_BACK;
    case 6:
        return XINPUT_GAMEPAD_LEFT_THUMB;
    case 7:
        return XINPUT_GAMEPAD_RIGHT_THUMB;
    case 8:
        return XINPUT_GAMEPAD_LEFT_SHOULDER;
    case 9:
        return XINPUT_GAMEPAD_RIGHT_SHOULDER;
    case 10:
        return XINPUT_GAMEPAD_A;
    case 11:
        return XINPUT_GAMEPAD_B;
    case 12:
        return XINPUT_GAMEPAD_X;
    case 13:
        return XINPUT_GAMEPAD_Y;
    default:
        return 0;
    }
}

static bool getXInputState(unsigned long gamepadHandle)
{
    GP_ASSERT(0 <= gamepadHandle && gamepadHandle < XUSER_MAX_COUNT);

    if (XInputGetState((DWORD)gamepadHandle, &__xinputGamepads[gamepadHandle].state) == ERROR_SUCCESS)
        return (__xinputGamepads[gamepadHandle].connected = true);
    else
        return (__xinputGamepads[gamepadHandle].connected = false);
}

static bool getXInputButtonState(unsigned long gamepadHandle, unsigned long buttonHandle)
{
    GP_ASSERT(0 <= gamepadHandle && gamepadHandle < XUSER_MAX_COUNT);
    GP_ASSERT(0 <= buttonHandle && buttonHandle < 14);
    
    WORD buttonMask = getXInputGamepadButtonMask(buttonHandle); // Conversion to button mask.

    if ((__xinputGamepads[gamepadHandle].state.Gamepad.wButtons & buttonMask) == buttonMask)
        return true;
    else
        return false;
}

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
        if (value < 0)
            value = -1;
        else if (value > 0)
            value = 1;
        else
            value = 0;
        return value * (absAxisValue - deadZone) / (float)(32768 - deadZone);
    }
}

static float getXInputJoystickAxisX(unsigned long gamepadHandle, unsigned long joystickHandle)
{
    GP_ASSERT(0 <= gamepadHandle && gamepadHandle < XUSER_MAX_COUNT);
    GP_ASSERT(0 <= joystickHandle && joystickHandle < 2);
    
    switch(joystickHandle)
    {
    case 0:
        return normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    case 1:
        return normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    default: return 0.0f;
    }
}

static float getXInputJoystickAxisY(unsigned long gamepadHandle, unsigned long joystickHandle)
{
    GP_ASSERT(0 <= gamepadHandle && gamepadHandle < XUSER_MAX_COUNT);
    GP_ASSERT(0 <= joystickHandle && joystickHandle < 2);

    switch(joystickHandle)
    {
    case 0:
        return normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    case 1:
        return normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    default: return 0.0f;
    }
}

static void getXInputJoystickAxisValues(unsigned long gamepadHandle, unsigned long joystickHandle, gameplay::Vector2* outValue)
{
    GP_ASSERT(0 <= gamepadHandle && gamepadHandle < XUSER_MAX_COUNT);
    GP_ASSERT(0 <= joystickHandle && joystickHandle < 2);

    switch(joystickHandle)
    {
    case 0:
        outValue->x = normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        outValue->y = normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        break;
    case 1:
        outValue->x = normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        outValue->y = normalizeXInputJoystickAxis(__xinputGamepads[gamepadHandle].state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        break;
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

void UpdateCapture(LPARAM lParam)
{
    if ((lParam & MK_LBUTTON) || (lParam & MK_MBUTTON) || (lParam & MK_RBUTTON))
        SetCapture(__hwnd);
    else
        ReleaseCapture();
}

void WarpMouse(int clientX, int clientY)
{
    POINT p = { clientX, clientY };
    ClientToScreen(__hwnd, &p);
    SetCursorPos(p.x, p.y);
}

LRESULT CALLBACK __WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static gameplay::Game* game = gameplay::Game::getInstance();

    if (!game->isInitialized() || hwnd != __hwnd)
    {
        // Ignore messages that are not for our game window.
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // Scale factors for the mouse movement used to simulate the accelerometer.
    GP_ASSERT(__width);
    GP_ASSERT(__height);
    static const float ACCELEROMETER_X_FACTOR = 90.0f / __width;
    static const float ACCELEROMETER_Y_FACTOR = 90.0f / __height;

    static int lx, ly;

    static bool shiftDown = false;
    static bool capsOn = false;

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(__hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        UpdateCapture(wParam);
        if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON, x, y, 0))
        {
            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, x, y, 0);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON, x, y, 0))
        {
            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, x, y, 0);
        }
        UpdateCapture(wParam);
        return 0;
    }
    case WM_RBUTTONDOWN:
        UpdateCapture(wParam);
        lx = GET_X_LPARAM(lParam);
        ly = GET_Y_LPARAM(lParam);
        gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_PRESS_RIGHT_BUTTON, lx, ly, 0);
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
                gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0);
                return 0;
            }
            else if ((wParam & MK_RBUTTON) == MK_RBUTTON)
            {
                // Update the pitch and roll by adding the scaled deltas.
                __roll += (float)(x - lx) * ACCELEROMETER_X_FACTOR;
                __pitch += -(float)(y - ly) * ACCELEROMETER_Y_FACTOR;

                // Clamp the values to the valid range.
                __roll = max(min(__roll, 90.0f), -90.0f);
                __pitch = max(min(__pitch, 90.0f), -90.0f);

                // Update the last X/Y values.
                lx = x;
                ly = y;
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
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam); 
}


namespace gameplay
{

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
#ifdef USE_XINPUT
    SAFE_DELETE_ARRAY(__xinputGamepads);
#endif
}

bool initializeGL()
{
    // Choose pixel format. 32-bit. RGBA.
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(__hdc, &pfd);

    if (pixelFormat == 0)
    {
        GP_ERROR("Failed to choose a pixel format.");
        return false;
    }
    if (!SetPixelFormat (__hdc, pixelFormat, &pfd))
    {
        GP_ERROR("Failed to set the pixel format.");
        return false;
    }

    HGLRC tempContext = wglCreateContext(__hdc);
    wglMakeCurrent(__hdc, tempContext);

    if (GLEW_OK != glewInit())
    {
        GP_ERROR("Failed to initialize GLEW.");
        return false;
    }

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
    wglDeleteContext(tempContext);

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
    if (!__attachToWindow)
    {
        LPCTSTR windowClass = L"gameplay";
        std::wstring windowName;
        bool fullscreen = false;
    
        // Read window settings from config.
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
                    windowName = wtitle;
                    SAFE_DELETE_ARRAY(wtitle);
                }

                // Read window size.
                int width = config->getInt("width");
                if (width != 0)
                    __width = width;
                int height = config->getInt("height");
                if (height != 0)
                    __height = height;

                // Read fullscreen state.
                fullscreen = config->getBool("fullscreen");
            }
        }

        RECT rect = { 0, 0, __width, __height };

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
        wc.lpszClassName  = windowClass;

        if (!::RegisterClassEx(&wc))
        {
            GP_ERROR("Failed to register window class.");
            goto error;
        }

        if (fullscreen)
        {
            DEVMODE dm;
            memset(&dm, 0, sizeof(dm));
            dm.dmSize= sizeof(dm);
            dm.dmPelsWidth	= __width;
            dm.dmPelsHeight	= __height;
            dm.dmBitsPerPel	= 32;
            dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try to set selected mode and get results. NOTE: CDS_FULLSCREEN gets rid of start bar.
            if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                fullscreen = false;
                GP_ERROR("Failed to start game in full-screen mode with resolution %dx%d.", __width, __height);
                goto error;
            }
        }

        // Set the window style.
        DWORD style   = (fullscreen ? WS_POPUP : WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        DWORD styleEx = (fullscreen ? WS_EX_APPWINDOW : WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

        // Adjust the window rectangle so the client size is the requested size.
        AdjustWindowRectEx(&rect, style, FALSE, styleEx);

        // Create the native Windows window.
        __hwnd = CreateWindowEx(styleEx, windowClass, windowName.c_str(), style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, __hinstance, NULL);

        // Get the drawing context.
        __hdc = GetDC(__hwnd);

        // Center the window
        const int screenX = (GetSystemMetrics(SM_CXSCREEN) - __width) / 2;
        const int screenY = (GetSystemMetrics(SM_CYSCREEN) - __height) / 2;
        ::SetWindowPos(__hwnd, __hwnd, screenX, screenY, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

        if (!initializeGL())
            goto error;

        // Show the window.
        ShowWindow(__hwnd, SW_SHOW);
    }
    else
    {
        // Attach to a previous windows
        __hwnd = (HWND)__attachToWindow;
        __hdc = GetDC(__hwnd);

        SetWindowLongPtr(__hwnd, GWL_WNDPROC, (LONG)(WNDPROC)__WndProc);
        
        if (!initializeGL())
            goto error;
    }

#ifdef USE_XINPUT
    // Initialize XInputGamepads.
    __xinputGamepads = new XInputGamepad[XUSER_MAX_COUNT];
    for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        switch(i)
        {
        case 0:
            __xinputGamepads[i].id = "XINPUT 1";
            break;
        case 1:
            __xinputGamepads[i].id = "XINPUT 2";
            break;
        case 2:
            __xinputGamepads[i].id = "XINPUT 3";
            break;
        case 3:
            __xinputGamepads[i].id = "XINPUT 4";
            break;
        }
        __xinputGamepads[i].connected = false;
        __xinputGamepads[i].handle = -1;
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
    int rc = 0;

    // Get the initial time.
    LARGE_INTEGER tps;
    QueryPerformanceFrequency(&tps);
    __timeTicksPerMillis = (double)(tps.QuadPart / 1000L);
    LARGE_INTEGER queryTime;
    QueryPerformanceCounter(&queryTime);
    GP_ASSERT(__timeTicksPerMillis);
    __timeStart = queryTime.QuadPart / __timeTicksPerMillis;

    // Set the initial pitch and roll values.
    __pitch = 0.0;
    __roll = 0.0;

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
                _game->exit();
                break;
            }
        }
        else
        {
            _game->frame();
            SwapBuffers(__hdc);
        }

        // If we are done, then exit.
        if (_game->getState() == Game::UNINITIALIZED)
            break;
    }
    return msg.wParam;
}

void Platform::signalShutdown() 
{
    // nothing to do  
}

unsigned int Platform::getDisplayWidth()
{
    return __width;
}

unsigned int Platform::getDisplayHeight()
{
    return __height;
}

double Platform::getAbsoluteTime()
{
    LARGE_INTEGER queryTime;
    QueryPerformanceCounter(&queryTime);
    GP_ASSERT(__timeTicksPerMillis);
    __timeAbsolute = queryTime.QuadPart / __timeTicksPerMillis;

    return __timeAbsolute;
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

void Platform::setMultiTouch(bool enabled)
{
    // not supported
}

bool Platform::isMultiTouch()
{
    return false;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = __pitch;
    *roll = __roll;
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

void Platform::swapBuffers()
{
    if (__hdc)
        SwapBuffers(__hdc);
}

void Platform::displayKeyboard(bool display)
{
    // Do nothing.
}

unsigned int Platform::getGamepadsConnected()
{
    // Check to see what xbox360 gamepads are connected.
    __gamepadsConnected = 0;

#ifdef USE_XINPUT
    for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        if (isGamepadConnected(i))
            __gamepadsConnected++;
    }    
#endif
    return __gamepadsConnected;
}

bool Platform::isGamepadConnected(unsigned int gamepadHandle)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);
    Game* game = Game::getInstance();
    GP_ASSERT(game);

    if (__xinputGamepads[gamepadHandle].handle == -1)
        __xinputGamepads[gamepadHandle].handle = game->createGamepad(__xinputGamepads[gamepadHandle].id.c_str(), gamepadHandle, __xinputGamepads[gamepadHandle].BUTTON_COUNT, 
            __xinputGamepads[gamepadHandle].JOYSTICK_COUNT, __xinputGamepads[gamepadHandle].TRIGGER_COUNT);

    bool isConnected = __xinputGamepads[gamepadHandle].connected;
    if (getXInputState(gamepadHandle))
    {
        if (!isConnected)
        {
            __xinputGamepads[gamepadHandle].connected = true;
            
            Gamepad* gamepad = game->getGamepad(__xinputGamepads[gamepadHandle].handle);
            GP_ASSERT(gamepad);
            if (game->isInitialized())
                game->gamepadEvent(Gamepad::CONNECTED_EVENT, game->getGamepad(__xinputGamepads[gamepadHandle].handle)); 
        }
        return true;
    }
    else
    {
        if (isConnected)
        {
            // if it was connected, but now isn't pass the detached event to gamepadEvent()
            __xinputGamepads[gamepadHandle].connected = false;
            
            Gamepad* gamepad = game->getGamepad(__xinputGamepads[gamepadHandle].handle);
            GP_ASSERT(gamepad);
            if (game->isInitialized())
                game->gamepadEvent(Gamepad::DISCONNECTED_EVENT, game->getGamepad(__xinputGamepads[gamepadHandle].handle));
        }
        return false;
    }
#endif

    return false;
}

const char* Platform::getGamepadId(unsigned int gamepadHandle) 
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    return __xinputGamepads[gamepadHandle].id.c_str();
#endif
    return NULL;
}

unsigned int Platform::getGamepadButtonCount(unsigned int gamepadHandle)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);
    if (!__xinputGamepads[gamepadHandle].connected)
        return 0;

    return XInputGamepad::BUTTON_COUNT;
#endif

    return 0;
}

bool Platform::getGamepadButtonState(unsigned int gamepadHandle, unsigned int buttonIndex)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);
    return getXInputButtonState(gamepadHandle, buttonIndex);
#endif

    return false;
}

unsigned int Platform::getGamepadJoystickCount(unsigned int gamepadHandle)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    if (!__xinputGamepads[gamepadHandle].connected)
        return 0;

    return XInputGamepad::JOYSTICK_COUNT;
#endif

    return 0;
}

float Platform::getGamepadJoystickAxisX(unsigned int gamepadHandle, unsigned int joystickIndex)
{
    
#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    return getXInputJoystickAxisX(gamepadHandle, joystickIndex);
#endif 

    return 0.0f;
}

float Platform::getGamepadJoystickAxisY(unsigned int gamepadHandle, unsigned int joystickIndex)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    return getXInputJoystickAxisY(gamepadHandle, joystickIndex);
#endif

    return 0.0f;
}

void Platform::getGamepadJoystickAxisValues(unsigned int gamepadHandle, unsigned int joystickIndex, Vector2* outValue)
{
    
#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    getXInputJoystickAxisValues(gamepadHandle, joystickIndex, outValue);
#endif

}

bool Platform::isGamepadJoystickActive(unsigned int gamepadHandle, unsigned int joystickIndex)
{
    
#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    return (getXInputJoystickAxisX(gamepadHandle, joystickIndex) != 0.0f || getXInputJoystickAxisY(gamepadHandle, joystickIndex) != 0.0f);
#endif 

    return false;
}

unsigned int Platform::getGamepadTriggerCount(unsigned int gamepadHandle)
{

#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);

    return XInputGamepad::TRIGGER_COUNT;
#endif

    return 0;
}

float Platform::getGamepadTriggerValue(unsigned int gamepadHandle, unsigned int triggerIndex)
{
    
#ifdef USE_XINPUT
    GP_ASSERT(0 <= gamepadHandle);
    GP_ASSERT(gamepadHandle < XUSER_MAX_COUNT);
    //TODO:
#endif
    return 0.0f;
}

void Platform::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!Form::touchEventInternal(evt, x, y, contactIndex))
    {
        Game::getInstance()->touchEvent(evt, x, y, contactIndex);
        Game::getInstance()->getScriptController()->touchEvent(evt, x, y, contactIndex);
    }
}

void Platform::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (!Form::keyEventInternal(evt, key))
    {
        Game::getInstance()->keyEvent(evt, key);
        Game::getInstance()->getScriptController()->keyEvent(evt, key);
    }
}

bool Platform::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
    {
        return true;
    }
    else if (Game::getInstance()->mouseEvent(evt, x, y, wheelDelta))
    {
        return true;
    }
    else
    {
        return Game::getInstance()->getScriptController()->mouseEvent(evt, x, y, wheelDelta);
    }
}

void Platform::sleep(long ms)
{
    Sleep(ms);
}

}

#endif
