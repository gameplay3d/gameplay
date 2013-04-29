#ifdef __QNX__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"
#include <unistd.h>
#include <sys/keycodes.h>
#include <screen/screen.h>
#include <input/screen_helpers.h>
#include <gestures/set.h>
#include <gestures/swipe.h>
#include <gestures/pinch.h>
#include <gestures/tap.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>
#include <bps/navigator.h>
#include <bps/sensor.h>
#include <bps/orientation.h>
#include <bps/virtualkeyboard.h>

#define TOUCH_COUNT_MAX     4

using namespace std;

int __argc = 0;
char** __argv = 0;
struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static screen_context_t __screenContext;
static screen_window_t __screenWindow;
static screen_event_t __screenEvent;
static int __screenWindowSize[2];
static bool __screenFullscreen = false;
static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
static EGLContext __eglContext = EGL_NO_CONTEXT;
static EGLSurface __eglSurface = EGL_NO_SURFACE;
static EGLConfig __eglConfig = 0;
static int __orientationAngle;
static bool __multiTouch = false;
static bool __multiSampling = false;
static float __pitch;
static float __roll;
static float __accelRawX;
static float __accelRawY;
static float __accelRawZ;
static float __gyroRawX;
static float __gyroRawY;
static float __gyroRawZ;
static const char* __glExtensions;
static struct gestures_set * __gestureSet;
static bitset<3> __gestureEventsProcessed;
static bool __gestureSwipeRecognized = false;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

