/*
 * PlatformQNX.cpp
 */
#ifdef __QNX__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Input.h"
#include <sys/keycodes.h>
#include <screen/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>
#include <bps/navigator.h>
#include <bps/accelerometer.h>
#include <bps/orientation.h>

using namespace std;

struct timespec __timespec;
static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static screen_context_t __screenContext;
static screen_window_t __screenWindow;
static screen_event_t __screenEvent;
static int __screenWindowSize[2];
static EGLDisplay __eglDisplay;
static EGLContext __eglContext;
static EGLSurface __eglSurface;
static EGLConfig __eglConfig = 0;
static int __orientationAngle;

static const char* __glExtensions;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

namespace gameplay
{

// Gets the Input::Key enumeration constant that corresponds to the given QNX key code.
static Input::Key getGameplayInputKey(int qnxKeycode)
{
    switch (qnxKeycode)
    {
    case KEYCODE_SYSREQ:
        return Input::KEY_SYSREQ;
    case KEYCODE_BREAK:
        return Input::KEY_BREAK;
    case KEYCODE_MENU:
        return Input::KEY_MENU;
    case KEYCODE_KP_ENTER:
        return Input::KEY_KP_ENTER;
    case KEYCODE_PAUSE:
        return Input::KEY_PAUSE;
    case KEYCODE_SCROLL_LOCK:
        return Input::KEY_SCROLL_LOCK;
    case KEYCODE_PRINT:
        return Input::KEY_PRINT;
    case KEYCODE_ESCAPE:
        return Input::KEY_ESCAPE;
    case KEYCODE_BACKSPACE:
        return Input::KEY_BACKSPACE;
    case KEYCODE_BACK_TAB:
        return Input::KEY_BACK_TAB;
    case KEYCODE_TAB:
        return Input::KEY_TAB;
    case KEYCODE_RETURN:
        return Input::KEY_RETURN;
    case KEYCODE_CAPS_LOCK:
        return Input::KEY_CAPS_LOCK;
    case KEYCODE_LEFT_SHIFT:
        return Input::KEY_LEFT_SHIFT;
    case KEYCODE_RIGHT_SHIFT:
        return Input::KEY_RIGHT_SHIFT;
    case KEYCODE_LEFT_CTRL:
        return Input::KEY_LEFT_CTRL;
    case KEYCODE_RIGHT_CTRL:
        return Input::KEY_RIGHT_CTRL;
    case KEYCODE_LEFT_ALT:
        return Input::KEY_LEFT_ALT;
    case KEYCODE_RIGHT_ALT:
        return Input::KEY_RIGHT_ALT;
    case KEYCODE_LEFT_HYPER:
        return Input::KEY_LEFT_HYPER;
    case KEYCODE_RIGHT_HYPER:
        return Input::KEY_RIGHT_HYPER;
    case KEYCODE_INSERT:
        return Input::KEY_INSERT;
    case KEYCODE_HOME:
        return Input::KEY_HOME;
    case KEYCODE_PG_UP:
        return Input::KEY_PG_UP;
    case KEYCODE_DELETE:
        return Input::KEY_DELETE;
    case KEYCODE_END:
        return Input::KEY_END;
    case KEYCODE_PG_DOWN:
        return Input::KEY_PG_DOWN;
    case KEYCODE_LEFT:
        return Input::KEY_LEFT_ARROW;
    case KEYCODE_RIGHT:
        return Input::KEY_RIGHT_ARROW;
    case KEYCODE_UP:
        return Input::KEY_UP_ARROW;
    case KEYCODE_DOWN:
        return Input::KEY_DOWN_ARROW;
    case KEYCODE_NUM_LOCK:
        return Input::KEY_NUM_LOCK;
    case KEYCODE_KP_PLUS:
        return Input::KEY_KP_PLUS;
    case KEYCODE_KP_MINUS:
        return Input::KEY_KP_MINUS;
    case KEYCODE_KP_MULTIPLY:
        return Input::KEY_KP_MULTIPLY;
    case KEYCODE_KP_DIVIDE:
        return Input::KEY_KP_DIVIDE;
    case KEYCODE_KP_HOME:
        return Input::KEY_KP_HOME;
    case KEYCODE_KP_UP:
        return Input::KEY_KP_UP;
    case KEYCODE_KP_PG_UP:
        return Input::KEY_KP_PG_UP;
    case KEYCODE_KP_LEFT:
        return Input::KEY_KP_LEFT;
    case KEYCODE_KP_FIVE:
        return Input::KEY_KP_FIVE;
    case KEYCODE_KP_RIGHT:
        return Input::KEY_KP_RIGHT;
    case KEYCODE_KP_END:
        return Input::KEY_KP_END;
    case KEYCODE_KP_DOWN:
        return Input::KEY_KP_DOWN;
    case KEYCODE_KP_PG_DOWN:
        return Input::KEY_KP_PG_DOWN;
    case KEYCODE_KP_INSERT:
        return Input::KEY_KP_INSERT;
    case KEYCODE_KP_DELETE:
        return Input::KEY_KP_DELETE;
    case KEYCODE_F1:
        return Input::KEY_F1;
    case KEYCODE_F2:
        return Input::KEY_F2;
    case KEYCODE_F3:
        return Input::KEY_F3;
    case KEYCODE_F4:
        return Input::KEY_F4;
    case KEYCODE_F5:
        return Input::KEY_F5;
    case KEYCODE_F6:
        return Input::KEY_F6;
    case KEYCODE_F7:
        return Input::KEY_F7;
    case KEYCODE_F8:
        return Input::KEY_F8;
    case KEYCODE_F9:
        return Input::KEY_F9;
    case KEYCODE_F10:
        return Input::KEY_F10;
    case KEYCODE_F11:
        return Input::KEY_F11;
    case KEYCODE_F12:
        return Input::KEY_F12;
    case KEYCODE_SPACE:
        return Input::KEY_SPACE;
    case KEYCODE_RIGHT_PAREN:
        return Input::KEY_RIGHT_PARENTHESIS;
    case KEYCODE_ZERO:
        return Input::KEY_ZERO;
    case KEYCODE_EXCLAM:
        return Input::KEY_EXCLAM;
    case KEYCODE_ONE:
        return Input::KEY_ONE;
    case KEYCODE_AT:
        return Input::KEY_AT;
    case KEYCODE_TWO:
        return Input::KEY_TWO;
    case KEYCODE_NUMBER:
        return Input::KEY_NUMBER;
    case KEYCODE_THREE:
        return Input::KEY_THREE;
    case KEYCODE_DOLLAR:
        return Input::KEY_DOLLAR;
    case KEYCODE_FOUR:
        return Input::KEY_FOUR;
    case KEYCODE_PERCENT:
        return Input::KEY_PERCENT;
    case KEYCODE_FIVE:
        return Input::KEY_FIVE;
    case KEYCODE_CIRCUMFLEX:
        return Input::KEY_CIRCUMFLEX;
    case KEYCODE_SIX:
        return Input::KEY_SIX;
    case KEYCODE_AMPERSAND:
        return Input::KEY_AMPERSAND;
    case KEYCODE_SEVEN:
        return Input::KEY_SEVEN;
    case KEYCODE_ASTERISK:
        return Input::KEY_ASTERISK;
    case KEYCODE_EIGHT:
        return Input::KEY_EIGHT;
    case KEYCODE_LEFT_PAREN:
        return Input::KEY_LEFT_PARENTHESIS;
    case KEYCODE_NINE:
        return Input::KEY_NINE;
    case KEYCODE_EQUAL:
        return Input::KEY_EQUAL;
    case KEYCODE_PLUS:
        return Input::KEY_PLUS;
    case KEYCODE_LESS_THAN:
        return Input::KEY_LESS_THAN;
    case KEYCODE_COMMA:
        return Input::KEY_COMMA;
    case KEYCODE_UNDERSCORE:
        return Input::KEY_UNDERSCORE;
    case KEYCODE_MINUS:
        return Input::KEY_MINUS;
    case KEYCODE_GREATER_THAN:
        return Input::KEY_GREATER_THAN;
    case KEYCODE_PERIOD:
        return Input::KEY_PERIOD;
    case KEYCODE_COLON:
        return Input::KEY_COLON;
    case KEYCODE_SEMICOLON:
        return Input::KEY_SEMICOLON;
    case KEYCODE_QUESTION:
        return Input::KEY_QUESTION;
    case KEYCODE_SLASH:
        return Input::KEY_SLASH;
    case KEYCODE_GRAVE:
        return Input::KEY_GRAVE;
    case KEYCODE_TILDE:
        return Input::KEY_TILDE;
    case KEYCODE_LEFT_BRACE:
        return Input::KEY_LEFT_BRACE;
    case KEYCODE_LEFT_BRACKET:
        return Input::KEY_LEFT_BRACKET;
    case KEYCODE_BAR:
        return Input::KEY_BAR;
    case KEYCODE_BACK_SLASH:
        return Input::KEY_BACK_SLASH;
    case KEYCODE_RIGHT_BRACE:
        return Input::KEY_RIGHT_BRACE;
    case KEYCODE_RIGHT_BRACKET:
        return Input::KEY_RIGHT_BRACKET;
    case KEYCODE_QUOTE:
        return Input::KEY_QUOTE;
    case KEYCODE_APOSTROPHE:
        return Input::KEY_APOSTROPHE;
    case KEYCODE_CAPITAL_A:
        return Input::KEY_CAPITAL_A;
    case KEYCODE_A:
        return Input::KEY_A;
    case KEYCODE_CAPITAL_B:
        return Input::KEY_CAPITAL_B;
    case KEYCODE_B:
        return Input::KEY_B;
    case KEYCODE_CAPITAL_C:
        return Input::KEY_CAPITAL_C;
    case KEYCODE_C:
        return Input::KEY_C;
    case KEYCODE_CAPITAL_D:
        return Input::KEY_CAPITAL_D;
    case KEYCODE_D:
        return Input::KEY_D;
    case KEYCODE_CAPITAL_E:
        return Input::KEY_CAPITAL_E;
    case KEYCODE_E:
        return Input::KEY_E;
    case KEYCODE_CAPITAL_F:
        return Input::KEY_CAPITAL_F;
    case KEYCODE_F:
        return Input::KEY_F;
    case KEYCODE_CAPITAL_G:
        return Input::KEY_CAPITAL_G;
    case KEYCODE_G:
        return Input::KEY_G;
    case KEYCODE_CAPITAL_H:
        return Input::KEY_CAPITAL_H;
    case KEYCODE_H:
        return Input::KEY_H;
    case KEYCODE_CAPITAL_I:
        return Input::KEY_CAPITAL_I;
    case KEYCODE_I:
        return Input::KEY_I;
    case KEYCODE_CAPITAL_J:
        return Input::KEY_CAPITAL_J;
    case KEYCODE_J:
        return Input::KEY_J;
    case KEYCODE_CAPITAL_K:
        return Input::KEY_CAPITAL_K;
    case KEYCODE_K:
        return Input::KEY_K;
    case KEYCODE_CAPITAL_L:
        return Input::KEY_CAPITAL_L;
    case KEYCODE_L:
        return Input::KEY_L;
    case KEYCODE_CAPITAL_M:
        return Input::KEY_CAPITAL_M;
    case KEYCODE_M:
        return Input::KEY_M;
    case KEYCODE_CAPITAL_N:
        return Input::KEY_CAPITAL_N;
    case KEYCODE_N:
        return Input::KEY_N;
    case KEYCODE_CAPITAL_O:
        return Input::KEY_CAPITAL_O;
    case KEYCODE_O:
        return Input::KEY_O;
    case KEYCODE_CAPITAL_P:
        return Input::KEY_CAPITAL_P;
    case KEYCODE_P:
        return Input::KEY_P;
    case KEYCODE_CAPITAL_Q:
        return Input::KEY_CAPITAL_Q;
    case KEYCODE_Q:
        return Input::KEY_Q;
    case KEYCODE_CAPITAL_R:
        return Input::KEY_CAPITAL_R;
    case KEYCODE_R:
        return Input::KEY_R;
    case KEYCODE_CAPITAL_S:
        return Input::KEY_CAPITAL_S;
    case KEYCODE_S:
        return Input::KEY_S;
    case KEYCODE_CAPITAL_T:
        return Input::KEY_CAPITAL_T;
    case KEYCODE_T:
        return Input::KEY_T;
    case KEYCODE_CAPITAL_U:
        return Input::KEY_CAPITAL_U;
    case KEYCODE_U:
        return Input::KEY_U;
    case KEYCODE_CAPITAL_V:
        return Input::KEY_CAPITAL_V;
    case KEYCODE_V:
        return Input::KEY_V;
    case KEYCODE_CAPITAL_W:
        return Input::KEY_CAPITAL_W;
    case KEYCODE_W:
        return Input::KEY_W;
    case KEYCODE_CAPITAL_X:
        return Input::KEY_CAPITAL_X;
    case KEYCODE_X:
        return Input::KEY_X;
    case KEYCODE_CAPITAL_Y:
        return Input::KEY_CAPITAL_Y;
    case KEYCODE_Y:
        return Input::KEY_Y;
    case KEYCODE_CAPITAL_Z:
        return Input::KEY_CAPITAL_Z;
    case KEYCODE_Z:
        return Input::KEY_Z;
    default:
        return Input::KEY_NONE;
    }
}

extern void printError(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);
}

