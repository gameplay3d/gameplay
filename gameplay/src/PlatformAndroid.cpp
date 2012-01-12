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

int __width;
int __height;
static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
static EGLContext __eglContext = EGL_NO_CONTEXT;
static EGLSurface __eglSurface = EGL_NO_SURFACE;
static EGLConfig __eglConfig = 0;


struct timespec __timespec;
static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;

struct android_app* __state;
extern std::string __assetsPath;

ASensorManager* __sensorManager;
ASensorEventQueue* __sensorEventQueue;
ASensorEvent __sensorEvent;
const ASensor* __accelerometerSensor;

static int __orientationAngle;
static bool __multiTouch = false;
extern bool __displayKeyboard;

static const char* __glExtensions;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

namespace gameplay
{

extern void displayKeyboard(android_app* state, bool pShow)
{ 
    
    // The following functions is supposed to show / hide functins from a native activity.. but currently
    // do not work. 
    // ANativeActivity_showSoftInput(state->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
    // ANativeActivity_hideSoftInput(state->activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
    
    // Show or hide the keyboard by calling the appropriate Java method through JNI instead.
    // Attaches the current thread to the JVM.
    jint lResult;
    jint lFlags = 0;
    JavaVM* lJavaVM = state->activity->vm;
    JNIEnv* lJNIEnv = state->activity->env; 
    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;
    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs); 
    if (lResult == JNI_ERR)
    { 
        return; 
    } 
    // Retrieves NativeActivity. 
    jobject lNativeActivity = state->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = lJNIEnv->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE = lJNIEnv->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE);
    
    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);
    
    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(ClassNativeActivity, "getWindow", "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass("android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow, MethodGetDecorView);
    if (pShow)
    {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput, lDecorView, lFlags); 
    } 
    else 
    { 
        // Runs lWindow.getViewToken() 
        jclass ClassView = lJNIEnv->FindClass("android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView, MethodGetWindowToken); 
        
        // lInputMethodManager.hideSoftInput(...). 
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z"); 
        jboolean lRes = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodHideSoftInput, lBinder, lFlags); 
    }
    
    // Finished with the JVM.
    lJavaVM->DetachCurrentThread(); 
}