namespace gameplay
{

// Gets the Keyboard::Key enumeration constant that corresponds to the given QNX key code.
static Keyboard::Key getKey(int qnxKeycode)
{
    switch (qnxKeycode)
    {
    case KEYCODE_SYSREQ:
        return Keyboard::KEY_SYSREQ;
    case KEYCODE_BREAK:
        return Keyboard::KEY_BREAK;
    case KEYCODE_MENU:
        return Keyboard::KEY_MENU;
    case KEYCODE_KP_ENTER:
        return Keyboard::KEY_KP_ENTER;
    case KEYCODE_PAUSE:
        return Keyboard::KEY_PAUSE;
    case KEYCODE_SCROLL_LOCK:
        return Keyboard::KEY_SCROLL_LOCK;
    case KEYCODE_PRINT:
        return Keyboard::KEY_PRINT;
    case KEYCODE_ESCAPE:
        return Keyboard::KEY_ESCAPE;
    case KEYCODE_BACKSPACE:
        return Keyboard::KEY_BACKSPACE;
    case KEYCODE_BACK_TAB:
        return Keyboard::KEY_BACK_TAB;
    case KEYCODE_TAB:
        return Keyboard::KEY_TAB;
    case KEYCODE_RETURN:
        return Keyboard::KEY_RETURN;
    case KEYCODE_CAPS_LOCK:
        return Keyboard::KEY_CAPS_LOCK;
    case KEYCODE_LEFT_SHIFT:
    case KEYCODE_RIGHT_SHIFT:
        return Keyboard::KEY_SHIFT;
    case KEYCODE_LEFT_CTRL:
    case KEYCODE_RIGHT_CTRL:
        return Keyboard::KEY_CTRL;
    case KEYCODE_LEFT_ALT:
    case KEYCODE_RIGHT_ALT:
        return Keyboard::KEY_ALT;
    case KEYCODE_LEFT_HYPER:
    case KEYCODE_RIGHT_HYPER:
        return Keyboard::KEY_HYPER;
    case KEYCODE_INSERT:
        return Keyboard::KEY_INSERT;
    case KEYCODE_HOME:
        return Keyboard::KEY_HOME;
    case KEYCODE_PG_UP:
        return Keyboard::KEY_PG_UP;
    case KEYCODE_DELETE:
        return Keyboard::KEY_DELETE;
    case KEYCODE_END:
        return Keyboard::KEY_END;
    case KEYCODE_PG_DOWN:
        return Keyboard::KEY_PG_DOWN;
    case KEYCODE_LEFT:
        return Keyboard::KEY_LEFT_ARROW;
    case KEYCODE_RIGHT:
        return Keyboard::KEY_RIGHT_ARROW;
    case KEYCODE_UP:
        return Keyboard::KEY_UP_ARROW;
    case KEYCODE_DOWN:
        return Keyboard::KEY_DOWN_ARROW;
    case KEYCODE_NUM_LOCK:
        return Keyboard::KEY_NUM_LOCK;
    case KEYCODE_KP_PLUS:
        return Keyboard::KEY_KP_PLUS;
    case KEYCODE_KP_MINUS:
        return Keyboard::KEY_KP_MINUS;
    case KEYCODE_KP_MULTIPLY:
        return Keyboard::KEY_KP_MULTIPLY;
    case KEYCODE_KP_DIVIDE:
        return Keyboard::KEY_KP_DIVIDE;
    case KEYCODE_KP_HOME:
        return Keyboard::KEY_KP_HOME;
    case KEYCODE_KP_UP:
        return Keyboard::KEY_KP_UP;
    case KEYCODE_KP_PG_UP:
        return Keyboard::KEY_KP_PG_UP;
    case KEYCODE_KP_LEFT:
        return Keyboard::KEY_KP_LEFT;
    case KEYCODE_KP_FIVE:
        return Keyboard::KEY_KP_FIVE;
    case KEYCODE_KP_RIGHT:
        return Keyboard::KEY_KP_RIGHT;
    case KEYCODE_KP_END:
        return Keyboard::KEY_KP_END;
    case KEYCODE_KP_DOWN:
        return Keyboard::KEY_KP_DOWN;
    case KEYCODE_KP_PG_DOWN:
        return Keyboard::KEY_KP_PG_DOWN;
    case KEYCODE_KP_INSERT:
        return Keyboard::KEY_KP_INSERT;
    case KEYCODE_KP_DELETE:
        return Keyboard::KEY_KP_DELETE;
    case KEYCODE_F1:
        return Keyboard::KEY_F1;
    case KEYCODE_F2:
        return Keyboard::KEY_F2;
    case KEYCODE_F3:
        return Keyboard::KEY_F3;
    case KEYCODE_F4:
        return Keyboard::KEY_F4;
    case KEYCODE_F5:
        return Keyboard::KEY_F5;
    case KEYCODE_F6:
        return Keyboard::KEY_F6;
    case KEYCODE_F7:
        return Keyboard::KEY_F7;
    case KEYCODE_F8:
        return Keyboard::KEY_F8;
    case KEYCODE_F9:
        return Keyboard::KEY_F9;
    case KEYCODE_F10:
        return Keyboard::KEY_F10;
    case KEYCODE_F11:
        return Keyboard::KEY_F11;
    case KEYCODE_F12:
        return Keyboard::KEY_F12;
    case KEYCODE_SPACE:
        return Keyboard::KEY_SPACE;
    case KEYCODE_RIGHT_PAREN:
        return Keyboard::KEY_RIGHT_PARENTHESIS;
    case KEYCODE_ZERO:
        return Keyboard::KEY_ZERO;
    case KEYCODE_EXCLAM:
        return Keyboard::KEY_EXCLAM;
    case KEYCODE_ONE:
        return Keyboard::KEY_ONE;
    case KEYCODE_AT:
        return Keyboard::KEY_AT;
    case KEYCODE_TWO:
        return Keyboard::KEY_TWO;
    case KEYCODE_NUMBER:
        return Keyboard::KEY_NUMBER;
    case KEYCODE_THREE:
        return Keyboard::KEY_THREE;
    case KEYCODE_DOLLAR:
        return Keyboard::KEY_DOLLAR;
    case KEYCODE_FOUR:
        return Keyboard::KEY_FOUR;
    case KEYCODE_PERCENT:
        return Keyboard::KEY_PERCENT;
    case KEYCODE_FIVE:
        return Keyboard::KEY_FIVE;
    case KEYCODE_CIRCUMFLEX:
        return Keyboard::KEY_CIRCUMFLEX;
    case KEYCODE_SIX:
        return Keyboard::KEY_SIX;
    case KEYCODE_AMPERSAND:
        return Keyboard::KEY_AMPERSAND;
    case KEYCODE_SEVEN:
        return Keyboard::KEY_SEVEN;
    case KEYCODE_ASTERISK:
        return Keyboard::KEY_ASTERISK;
    case KEYCODE_EIGHT:
        return Keyboard::KEY_EIGHT;
    case KEYCODE_LEFT_PAREN:
        return Keyboard::KEY_LEFT_PARENTHESIS;
    case KEYCODE_NINE:
        return Keyboard::KEY_NINE;
    case KEYCODE_EQUAL:
        return Keyboard::KEY_EQUAL;
    case KEYCODE_PLUS:
        return Keyboard::KEY_PLUS;
    case KEYCODE_LESS_THAN:
        return Keyboard::KEY_LESS_THAN;
    case KEYCODE_COMMA:
        return Keyboard::KEY_COMMA;
    case KEYCODE_UNDERSCORE:
        return Keyboard::KEY_UNDERSCORE;
    case KEYCODE_MINUS:
        return Keyboard::KEY_MINUS;
    case KEYCODE_GREATER_THAN:
        return Keyboard::KEY_GREATER_THAN;
    case KEYCODE_PERIOD:
        return Keyboard::KEY_PERIOD;
    case KEYCODE_COLON:
        return Keyboard::KEY_COLON;
    case KEYCODE_SEMICOLON:
        return Keyboard::KEY_SEMICOLON;
    case KEYCODE_QUESTION:
        return Keyboard::KEY_QUESTION;
    case KEYCODE_SLASH:
        return Keyboard::KEY_SLASH;
    case KEYCODE_GRAVE:
        return Keyboard::KEY_GRAVE;
    case KEYCODE_TILDE:
        return Keyboard::KEY_TILDE;
    case KEYCODE_LEFT_BRACE:
        return Keyboard::KEY_LEFT_BRACE;
    case KEYCODE_LEFT_BRACKET:
        return Keyboard::KEY_LEFT_BRACKET;
    case KEYCODE_BAR:
        return Keyboard::KEY_BAR;
    case KEYCODE_BACK_SLASH:
        return Keyboard::KEY_BACK_SLASH;
    case KEYCODE_RIGHT_BRACE:
        return Keyboard::KEY_RIGHT_BRACE;
    case KEYCODE_RIGHT_BRACKET:
        return Keyboard::KEY_RIGHT_BRACKET;
    case KEYCODE_QUOTE:
        return Keyboard::KEY_QUOTE;
    case KEYCODE_APOSTROPHE:
        return Keyboard::KEY_APOSTROPHE;
    case 0x20AC:
        return Keyboard::KEY_EURO;
    case KEYCODE_POUND_SIGN:
        return Keyboard::KEY_POUND;
    case KEYCODE_YEN_SIGN:
        return Keyboard::KEY_YEN;
    case KEYCODE_MIDDLE_DOT:
        return Keyboard::KEY_MIDDLE_DOT;
    case KEYCODE_CAPITAL_A:
        return Keyboard::KEY_CAPITAL_A;
    case KEYCODE_A:
        return Keyboard::KEY_A;
    case KEYCODE_CAPITAL_B:
        return Keyboard::KEY_CAPITAL_B;
    case KEYCODE_B:
        return Keyboard::KEY_B;
    case KEYCODE_CAPITAL_C:
        return Keyboard::KEY_CAPITAL_C;
    case KEYCODE_C:
        return Keyboard::KEY_C;
    case KEYCODE_CAPITAL_D:
        return Keyboard::KEY_CAPITAL_D;
    case KEYCODE_D:
        return Keyboard::KEY_D;
    case KEYCODE_CAPITAL_E:
        return Keyboard::KEY_CAPITAL_E;
    case KEYCODE_E:
        return Keyboard::KEY_E;
    case KEYCODE_CAPITAL_F:
        return Keyboard::KEY_CAPITAL_F;
    case KEYCODE_F:
        return Keyboard::KEY_F;
    case KEYCODE_CAPITAL_G:
        return Keyboard::KEY_CAPITAL_G;
    case KEYCODE_G:
        return Keyboard::KEY_G;
    case KEYCODE_CAPITAL_H:
        return Keyboard::KEY_CAPITAL_H;
    case KEYCODE_H:
        return Keyboard::KEY_H;
    case KEYCODE_CAPITAL_I:
        return Keyboard::KEY_CAPITAL_I;
    case KEYCODE_I:
        return Keyboard::KEY_I;
    case KEYCODE_CAPITAL_J:
        return Keyboard::KEY_CAPITAL_J;
    case KEYCODE_J:
        return Keyboard::KEY_J;
    case KEYCODE_CAPITAL_K:
        return Keyboard::KEY_CAPITAL_K;
    case KEYCODE_K:
        return Keyboard::KEY_K;
    case KEYCODE_CAPITAL_L:
        return Keyboard::KEY_CAPITAL_L;
    case KEYCODE_L:
        return Keyboard::KEY_L;
    case KEYCODE_CAPITAL_M:
        return Keyboard::KEY_CAPITAL_M;
    case KEYCODE_M:
        return Keyboard::KEY_M;
    case KEYCODE_CAPITAL_N:
        return Keyboard::KEY_CAPITAL_N;
    case KEYCODE_N:
        return Keyboard::KEY_N;
    case KEYCODE_CAPITAL_O:
        return Keyboard::KEY_CAPITAL_O;
    case KEYCODE_O:
        return Keyboard::KEY_O;
    case KEYCODE_CAPITAL_P:
        return Keyboard::KEY_CAPITAL_P;
    case KEYCODE_P:
        return Keyboard::KEY_P;
    case KEYCODE_CAPITAL_Q:
        return Keyboard::KEY_CAPITAL_Q;
    case KEYCODE_Q:
        return Keyboard::KEY_Q;
    case KEYCODE_CAPITAL_R:
        return Keyboard::KEY_CAPITAL_R;
    case KEYCODE_R:
        return Keyboard::KEY_R;
    case KEYCODE_CAPITAL_S:
        return Keyboard::KEY_CAPITAL_S;
    case KEYCODE_S:
        return Keyboard::KEY_S;
    case KEYCODE_CAPITAL_T:
        return Keyboard::KEY_CAPITAL_T;
    case KEYCODE_T:
        return Keyboard::KEY_T;
    case KEYCODE_CAPITAL_U:
        return Keyboard::KEY_CAPITAL_U;
    case KEYCODE_U:
        return Keyboard::KEY_U;
    case KEYCODE_CAPITAL_V:
        return Keyboard::KEY_CAPITAL_V;
    case KEYCODE_V:
        return Keyboard::KEY_V;
    case KEYCODE_CAPITAL_W:
        return Keyboard::KEY_CAPITAL_W;
    case KEYCODE_W:
        return Keyboard::KEY_W;
    case KEYCODE_CAPITAL_X:
        return Keyboard::KEY_CAPITAL_X;
    case KEYCODE_X:
        return Keyboard::KEY_X;
    case KEYCODE_CAPITAL_Y:
        return Keyboard::KEY_CAPITAL_Y;
    case KEYCODE_Y:
        return Keyboard::KEY_Y;
    case KEYCODE_CAPITAL_Z:
        return Keyboard::KEY_CAPITAL_Z;
    case KEYCODE_Z:
        return Keyboard::KEY_Z;
    default:
        return Keyboard::KEY_NONE;
    }
}

/**
 * Returns the unicode value from the given QNX key code value.
 * Some non-printable characters also have corresponding unicode values, such as backspace.
 *
 * @param qnxKeyCode The keyboard key code.
 *
 * @return The unicode value or 0 if the keycode did not represent a unicode key.
 */
static int getUnicode(int qnxKeyCode)
{
    if (qnxKeyCode >= KEYCODE_PC_KEYS && qnxKeyCode <= UNICODE_PRIVATE_USE_AREA_LAST)
    {
        switch (qnxKeyCode)
        {
        case KEYCODE_BACKSPACE:
            return 0x0008;
        case KEYCODE_TAB:
            return 0x0009;
        case KEYCODE_KP_ENTER:
        case KEYCODE_RETURN:
            return 0x000A;
        case KEYCODE_ESCAPE:
            return 0x001B;
        // Win32 doesn't consider delete to be a key char.
        default:
            return 0;
        }
    }
    return qnxKeyCode;
}

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

EGLenum checkErrorEGL(const char* msg)
{
    GP_ASSERT(msg);
    static const char* errmsg[] =
    {
        "EGL function failed",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "EGL cannot access a requested resource",
        "EGL failed to allocate resources for the requested operation",
        "EGL fail to access an unrecognized attribute or attribute value was passed in an attribute list",
        "EGLConfig argument does not name a valid EGLConfig",
        "EGLContext argument does not name a valid EGLContext",
        "EGL current surface of the calling thread is no longer valid",
        "EGLDisplay argument does not name a valid EGLDisplay",
        "EGL arguments are inconsistent",
        "EGLNativePixmapType argument does not refer to a valid native pixmap",
        "EGLNativeWindowType argument does not refer to a valid native window",
        "EGL one or more argument values are invalid",
        "EGLSurface argument does not name a valid surface configured for rendering",
        "EGL power management event has occurred",
    };
    EGLenum error = eglGetError();
    fprintf(stderr, "%s: %s\n", msg, errmsg[error - EGL_SUCCESS]);
    return error;
}

void gesture_callback(gesture_base_t* gesture, mtouch_event_t* event, void* param, int async)
{
    switch (gesture->type)
    {
    case GESTURE_SWIPE:
        {
            if ( __gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) )
            {
                gesture_swipe_t* swipe = (gesture_swipe_t*)gesture;
                if (!__gestureSwipeRecognized)
                {
                    Platform::gestureSwipeEventInternal(swipe->coords.x, swipe->coords.y, swipe->direction);
                    __gestureSwipeRecognized = true;
                }

            }
            break;
        }

    case GESTURE_PINCH:
        {
            if ( __gestureEventsProcessed.test(Gesture::GESTURE_PINCH) )
            {
                gesture_pinch_t* pinch = (gesture_pinch_t*)gesture;
                float dist_x = (float)pinch->last_distance.x - (float)pinch->distance.x;
                float dist_y = (float)pinch->last_distance.y - (float)pinch->distance.y;
                float scale = sqrt( (dist_x * dist_x) + (dist_y * dist_y) );
                Game::getInstance()->gesturePinchEvent(pinch->centroid.x, pinch->centroid.y, scale);
            }
            break;
        }

    case GESTURE_TAP:
        {
            if ( __gestureEventsProcessed.test(Gesture::GESTURE_TAP) )
            {
                gesture_tap_t* tap = (gesture_tap_t*)gesture;
                Platform::gestureTapEventInternal(tap->touch_coords.x, tap->touch_coords.y);
            }
            break;
        }

    default:
        break;

    }
}