EGLenum checkErrorEGL(const char* msg)
{
    static const char* errmsg[] =
    {
        "EGL function succeeded",
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

Platform::Platform(Game* game)
    : _game(game)
{
}

Platform::Platform(const Platform& copy)
{
    // hidden
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

Platform* Platform::create(Game* game)
{
    FileSystem::setResourcePath("./app/native/");

    Platform* platform = new Platform(game);

    bps_initialize();
    accelerometer_set_update_frequency(FREQ_40_HZ);
    navigator_request_events(0);
    navigator_rotation_lock(true);

    // Determine initial orientation angle.
    orientation_direction_t direction;
    orientation_get(&direction, &__orientationAngle);

    int rc = 0;
    int screenFormat = SCREEN_FORMAT_RGBA8888;
    int screenUsage = SCREEN_USAGE_DISPLAY|SCREEN_USAGE_OPENGL_ES2;
    int screenSwapInterval = WINDOW_VSYNC ? 1 : 0;
    int screenTransparency = SCREEN_TRANSPARENCY_NONE;

    // Hard-coded to fullscreen.
    __screenWindowSize[0] = -1;
    __screenWindowSize[1] = -1;

    // Hard-coded to (0,0).
    int windowPosition[] =
    {
        0, 0
    };

    EGLint eglConfigCount;

    // Hard-coded to 32-bit/OpenGL ES 2.0.
    const EGLint eglConfigAttrs[] =
    {
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

    // Set/get any window prooperties.
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

    if (__screenWindowSize[0] > 0 && __screenWindowSize[1] > 0)
    {
        rc = screen_set_window_property_iv(__screenWindow, SCREEN_PROPERTY_SIZE, __screenWindowSize);
        if (rc)
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_SIZE)");
            goto error;
        }
    }
    else
    {
        rc = screen_get_window_property_iv(__screenWindow, SCREEN_PROPERTY_SIZE, __screenWindowSize);
        if (rc)
        {
            perror("screen_get_window_property_iv(SCREEN_PROPERTY_SIZE)");
            goto error;
        }
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
        checkErrorEGL("eglChooseConfig");
        goto error;
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
        // Disable VAO extension for now.
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
    }

    return platform;

error:

    // TODO: cleanup
    //

    return NULL;
}

/**
 * Convert the timespec into milliseconds.
 */
long timespec2millis(struct timespec *a)
{
    return a->tv_sec*1000 + a->tv_nsec/1000000;
}

int Platform::enterMessagePump()
{
    int rc;
    int eventType;
    int flags;
    int value;
    int visible = 1;
    int position[2];
    int domain;

    // Get the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    __timeStart = timespec2millis(&__timespec);
    __timeAbsolute = 0L;

    _game->run(__screenWindowSize[0], __screenWindowSize[1]);

    // Message loop.
    while (true)
    {
        bps_event_t* event = NULL;
        
        while (true)
        {
            rc = bps_get_event(&event, 1);  // 1 = Hack for now until bps is fixed. Soon.
            assert(rc == BPS_SUCCESS);

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
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_POSITION, position);
                        Game::getInstance()->touch(position[0], position[1], Input::TOUCHEVENT_PRESS);
                        break;
                    case SCREEN_EVENT_MTOUCH_MOVE:
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_POSITION, position);
                        Game::getInstance()->touch(position[0], position[1], Input::TOUCHEVENT_MOVE);
                        break;
                    case SCREEN_EVENT_MTOUCH_RELEASE:
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_POSITION, position);
                        Game::getInstance()->touch(position[0], position[1], Input::TOUCHEVENT_RELEASE);
                        break;

                    case SCREEN_EVENT_KEYBOARD:
                    {
                        // Keyboard event.
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_KEY_FLAGS, &flags);
                        screen_get_event_property_iv(__screenEvent, SCREEN_PROPERTY_KEY_SYM, &value);
                        int keyEvent = flags & KEY_DOWN ? Input::KEYEVENT_DOWN : Input::KEYEVENT_UP;
                        Game::getInstance()->keyPress(getGameplayInputKey(value), keyEvent);
                        break;
                    }
                }
            }
            else if (domain == navigator_get_domain())
            {
                switch (bps_event_get_code(event))
                {
                case NAVIGATOR_SWIPE_DOWN:
                    _game->menu();
                    break;
                case NAVIGATOR_EXIT:
                    _game->exit();
                    break;
                }
            }
        }

        // If we are done, then exit.
        if (_game->getState() == Game::UNINITIALIZED)
            break;

        // Idle time (no events left to process) is spent rendering.
        // We skip rendering when the window is not visible or the app is paused.
        if (visible && _game->getState() != Game::PAUSED)
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
                _game->exit();
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

long Platform::getAbsoluteTime()
{
    clock_gettime(CLOCK_REALTIME, &__timespec);
    long now = timespec2millis(&__timespec);
    __timeAbsolute = now - __timeStart;

    return __timeAbsolute;
}

void Platform::setAbsoluteTime(long time)
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

int Platform::getOrientationAngle()
{
    return __orientationAngle;
}

bool Platform::isAccelerometerSupported()
{
    return accelerometer_is_supported();
}

void Platform::getAccelerometerPitchAndRoll(float* pitch, float* roll)
{
    double tx, ty, tz;
    accelerometer_read_forces(&tx, &ty, &tz);

    // Hack landscape adjustment only.
    if (__orientationAngle == 0)
    {
        tx = -tx;
        ty = -ty;
        tz = -tz;
    }

    if (pitch != NULL)
        *pitch = atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
    if (roll != NULL)
        *roll = atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
}

}

#endif
