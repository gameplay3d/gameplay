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

#ifdef USE_GL_VAOS
    if (strstr(__glExtensions, "GL_OES_vertex_array_object") || strstr(__glExtensions, "GL_ARB_vertex_array_object"))
    {
        // Disable VAO extension for now.
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
    }
#endif

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
                        switch (value)
                        {
                            case KEYCODE_A:
                                Game::getInstance()->keyPress(Input::KEY_A, keyEvent);
                                break;
                            case KEYCODE_B:
                                Game::getInstance()->keyPress(Input::KEY_B, keyEvent);
                                break;
                            case KEYCODE_P:
                                Game::getInstance()->keyPress(Input::KEY_P, keyEvent);
                                break;
                        }
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
                perror("eglSwapBuffers");
                break;
            }
        }
    }

    _game->exit();

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