#ifdef USE_BLACKBERRY_GAMEPAD

static const char* __vendorStrings[] =
{
    "SteelSeries",
    "Nintendo",
};

static const char* __productStrings[] =
{
    "FREE",
    "Wii Remote",
};

static const int __VIDs[] = {
    0x1038,
    0x057e,
};

static const int __PIDs[] = {
    0x1412,
    0x0306,
};

static const unsigned int __knownGamepads = 2;

void queryGamepad(GamepadHandle handle, int* buttonCount, int* joystickCount, int* productId, int* vendorId, char* productString, char* vendorString)
{
    char id[128];
    screen_get_device_property_iv(handle, SCREEN_PROPERTY_BUTTON_COUNT, buttonCount);
    screen_get_device_property_cv(handle, SCREEN_PROPERTY_ID_STRING, 128, id);
    screen_get_device_property_cv(handle, SCREEN_PROPERTY_PRODUCT, 64, productString);
    screen_get_device_property_cv(handle, SCREEN_PROPERTY_VENDOR, 64, vendorString);

    // Check for the existence of analog sticks.
    int analogs[3];
    if (!screen_get_device_property_iv(handle, SCREEN_PROPERTY_ANALOG0, analogs))
    {
    	++(*joystickCount);
    }

    if (!screen_get_device_property_iv(handle, SCREEN_PROPERTY_ANALOG1, analogs))
    {
    	++(*joystickCount);
    }

    // ID string format: A-BBBB-CCCC-D.D
    // A is the device's index
    // BBBB is the device's Vendor ID (in hexadecimal)
    // CCCC is the device's Product ID (also in hexadecimal)
    // D.D is the device's version number
    char* token = strtok(id, "-");
    token = strtok(NULL, "-");
    if (token)
    {
	    *vendorId = strtol(token, NULL, 16);
    }

    token = strtok(NULL, "-");
    if (token)
    {
        *productId = strtol(token, NULL, 16);
    }

    // For gamepads unknown to BB10,
    // check VID and PID against gamepads known to gameplay.
    if (strlen(productString) == 0 || strlen(vendorString) == 0)
    {
        for (unsigned int i = 0; i < __knownGamepads; ++i)
        {
            if (__VIDs[i] == *vendorId && __PIDs[i] == *productId)
            {
            	strcpy(vendorString, __vendorStrings[i]);
                strcpy(productString, __productStrings[i]);
            }
        }
    }
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
	unsigned int buttons;
    screen_get_device_property_iv(gamepad->_handle, SCREEN_PROPERTY_BUTTONS, (int*)&buttons);
	gamepad->setButtons(buttons);

    unsigned int i;
    for (i = 0; i < gamepad->_joystickCount; ++i)
    {
        GP_ASSERT(i < 2);

        int analog[3];
        switch (i)
        {
        case 0:
            screen_get_device_property_iv(gamepad->_handle, SCREEN_PROPERTY_ANALOG0, analog);
            break;
        case 1:
            screen_get_device_property_iv(gamepad->_handle, SCREEN_PROPERTY_ANALOG1, analog);
            break;
        }
        
        // So far we've tested two gamepads with analog sticks on BlackBerry:
        // the SteelSeries FREE, and the iControlPad.
        // Both return values between -128 and +127, with the y axis starting from
        // the top at -128.
        // 1 / 128 == 0.0078125f
        // 1 / 127 == 0.0078740157480315f
        float x = (float)analog[0];
        float y = -(float)analog[1];
        x *= (x < 0) ? 0.0078125f : 0.0078740157480315f;
        y *= (y > 0) ? 0.0078125f : 0.0078740157480315f;

		gamepad->setJoystickValue(i, x, y);
    }

    for (i = 0; i < gamepad->_triggerCount; ++i)
    {
        GP_ASSERT(i < 2);

        int analog[3];
        switch (i)
        {
        case 0:
            screen_get_device_property_iv(gamepad->_handle, SCREEN_PROPERTY_ANALOG0, analog);
            break;
        case 1:
            screen_get_device_property_iv(gamepad->_handle, SCREEN_PROPERTY_ANALOG1, analog);
            break;
        }

        float value = (float)analog[2] * 0.0078125f;
		gamepad->setTriggerValue(i, value);
    }
}
#else
void Platform::pollGamepadState(Gamepad* gamepad)
{
}
#endif