// Gets the Keyboard::Key enumeration constant that corresponds to the given Android key code.
extern Keyboard::Key getKey(int keycode, int metastate)
{
    bool shiftOn = (metastate == AMETA_SHIFT_ON);
    
    switch(keycode)
    {
        case AKEYCODE_HOME:
            return Keyboard::KEY_HOME;
        case AKEYCODE_0:
            return Keyboard::KEY_ZERO;
        case AKEYCODE_1:
            return Keyboard::KEY_ONE;
        case AKEYCODE_2:
            return Keyboard::KEY_TWO;
        case AKEYCODE_3:
            return Keyboard::KEY_THREE;
        case AKEYCODE_4:
            return Keyboard::KEY_FOUR;
        case AKEYCODE_5:
            return Keyboard::KEY_FIVE;
        case AKEYCODE_6:
            return Keyboard::KEY_SIX;
        case AKEYCODE_7:
            return Keyboard::KEY_SEVEN;
        case AKEYCODE_8:
            return Keyboard::KEY_EIGHT;
        case AKEYCODE_9:
            return Keyboard::KEY_NINE;
        case AKEYCODE_STAR:
            return Keyboard::KEY_ASTERISK;
        case AKEYCODE_POUND:
            return Keyboard::KEY_NUMBER;
        case AKEYCODE_DPAD_UP:
            return Keyboard::KEY_UP_ARROW;
        case AKEYCODE_DPAD_DOWN:
            return Keyboard::KEY_DOWN_ARROW;
        case AKEYCODE_DPAD_LEFT:
            return Keyboard::KEY_LEFT_ARROW;
        case AKEYCODE_DPAD_RIGHT:
            return Keyboard::KEY_RIGHT_ARROW;
        case AKEYCODE_A:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_A : Keyboard::KEY_A;
        case AKEYCODE_B:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_B : Keyboard::KEY_B;
       case AKEYCODE_C:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_C : Keyboard::KEY_C;
        case AKEYCODE_D:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_D : Keyboard::KEY_D;
        case AKEYCODE_E:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_E : Keyboard::KEY_E;
        case AKEYCODE_F:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_F : Keyboard::KEY_F;
        case AKEYCODE_G:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_G : Keyboard::KEY_G;
        case AKEYCODE_H:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_H : Keyboard::KEY_H;
        case AKEYCODE_I:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_I : Keyboard::KEY_I;
        case AKEYCODE_J:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_J : Keyboard::KEY_J;
        case AKEYCODE_K:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_K : Keyboard::KEY_K;
        case AKEYCODE_L:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_L : Keyboard::KEY_L;
        case AKEYCODE_M:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_M : Keyboard::KEY_M;
        case AKEYCODE_N:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_N : Keyboard::KEY_N;
        case AKEYCODE_O:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_O : Keyboard::KEY_O;
        case AKEYCODE_P:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_P : Keyboard::KEY_P;
        case AKEYCODE_Q:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_Q : Keyboard::KEY_Q;
        case AKEYCODE_R:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_R : Keyboard::KEY_R;
        case AKEYCODE_S:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_S : Keyboard::KEY_S;
        case AKEYCODE_T:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_T : Keyboard::KEY_T;
        case AKEYCODE_U:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_U : Keyboard::KEY_U;
        case AKEYCODE_V:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_V : Keyboard::KEY_V;
        case AKEYCODE_W:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_W : Keyboard::KEY_W;
        case AKEYCODE_X:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_X : Keyboard::KEY_X;
        case AKEYCODE_Y:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_Y : Keyboard::KEY_Y;
        case AKEYCODE_Z:
            return (shiftOn) ? Keyboard::KEY_CAPITAL_Y : Keyboard::KEY_Y;
        case AKEYCODE_COMMA:
            return Keyboard::KEY_COMMA;
        case AKEYCODE_PERIOD:
            return Keyboard::KEY_PERIOD;
        case AKEYCODE_ALT_LEFT:
            return Keyboard::KEY_LEFT_ALT;
        case AKEYCODE_ALT_RIGHT:
            return Keyboard::KEY_RIGHT_ALT;
        case AKEYCODE_SHIFT_LEFT:
            return Keyboard::KEY_LEFT_SHIFT;
        case AKEYCODE_SHIFT_RIGHT:
            return Keyboard::KEY_RIGHT_SHIFT;
        case AKEYCODE_TAB:
            return Keyboard::KEY_TAB;
        case AKEYCODE_SPACE:
            return Keyboard::KEY_SPACE;
        case AKEYCODE_ENTER:
            return Keyboard::KEY_RETURN;
        case AKEYCODE_DEL:
            return Keyboard::KEY_DELETE;
        case AKEYCODE_GRAVE:
            return Keyboard::KEY_GRAVE;
        case AKEYCODE_MINUS:
            return Keyboard::KEY_MINUS;
        case AKEYCODE_EQUALS:
            return Keyboard::KEY_EQUAL;
        case AKEYCODE_LEFT_BRACKET:
            return Keyboard::KEY_LEFT_BRACKET;
        case AKEYCODE_RIGHT_BRACKET:
            return Keyboard::KEY_RIGHT_BRACKET;
        case AKEYCODE_BACKSLASH:
            return Keyboard::KEY_BACK_SLASH;
        case AKEYCODE_SEMICOLON:
            return Keyboard::KEY_SEMICOLON;
        case AKEYCODE_APOSTROPHE:
            return Keyboard::KEY_APOSTROPHE;
        case AKEYCODE_SLASH:
            return Keyboard::KEY_SLASH;
        case AKEYCODE_AT:
            return Keyboard::KEY_AT;
        case AKEYCODE_PLUS:
            return Keyboard::KEY_PLUS;
        case AKEYCODE_PAGE_UP:
            return Keyboard::KEY_PG_UP;
        case AKEYCODE_PAGE_DOWN:
            return Keyboard::KEY_PG_DOWN;
        default:
            return Keyboard::KEY_NONE;
    }
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

void Platform::displayKeyboard(bool display)
{
    if (display)
        __displayKeyboard = true;
    else
        __displayKeyboard = false;
}

}

#endif
