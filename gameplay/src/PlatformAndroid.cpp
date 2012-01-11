#ifdef __ANDROID__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include <unistd.h>

#include <android/sensor.h>
#include <android_native_app_glue.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#define TOUCH_COUNT_MAX     4

using namespace std;

struct timespec __timespec;
static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;

struct android_app* __state;
extern std::string __assetsPath;
int __width;
int __height;
static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
static EGLContext __eglContext = EGL_NO_CONTEXT;
static EGLSurface __eglSurface = EGL_NO_SURFACE;
static EGLConfig __eglConfig = 0;
ASensorManager* __sensorManager;
const ASensor* __accelerometerSensor;
ASensorEventQueue* __sensorEventQueue;
ASensorEvent __sensorEvent;
static int __orientationAngle;
static bool __multiTouch = false;

static const char* __glExtensions;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;



//gameplay::Keyboard::Key getKey(int keycode, int metastate);

namespace gameplay
{

// Gets the Keyboard::Key enumeration constant that corresponds to the given QNX key code.
Keyboard::Key getKey(int keycode, int metastate)
{
    
	// TODO.
	
	return Keyboard::KEY_NONE;
}


extern void printError(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    LOGI(format, argptr);
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
    LOGI("%s: %s\n", msg, errmsg[error - EGL_SUCCESS]);
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
}

Platform* Platform::create(Game* game)
{
    FileSystem::setResourcePath(__assetsPath.c_str());
   
    Platform* platform = new Platform(game);

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
    
    EGLint eglConfigCount;
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

    // Get the EGL display and initialize.
    __eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (__eglDisplay == EGL_NO_DISPLAY)
    {
        checkErrorEGL("eglGetDisplay");
        goto error;
    }
    
    if (eglInitialize(__eglDisplay, NULL, NULL) != EGL_TRUE)
    {
        checkErrorEGL("eglInitialize");
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
    
    // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
    // guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
    // As soon as we picked a EGLConfig, we can safely reconfigure the
    // ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
    EGLint format;
    eglGetConfigAttrib(__eglDisplay, __eglConfig, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(__state->window, 0, 0, format);

    __eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, __state->window, eglSurfaceAttrs);
    if (__eglSurface == EGL_NO_SURFACE)
    {
        checkErrorEGL("eglCreateWindowSurface");
        goto error;
    }

    LOGI("Platform::create - 3");
    
    if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
    {
        checkErrorEGL("eglMakeCurrent");
        goto error;
    }

    eglQuerySurface(__eglDisplay, __eglSurface, EGL_WIDTH, &__width);
    eglQuerySurface(__eglDisplay, __eglSurface, EGL_HEIGHT, &__height);
	
    WARN_VARG("Platform::create - WIDTH: %d HEIGHT = %d" , __width, __height);
    
    // Set vsync.
    eglSwapInterval(__eglDisplay, WINDOW_VSYNC ? 1 : 0);

    // Initialize OpenGL ES extensions.
    __glExtensions = (const char*)glGetString(GL_EXTENSIONS);

    if (strstr(__glExtensions, "GL_OES_vertex_array_object") || strstr(__glExtensions, "GL_ARB_vertex_array_object"))
    {
        WARN("Platform::create - VAOs supported");
		
		// Disable VAO extension for now.
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
    }

    
    // TODO: Determine initial orientation angle.
    //orientation_direction_t direction;
    //orientation_get(&direction, &__orientationAngle);

    return platform;

error:

    // TODO: cleanup.

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
    static int state = 0;
	
	switch (state)
	{
		case 0:
			// Get the initial time.
			clock_gettime(CLOCK_REALTIME, &__timespec);
			__timeStart = timespec2millis(&__timespec);
			__timeAbsolute = 0L;
			WARN_VARG("Platform::enterMessagePump() - WIDTH: %d HEIGHT = %d" , __width, __height);
			_game->run(__width, __height);
			
			state = 1;
			break;
		case 1:
			// Idle time (no events left to process) is spent rendering.
			// We skip rendering when the app is paused.
			if (_game->getState() != Game::PAUSED)
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
				int rc = eglSwapBuffers(__eglDisplay, __eglSurface);
				if (rc != EGL_TRUE)
				{
					perror("eglSwapBuffers");
					_game->exit();
					break;
				}
			}
			break;
	}
    
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

void Platform::setMultiTouch(bool enabled)
{
    __multiTouch = enabled;
}

bool Platform::isMultiTouch()
{
    return __multiTouch;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    double tx, ty, tz;
	ASensorEvent event;
	
	// By default, android accelerometer values are oriented to the portrait mode.
	// flipping the x and y to get the desired landscape mode values.
	tx = -__sensorEvent.acceleration.y;
	ty = __sensorEvent.acceleration.x;
	tz = -__sensorEvent.acceleration.z;
	
	if (pitch != NULL)
		*pitch = atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
	if (roll != NULL)
		*roll = atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
}

void Platform::swapBuffers()
{
    if (__eglDisplay && __eglSurface)
        eglSwapBuffers(__eglDisplay, __eglSurface);
}

}

#endif