Platform::Platform(Game* game)
    : _game(game)
{
}

Platform::~Platform()
{
    if (__eglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(__eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    if (__eglSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(__eglDisplay, __eglSurface);
        __eglSurface = EGL_NO_SURFACE;
    }

    if (__eglContext != EGL_NO_CONTEXT)
    {
        eglDestroyContext(__eglDisplay, __eglContext);
        __eglContext = EGL_NO_CONTEXT;
    }

    if (__eglDisplay != EGL_NO_DISPLAY)
    {
        eglTerminate(__eglDisplay);
        __eglDisplay = EGL_NO_DISPLAY;
    }

    if (__screenWindow)
    {
        screen_destroy_window(__screenWindow);
        __screenWindow = NULL;
    }

    if (__screenEvent)
    {
        screen_destroy_event(__screenEvent);
        __screenEvent = NULL;
    }

    if (__screenContext)
    {
        screen_destroy_context(__screenContext);
        __screenContext = NULL;
    }
}

Platform* Platform::create(Game* game, void* attachToWindow)
{
    FileSystem::setResourcePath("./app/native/");
    Platform* platform = new Platform(game);

    // Query game config
    int samples = 0;
    Properties* config = Game::getInstance()->getConfig()->getNamespace("window", true);
    if (config)
    {
        samples = std::max(config->getInt("samples"), 0);
    }

    __gestureSet = gestures_set_alloc();
    swipe_gesture_alloc(NULL, gesture_callback, __gestureSet);
    pinch_gesture_alloc(NULL, gesture_callback, __gestureSet);
    tap_gesture_alloc(NULL, gesture_callback, __gestureSet);

    bps_initialize();

    // Initialize navigator and orientation
    static const int SENSOR_RATE = 25000; // (25000 microseconds = 40 Hz)
    sensor_set_rate(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, SENSOR_RATE);
    sensor_set_rate(SENSOR_TYPE_ACCELEROMETER, SENSOR_RATE);
    sensor_set_rate(SENSOR_TYPE_GYROSCOPE, SENSOR_RATE);
    sensor_set_skip_duplicates(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, true);
    sensor_set_skip_duplicates(SENSOR_TYPE_ACCELEROMETER, true);
    sensor_set_skip_duplicates(SENSOR_TYPE_GYROSCOPE, true);
    sensor_request_events(SENSOR_TYPE_AZIMUTH_PITCH_ROLL);
    sensor_request_events(SENSOR_TYPE_ACCELEROMETER);
    sensor_request_events(SENSOR_TYPE_GYROSCOPE);
    navigator_request_events(0);
    navigator_rotation_lock(true);
    __orientationAngle = atoi(getenv("ORIENTATION"));

    int rc = 0;
    int screenFormat = SCREEN_FORMAT_RGBA8888;
#ifdef __X86__
    int screenUsage = SCREEN_USAGE_OPENGL_ES2;
#else
    int screenUsage = SCREEN_USAGE_DISPLAY|SCREEN_USAGE_OPENGL_ES2; // Physical device copy directly into physical display
#endif
    int screenSwapInterval = WINDOW_VSYNC ? 1 : 0;
    int screenTransparency = SCREEN_TRANSPARENCY_NONE;

    char *width_str = getenv("WIDTH");
    char *height_str = getenv("HEIGHT");

    // Hard-coded to (0,0).
    int windowPosition[] =
    {
        0, 0
    };

    EGLint eglConfigCount;

    // Hard-coded to 32-bit/OpenGL ES 2.0.
    // NOTE: EGL_SAMPLE_BUFFERS and EGL_SAMPLES MUST remain at the beginning of the attribute list
    // since they are expected to be at indices 0-3 in config fallback code later.
    EGLint eglConfigAttrs[] =
    {
        EGL_SAMPLE_BUFFERS,     samples > 0 ? 1 : 0,
        EGL_SAMPLES,            samples,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_DEPTH_SIZE,         24,
        EGL_STENCIL_SIZE,       8,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    __multiSampling = samples > 0;

    const EGLint eglContextAttrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION,    2,
        EGL_NONE
    };

    const EGLint eglSurfaceAttrs[] =
    {
        EGL_RENDER_BUFFER,    EGL_BACK_BUFFER,
        EGL_NONE
    };

    // Create the screen context.
    rc = screen_create_context(&__screenContext, 0);
    if (rc)
    {
        perror("screen_create_context");
        goto error;
    }

    // Create the screen window.
    rc = screen_create_window(&__screenWindow, __screenContext);
    if (rc)
    {
        perror("screen_create_window");
        goto error;
    }

    // Set/get any window properties.
    rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_FORMAT, &screenFormat);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
        goto error;
    }

    rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_USAGE, &screenUsage);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)");
        goto error;
    }

    if (width_str && height_str)
    {
        __screenWindowSize[0] = atoi(width_str);
        __screenWindowSize[1] = atoi(height_str);
    }
    else
    {
        screen_display_t screen_display;
        rc = screen_get_window_property_pv(__screenWindow, SCREEN_PROPERTY_DISPLAY, (void **)&screen_display);
        if (rc)
        {
            perror("screen_get_window_property_pv(SCREEN_PROPERTY_DISPLAY)");
            goto error;
        }

        screen_display_mode_t screen_mode;
        rc = screen_get_display_property_pv(screen_display, SCREEN_PROPERTY_MODE, (void**)&screen_mode);
        if (rc)
        {
            perror("screen_get_display_property_pv(SCREEN_PROPERTY_MODE)");
            goto error;
        }

        int size[2];
        rc = screen_get_window_property_iv(__screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, size);
        if (rc)
        {
            perror("screen_get_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)");
            goto error;
        }

        __screenWindowSize[0] = size[0];
        __screenWindowSize[1] = size[1];

        if ((__orientationAngle == 0) || (__orientationAngle == 180))
        {
            if (((screen_mode.width > screen_mode.height) && (size[0] < size[1])) ||
                ((screen_mode.width < screen_mode.height) && (size[0] > size[1])))
            {
                __screenWindowSize[1] = size[0];
                __screenWindowSize[0] = size[1];
            }
        }
        else if ((__orientationAngle == 90) || (__orientationAngle == 270))
        {
            if (((screen_mode.width > screen_mode.height) && (size[0] > size[1])) ||
                ((screen_mode.width < screen_mode.height) && (size[0] < size[1])))
            {
                __screenWindowSize[1] = size[0];
                __screenWindowSize[0] = size[1];
            }
        }
        else
        {
            perror("Navigator returned an unexpected orientation angle.");
            goto error;
        }


        rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_ROTATION, &__orientationAngle);
        if (rc)
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_ROTATION)");
            goto error;
        }
    }

    rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, __screenWindowSize);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)");
        goto error;
    }

    if (windowPosition[0] != 0 || windowPosition[1] != 0)
    {
        rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_POSITION, windowPosition);
        if (rc)
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_POSITION)");
            goto error;
        }
    }

    rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_TRANSPARENCY, &screenTransparency);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_TRANSPARENCY)");
        goto error;
    }

    // Double buffered.
    rc = screen_create_window_buffers(__screenWindow, 2);
    if (rc)
    {
        perror("screen_create_window_buffers");
        goto error;
    }

    // Create screen event object.
    rc = screen_create_event(&__screenEvent);
    if (rc)
    {
        perror("screen_create_event");
        goto error;
    }

    // Request screen events.
    screen_request_events(__screenContext);

    // Get the EGL display and initialize.
    __eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (__eglDisplay == EGL_NO_DISPLAY)
    {
        perror("eglGetDisplay");
        goto error;
    }
    if (eglInitialize(__eglDisplay, NULL, NULL) != EGL_TRUE)
    {
        perror("eglInitialize");
        goto error;
    }

    if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) != EGL_TRUE || eglConfigCount == 0)
    {
        bool success = false;
        while (samples)
        {
            // Try lowering the MSAA sample count until we find a supported config
            GP_WARN("Failed to find a valid EGL configuration with EGL samples=%d. Trying samples=%d instead.", samples, samples/2);
            samples /= 2;
            eglConfigAttrs[1] = samples > 0 ? 1 : 0;
            eglConfigAttrs[3] = samples;
            if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
            {
                success = true;
                break;
            }
        }

        __multiSampling = samples > 0;

        if (!success)
        {
            checkErrorEGL("eglChooseConfig");
            goto error;
        }
    }

    __eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextAttrs);
    if (__eglContext == EGL_NO_CONTEXT)
    {
        checkErrorEGL("eglCreateContext");
        goto error;
    }

    __eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, __screenWindow, eglSurfaceAttrs);
    if (__eglSurface == EGL_NO_SURFACE)
    {
        checkErrorEGL("eglCreateWindowSurface");
        goto error;
    }

    if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
    {
        checkErrorEGL("eglMakeCurrent");
        goto error;
    }

    // Set vsync.
    eglSwapInterval(__eglDisplay, screenSwapInterval);

    // Initialize OpenGL ES extensions.
    __glExtensions = (const char*)glGetString(GL_EXTENSIONS);

    if (strstr(__glExtensions, "GL_OES_vertex_array_object") || strstr(__glExtensions, "GL_ARB_vertex_array_object"))
    {
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
    }

 #ifdef USE_BLACKBERRY_GAMEPAD

    screen_device_t* screenDevs;

    // Discover initial gamepad devices.
    int count;
    screen_get_context_property_iv(__screenContext, SCREEN_PROPERTY_DEVICE_COUNT, &count);
    screenDevs = (screen_device_t*)calloc(count, sizeof(screen_device_t));
    screen_get_context_property_pv(__screenContext, SCREEN_PROPERTY_DEVICES, (void**)screenDevs);

	for (int i = 0; i < count; i++) 
    {
	    int type;
        screen_get_device_property_iv(screenDevs[i], SCREEN_PROPERTY_TYPE, &type);

        if (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)
        {
            int buttonCount = 0;
            int joystickCount = 0;
            int productId;
            int vendorId;
            char productString[64];
            char vendorString[64];
            queryGamepad(screenDevs[i], &buttonCount, &joystickCount, &productId, &vendorId, productString, vendorString);
            Platform::gamepadEventConnectedInternal(screenDevs[i], buttonCount, joystickCount, 0, vendorId, productId, vendorString, productString);
        }
	}
	free(screenDevs);
#endif

    return platform;

error:

    return NULL;
}

/**
 * Convert the timespec into milliseconds.
 */
double timespec2millis(struct timespec *a)
{
    GP_ASSERT(a);
    return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
}

/**
 * Fires a mouse event or a touch event on the game.
 * If the mouse event is not consumed, a touch event is fired instead.
 *
 * @param mouseEvent The mouse event to fire.
 * @param touchEvent The touch event to fire.
 * @param x The x position of the touch in pixels.
 * @param y The y position of the touch in pixels.
 */
void mouseOrTouchEvent(Mouse::MouseEvent mouseEvent, Touch::TouchEvent touchEvent, int x, int y)
{
    if (!gameplay::Platform::mouseEventInternal(mouseEvent, x, y, 0))
    {
        Platform::touchEventInternal(touchEvent, x, y, 0, true);
    }
}

int Platform::enterMessagePump()
{
    GP_ASSERT(_game);

    int rc;
    int eventType;
    int flags;
    int value;
    int position[2];
    int domain;
    mtouch_event_t touchEvent;
    bool suspended = false;

    // Get the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    __timeStart = timespec2millis(&__timespec);
    __timeAbsolute = 0L;

    _game->run();

    // Message loop.
    while (true)
    {
        bps_event_t* event = NULL;
        
        while (true)
        {
            rc = bps_get_event(&event, 1);
            GP_ASSERT(rc == BPS_SUCCESS);

            if (event == NULL)
                break;

            domain = bps_event_get_domain(event);

            if (domain == screen_get_domain())
            {
                __screenEvent = screen_event_get_event(event);
                screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_TYPE, &eventType);
                switch (eventType)
                {
                    case SCREEN_EVENT_MTOUCH_TOUCH:
                    {
                        screen_get_mtouch_event(__screenEvent, &touchEvent, 0);
                        if (__gestureEventsProcessed.any())
                            rc = gestures_set_process_event(__gestureSet, &touchEvent, NULL);

                        if ( !rc && (__multiTouch || touchEvent.contact_id == 0) )
                        {
                            gameplay::Platform::touchEventInternal(Touch::TOUCH_PRESS, touchEvent.x, touchEvent.y, touchEvent.contact_id);
                        }
                        break;
                    }

                    case SCREEN_EVENT_MTOUCH_RELEASE:
                    {
                        screen_get_mtouch_event(__screenEvent, &touchEvent, 0);
                        if (__gestureEventsProcessed.any())
                            rc = gestures_set_process_event(__gestureSet, &touchEvent, NULL);

                        if ( !rc && (__multiTouch || touchEvent.contact_id == 0) )
                        {
                            gameplay::Platform::touchEventInternal(Touch::TOUCH_RELEASE, touchEvent.x, touchEvent.y, touchEvent.contact_id);
                        }
                        if (__gestureSwipeRecognized)
                        {
                            __gestureSwipeRecognized = false;
                        }
                        break;
                    }

                    case SCREEN_EVENT_MTOUCH_MOVE:
                    {
                        screen_get_mtouch_event(__screenEvent, &touchEvent, 0);
                        if (__gestureEventsProcessed.any())
                            rc = gestures_set_process_event(__gestureSet, &touchEvent, NULL);

                        if ( !rc && (__multiTouch || touchEvent.contact_id == 0) )
                        {
                            gameplay::Platform::touchEventInternal(Touch::TOUCH_MOVE, touchEvent.x, touchEvent.y, touchEvent.contact_id);
                        }
                        break;
                    }

                    case SCREEN_EVENT_POINTER:
                    {
                        static int mouse_pressed = 0;
                        int buttons;
                        int wheel;
                        // A move event will be fired unless a button state changed.
                        bool move = true;
                        bool left_move = false;
                        // This is a mouse move event, it is applicable to a device with a usb mouse or simulator.
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_BUTTONS, &buttons);
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_SOURCE_POSITION, position);
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_MOUSE_WHEEL, &wheel);

                        // Handle left mouse. Interpret as touch if the left mouse event is not consumed.
                        if (buttons & SCREEN_LEFT_MOUSE_BUTTON)
                        {
                            if (mouse_pressed & SCREEN_LEFT_MOUSE_BUTTON)
                            {
                                left_move = true;
                            }
                            else
                            {
                                move = false;
                                mouse_pressed |= SCREEN_LEFT_MOUSE_BUTTON;
                                mouseOrTouchEvent(Mouse::MOUSE_PRESS_LEFT_BUTTON, Touch::TOUCH_PRESS, position[0], position[1]);
                            }
                        }
                        else if (mouse_pressed & SCREEN_LEFT_MOUSE_BUTTON)
                        {
                            move = false;
                            mouse_pressed &= ~SCREEN_LEFT_MOUSE_BUTTON;
                            mouseOrTouchEvent(Mouse::MOUSE_RELEASE_LEFT_BUTTON, Touch::TOUCH_RELEASE, position[0], position[1]);
                        }

                        // Handle right mouse.
                        if (buttons & SCREEN_RIGHT_MOUSE_BUTTON)
                        {
                            if ((mouse_pressed & SCREEN_RIGHT_MOUSE_BUTTON) == 0)
                            {
                                move = false;
                                mouse_pressed |= SCREEN_RIGHT_MOUSE_BUTTON;
                                gameplay::Platform::mouseEventInternal(Mouse::MOUSE_PRESS_RIGHT_BUTTON, position[0], position[1], 0);
                            }
                        }
                        else if (mouse_pressed & SCREEN_RIGHT_MOUSE_BUTTON)
                        {
                            move = false;
                            mouse_pressed &= ~SCREEN_RIGHT_MOUSE_BUTTON;
                            gameplay::Platform::mouseEventInternal(Mouse::MOUSE_RELEASE_RIGHT_BUTTON, position[0], position[1], 0);
                        }

                        // Handle middle mouse.
                        if (buttons & SCREEN_MIDDLE_MOUSE_BUTTON)
                        {
                            if ((mouse_pressed & SCREEN_MIDDLE_MOUSE_BUTTON) == 0)
                            {
                                move = false;
                                mouse_pressed |= SCREEN_MIDDLE_MOUSE_BUTTON;
                                gameplay::Platform::mouseEventInternal(Mouse::MOUSE_PRESS_MIDDLE_BUTTON, position[0], position[1], 0);
                            }
                        }
                        else if (mouse_pressed & SCREEN_MIDDLE_MOUSE_BUTTON)
                        {
                            move = false;
                            mouse_pressed &= ~SCREEN_MIDDLE_MOUSE_BUTTON;
                            gameplay::Platform::mouseEventInternal(Mouse::MOUSE_RELEASE_MIDDLE_BUTTON, position[0], position[1], 0);
                        }

                        // Fire a move event if none of the buttons changed.
                        if (left_move)
                        {
                            mouseOrTouchEvent(Mouse::MOUSE_MOVE, Touch::TOUCH_MOVE, position[0], position[1]);
                        }
                        else if (move)
                        {
                            gameplay::Platform::mouseEventInternal(Mouse::MOUSE_MOVE, position[0], position[1], 0);
                        }

                        // Handle mouse wheel events.
                        if (wheel)
                        {
                            gameplay::Platform::mouseEventInternal(Mouse::MOUSE_WHEEL, position[0], position[1], -wheel);
                        }
                        break;
                    }

                    case SCREEN_EVENT_KEYBOARD:
                    {
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_KEY_FLAGS, &flags);
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_KEY_SYM, &value);
                        gameplay::Keyboard::KeyEvent evt = (flags & KEY_DOWN) ? gameplay::Keyboard::KEY_PRESS :  gameplay::Keyboard::KEY_RELEASE;
                        // Suppress key repeats.
                        if ((flags & KEY_REPEAT) == 0)
                        {
                            keyEventInternal(evt, getKey(value));
                            if (evt == gameplay::Keyboard::KEY_PRESS && (flags & KEY_SYM_VALID))
                            {
                                int unicode = getUnicode(value);
                                if (unicode)
                                    keyEventInternal(gameplay::Keyboard::KEY_CHAR, unicode);
                            }
                        }
                        break;
                    }
#ifdef USE_BLACKBERRY_GAMEPAD
                    case SCREEN_EVENT_DEVICE:
                    {
                        // A device was attached or removed.
                        screen_device_t device;
                        int attached;

                        screen_get_event_property_pv(__screenEvent, SCREEN_PROPERTY_DEVICE, (void**)&device);
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_ATTACHED, &attached);

                        if (attached)
                        {
                            int type;
                            screen_get_device_property_iv(device, SCREEN_PROPERTY_TYPE, &type);
                            if (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)
                            {
                                int buttonCount = 0;
                                int joystickCount = 0;
                                int productId;
                                int vendorId;
                                char productString[64];
                                char vendorString[64];
                                queryGamepad(device, &buttonCount, &joystickCount, &productId, &vendorId, productString, vendorString);
                                Platform::gamepadEventConnectedInternal(device, buttonCount, joystickCount, 0, vendorId, productId, vendorString, productString);
                            }
                        }
                        else
                        {
                            Platform::gamepadEventDisconnectedInternal(device);
                        }

                        break;
                    }
#endif
                    default:
                        break;
                }
            }
            else if (domain == navigator_get_domain())
            {
                switch (bps_event_get_code(event))
                {
                case NAVIGATOR_SWIPE_DOWN:
                    _game->menuEvent();
                    break;
                case NAVIGATOR_WINDOW_STATE:
                {
                    navigator_window_state_t state = navigator_event_get_window_state(event);
                    switch (state)
                    {
                    case NAVIGATOR_WINDOW_FULLSCREEN:
                        if (!__screenFullscreen)
                            __screenFullscreen = true;
                        _game->resume();
                        suspended = false;
                        break;
                    case NAVIGATOR_WINDOW_THUMBNAIL:
                    case NAVIGATOR_WINDOW_INVISIBLE:
                        if (__screenFullscreen && !suspended)
                        {
                            _game->pause();
                            suspended = true;
                        }
                        break;
                    }
                    break;
                }
                case NAVIGATOR_EXIT:
                	// Call Game::shutdown directly, instead of Game::exit.
                	// We need to do this since exit() queues a request to shutdown for the
                	// next frame, which will never get executed because we are suspended.
                    _game->shutdown();
                    break;
                }
            }
            else if (domain == sensor_get_domain())
            {
                if (bps_event_get_code(event) == SENSOR_AZIMUTH_PITCH_ROLL_READING)
                {
                    float azimuth;
                    sensor_event_get_apr(event, &azimuth, &__pitch, &__roll);
                }
                else if (bps_event_get_code(event) == SENSOR_ACCELEROMETER_READING)
                {
                    sensor_event_get_xyz(event, &__accelRawX, &__accelRawY, &__accelRawZ);
                }
                else if (bps_event_get_code(event) == SENSOR_GYROSCOPE_READING)
                {
                    sensor_event_get_xyz(event, &__gyroRawX, &__gyroRawY, &__gyroRawZ);
                }
            }
        }

        // If we are done, then exit.
        if (_game->getState() == Game::UNINITIALIZED)
            break;

        if (!suspended)
        {
            _game->frame();

            // Post the new frame to the display.
            // Note that there are a couple cases where eglSwapBuffers could fail
            // with an error code that requires a certain level of re-initialization:
            //
            // 1) EGL_BAD_NATIVE_WINDOW - Called when the surface we're currently using
            //    is invalidated. This would require us to destroy our EGL surface,
            //    close our OpenKODE window, and start again.
            //
            // 2) EGL_CONTEXT_LOST - Power management event that led to our EGL context
            //    being lost. Requires us to re-create and re-initalize our EGL context
            //    and all OpenGL ES state.
            //
            // For now, if we get these, we'll simply exit.
            rc = eglSwapBuffers(__eglDisplay, __eglSurface);
            if (rc != EGL_TRUE)
            {
                _game->shutdown();
                perror("eglSwapBuffers");
                break;
            }
        }
    }

    screen_stop_events(__screenContext);
    bps_shutdown();
    screen_destroy_context(__screenContext);

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
    return __screenWindowSize[0];
}

unsigned int Platform::getDisplayHeight()
{
    return __screenWindowSize[1];
}

double Platform::getAbsoluteTime()
{
    clock_gettime(CLOCK_REALTIME, &__timespec);
    double now = timespec2millis(&__timespec);
    __timeAbsolute = now - __timeStart;

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
    eglSwapInterval(__eglDisplay, enable ? 1 : 0);
    __vsync = enable;
}

void Platform::swapBuffers()
{
    if (__eglDisplay && __eglSurface)
        eglSwapBuffers(__eglDisplay, __eglSurface);
}

void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

void Platform::setMultiSampling(bool enabled)
{
    if (enabled == __multiSampling)
    {
        return;
    }

    //todo

    __multiSampling = enabled;
}

bool Platform::isMultiSampling()
{
    return __multiSampling;
}

void Platform::setMultiTouch(bool enabled)
{
    __multiTouch = enabled;
}

bool Platform::isMultiTouch()
{
    return __multiTouch;
}

bool Platform::hasAccelerometer()
{
    return true;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    switch(__orientationAngle)
    {
    // Landscape based device adjusting for landscape game mode
    case 0:
        if (pitch)
            *pitch = __pitch;
        if (roll)
            *roll = -__roll;
        break;
    case 180:
        if (pitch)
            *pitch = -__pitch;
        if (roll)
            *roll = __roll;
        break;

    // Portrait based device adjusting for landscape game mode
    case 90:
        if (pitch)
            *pitch = -__roll;
        if (roll)
            *roll = -__pitch;
        break;

    case  270:
        if (pitch)
            *pitch = __roll;
        if (roll)
            *roll = __pitch;
        break;

    default:
        break;
    }
}

void Platform::getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
	if (accelX)
	{
		*accelX = __accelRawX;
	}

	if (accelY)
	{
		*accelY = __accelRawY;
	}

	if (accelZ)
	{
		*accelZ = __accelRawZ;
	}

	if (gyroX)
	{
		*gyroX = __gyroRawX;
	}

	if (gyroY)
	{
		*gyroY = __gyroRawY;
	}

	if (gyroZ)
	{
		*gyroZ = __gyroRawZ;
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
    // not supported
    return false;
}

void Platform::setMouseCaptured(bool captured)
{
    // not supported
}

bool Platform::isMouseCaptured()
{
    // not supported
    return false;
}

void Platform::setCursorVisible(bool visible)
{
    // not supported
}

bool Platform::isCursorVisible()
{
    // not supported
    return false;
}

void Platform::displayKeyboard(bool display)
{
    if (display)
        virtualkeyboard_show();
    else
        virtualkeyboard_hide();
}

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    // All are supported no need to test the bitset
    return true;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
    switch(evt)
    {
    case Gesture::GESTURE_ANY_SUPPORTED:
        __gestureEventsProcessed.set();
        break;

    case Gesture::GESTURE_SWIPE:
    case Gesture::GESTURE_PINCH:
    case Gesture::GESTURE_TAP:
        __gestureEventsProcessed.set(evt);
        break;

    default:
        break;
    }
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
    switch(evt)
    {
    case Gesture::GESTURE_ANY_SUPPORTED:
        __gestureEventsProcessed.reset();
        break;

    case Gesture::GESTURE_SWIPE:
    case Gesture::GESTURE_PINCH:
    case Gesture::GESTURE_TAP:
        __gestureEventsProcessed.set(evt, 0);
        break;

    default:
        break;
    }
}
    
bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return __gestureEventsProcessed.test(evt);
}

bool Platform::launchURL(const char* url)
{
    if (url == NULL || *url == '\0')
        return false;

    return navigator_invoke(url, NULL) == BPS_SUCCESS;
}

}

#endif
