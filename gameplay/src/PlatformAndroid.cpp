#ifdef __ANDROID__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"
#include <unistd.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <android/log.h>

// Externally referenced global variables.
struct android_app* __state;
AAssetManager* __assetManager;

static bool __initialized;
static bool __suspended;
static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
static EGLContext __eglContext = EGL_NO_CONTEXT;
static EGLSurface __eglSurface = EGL_NO_SURFACE;
static EGLConfig __eglConfig = 0;
static int __width;
static int __height;
static struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static ASensorManager* __sensorManager;
static ASensorEventQueue* __sensorEventQueue;
static ASensorEvent __sensorEvent;
static const ASensor* __accelerometerSensor;
static const ASensor* __gyroscopeSensor;
static float __accelRawX;
static float __accelRawY;
static float __accelRawZ;
static float __gyroRawX;
static float __gyroRawY;
static float __gyroRawZ;
static int __orientationAngle = 90;
static bool __multiSampling = false;
static bool __multiTouch = false;
static int __primaryTouchId = -1;
static bool __displayKeyboard = false;

// OpenGL VAO functions.
static const char* __glExtensions;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

#define GESTURE_TAP_DURATION_MAX    200
#define GESTURE_SWIPE_DURATION_MAX  400
#define GESTURE_SWIPE_DISTANCE_MIN  50

static std::bitset<3> __gestureEventsProcessed;

struct TouchPointerData
{
    size_t pointerId;
    bool pressed;
    double time;
    int x;
    int y;
};

TouchPointerData __pointer0;
TouchPointerData __pointer1;

namespace gameplay
{

static double timespec2millis(struct timespec *a)
{
    GP_ASSERT(a);
    return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
}

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    __android_log_vprint(ANDROID_LOG_INFO, "gameplay-native-activity", format, argptr);
    va_end(argptr);
}

static EGLenum checkErrorEGL(const char* msg)
{
    GP_ASSERT(msg);
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
    print("%s: %s.", msg, errmsg[error - EGL_SUCCESS]);
    return error;
}

static int getRotation()
{
    jint rotation;

    // Get the android application's activity.
    ANativeActivity* activity = __state->activity;
    JavaVM* jvm = __state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR)
    {
        GP_ERROR("Failed to retrieve JVM environment when entering message pump.");
        return -1; 
    }
    GP_ASSERT(env);

    jclass clsContext = env->FindClass("android/content/Context");
    GP_ASSERT(clsContext != NULL);
    jmethodID getSystemService = env->GetMethodID(clsContext, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    GP_ASSERT(getSystemService != NULL);
    jfieldID WINDOW_SERVICE_ID = env->GetStaticFieldID(clsContext, "WINDOW_SERVICE", "Ljava/lang/String;");
    GP_ASSERT(WINDOW_SERVICE_ID != NULL);
    jstring WINDOW_SERVICE = (jstring) env->GetStaticObjectField(clsContext, WINDOW_SERVICE_ID);
    GP_ASSERT(WINDOW_SERVICE != NULL);
    jobject windowManager = env->CallObjectMethod(activity->clazz, getSystemService, WINDOW_SERVICE);
    GP_ASSERT(windowManager != NULL);
    jclass clsWindowManager = env->FindClass("android/view/WindowManager");
    GP_ASSERT(clsWindowManager != NULL);
    jmethodID getDefaultDisplay = env->GetMethodID(clsWindowManager, "getDefaultDisplay", "()Landroid/view/Display;");
    GP_ASSERT(getDefaultDisplay != NULL);
    jobject defaultDisplay = env->CallObjectMethod(windowManager, getDefaultDisplay);
    GP_ASSERT(defaultDisplay != NULL);
    jclass clsDisplay = env->FindClass("android/view/Display");
    GP_ASSERT(clsDisplay != NULL);
    jmethodID getRotation = env->GetMethodID(clsDisplay, "getRotation", "()I");
    GP_ASSERT(getRotation != NULL)
    rotation =  env->CallIntMethod(defaultDisplay, getRotation);

    return rotation;
}


// Initialized EGL resources.
static bool initEGL()
{
    int samples = 0;
    Properties* config = Game::getInstance()->getConfig()->getNamespace("window", true);
    if (config)
    {
        samples = std::max(config->getInt("samples"), 0);
    }

    // Hard-coded to 32-bit/OpenGL ES 2.0.
    // NOTE: EGL_SAMPLE_BUFFERS, EGL_SAMPLES and EGL_DEPTH_SIZE MUST remain at the beginning of the attribute list
    // since they are expected to be at indices 0-5 in config fallback code later.
    // EGL_DEPTH_SIZE is also expected to
    EGLint eglConfigAttrs[] =
    {
        EGL_SAMPLE_BUFFERS,     samples > 0 ? 1 : 0,
        EGL_SAMPLES,            samples,
        EGL_DEPTH_SIZE,         24,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_STENCIL_SIZE,       8,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    __multiSampling = samples > 0;
    
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

    if (__eglDisplay == EGL_NO_DISPLAY && __eglContext == EGL_NO_CONTEXT)
    {
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

        // Try both 24 and 16-bit depth sizes since some hardware (i.e. Tegra) does not support 24-bit depth
        bool validConfig = false;
        EGLint depthSizes[] = { 24, 16 };
        for (unsigned int i = 0; i < 2; ++i)
        {
            eglConfigAttrs[1] = samples > 0 ? 1 : 0;
            eglConfigAttrs[3] = samples;
            eglConfigAttrs[5] = depthSizes[i];

            if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
            {
                validConfig = true;
                break;
            }

            if (samples)
            {
                // Try lowering the MSAA sample size until we find a supported config
                int sampleCount = samples;
                while (sampleCount)
                {
                    GP_WARN("No EGL config found for depth_size=%d and samples=%d. Trying samples=%d instead.", depthSizes[i], sampleCount, sampleCount / 2);
                    sampleCount /= 2;
                    eglConfigAttrs[1] = sampleCount > 0 ? 1 : 0;
                    eglConfigAttrs[3] = sampleCount;
                    if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
                    {
                        validConfig = true;
                        break;
                    }
                }

                __multiSampling = sampleCount > 0;

                if (validConfig)
                    break;
            }
            else
            {
                GP_WARN("No EGL config found for depth_size=%d.", depthSizes[i]);
            }
        }

        if (!validConfig)
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
    
    if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
    {
        checkErrorEGL("eglMakeCurrent");
        goto error;
    }
    
    eglQuerySurface(__eglDisplay, __eglSurface, EGL_WIDTH, &__width);
    eglQuerySurface(__eglDisplay, __eglSurface, EGL_HEIGHT, &__height);

    __orientationAngle = getRotation() * 90;
    
    // Set vsync.
    eglSwapInterval(__eglDisplay, WINDOW_VSYNC ? 1 : 0);
    
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
    
    return true;
    
error:
    return false;
}

static void destroyEGLSurface()
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
}

static void destroyEGLMain()
{
    destroyEGLSurface();

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

// Display the android virtual keyboard.
static void displayKeyboard(android_app* state, bool show)
{ 
    // The following functions is supposed to show / hide functins from a native activity.. but currently do not work. 
    // ANativeActivity_showSoftInput(state->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
    // ANativeActivity_hideSoftInput(state->activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
    
    GP_ASSERT(state && state->activity && state->activity->vm);

    // Show or hide the keyboard by calling the appropriate Java method through JNI instead.
    jint flags = 0;
    JavaVM* jvm = state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint result = jvm->AttachCurrentThread(&env, NULL);
    if (result == JNI_ERR)
    {
        GP_ERROR("Failed to retrieve JVM environment to display keyboard.");
        return; 
    }
    GP_ASSERT(env);

    // Retrieves NativeActivity. 
    jobject lNativeActivity = state->activity->clazz;
    jclass ClassNativeActivity = env->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = env->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE);
    
    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = env->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = env->GetMethodID(ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = env->CallObjectMethod(lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);
    
    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = env->GetMethodID(ClassNativeActivity, "getWindow", "()Landroid/view/Window;");
    jobject lWindow = env->CallObjectMethod(lNativeActivity, MethodGetWindow);
    jclass ClassWindow = env->FindClass("android/view/Window");
    jmethodID MethodGetDecorView = env->GetMethodID(ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = env->CallObjectMethod(lWindow, MethodGetDecorView);
    if (show)
    {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = env->GetMethodID( ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
        jboolean result = env->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput, lDecorView, flags); 
    } 
    else 
    { 
        // Runs lWindow.getViewToken() 
        jclass ClassView = env->FindClass("android/view/View");
        jmethodID MethodGetWindowToken = env->GetMethodID(ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = env->CallObjectMethod(lDecorView, MethodGetWindowToken); 
        
        // lInputMethodManager.hideSoftInput(...). 
        jmethodID MethodHideSoftInput = env->GetMethodID(ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z"); 
        jboolean lRes = env->CallBooleanMethod( lInputMethodManager, MethodHideSoftInput, lBinder, flags); 
    }
    
    // Finished with the JVM.
    jvm->DetachCurrentThread(); 
}

// Gets the Keyboard::Key enumeration constant that corresponds to the given Android key code.
static Keyboard::Key getKey(int keycode, int metastate)
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
        case AKEYCODE_ALT_RIGHT:
            return Keyboard::KEY_ALT;
        case AKEYCODE_SHIFT_LEFT:
        case AKEYCODE_SHIFT_RIGHT:
            return Keyboard::KEY_SHIFT;
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
        case AKEYCODE_MENU:
            return Keyboard::KEY_MENU;
        case AKEYCODE_SEARCH:
            return Keyboard::KEY_SEARCH;
        default:
            return Keyboard::KEY_NONE;
    }
}

/**
 * Returns the unicode value for the given keycode or zero if the key is not a valid printable character.
 */
static int getUnicode(int keycode, int metastate)
{
    if (keycode == AKEYCODE_DEL)
        return 0x0008;
    // TODO: Doesn't support unicode currently.
    Keyboard::Key key = getKey(keycode, metastate);
    switch (key)
    {
    case Keyboard::KEY_BACKSPACE:
        return 0x0008;
    case Keyboard::KEY_TAB:
        return 0x0009;
    case Keyboard::KEY_RETURN:
    case Keyboard::KEY_KP_ENTER:
        return 0x000A;
    case Keyboard::KEY_ESCAPE:
        return 0x001B;
    case Keyboard::KEY_SPACE:
    case Keyboard::KEY_EXCLAM:
    case Keyboard::KEY_QUOTE:
    case Keyboard::KEY_NUMBER:
    case Keyboard::KEY_DOLLAR:
    case Keyboard::KEY_PERCENT:
    case Keyboard::KEY_CIRCUMFLEX:
    case Keyboard::KEY_AMPERSAND:
    case Keyboard::KEY_APOSTROPHE:
    case Keyboard::KEY_LEFT_PARENTHESIS:
    case Keyboard::KEY_RIGHT_PARENTHESIS:
    case Keyboard::KEY_ASTERISK:
    case Keyboard::KEY_PLUS:
    case Keyboard::KEY_COMMA:
    case Keyboard::KEY_MINUS:
    case Keyboard::KEY_PERIOD:
    case Keyboard::KEY_SLASH:
    case Keyboard::KEY_ZERO:
    case Keyboard::KEY_ONE:
    case Keyboard::KEY_TWO:
    case Keyboard::KEY_THREE:
    case Keyboard::KEY_FOUR:
    case Keyboard::KEY_FIVE:
    case Keyboard::KEY_SIX:
    case Keyboard::KEY_SEVEN:
    case Keyboard::KEY_EIGHT:
    case Keyboard::KEY_NINE:
    case Keyboard::KEY_COLON:
    case Keyboard::KEY_SEMICOLON:
    case Keyboard::KEY_LESS_THAN:
    case Keyboard::KEY_EQUAL:
    case Keyboard::KEY_GREATER_THAN:
    case Keyboard::KEY_QUESTION:
    case Keyboard::KEY_AT:
    case Keyboard::KEY_CAPITAL_A:
    case Keyboard::KEY_CAPITAL_B:
    case Keyboard::KEY_CAPITAL_C:
    case Keyboard::KEY_CAPITAL_D:
    case Keyboard::KEY_CAPITAL_E:
    case Keyboard::KEY_CAPITAL_F:
    case Keyboard::KEY_CAPITAL_G:
    case Keyboard::KEY_CAPITAL_H:
    case Keyboard::KEY_CAPITAL_I:
    case Keyboard::KEY_CAPITAL_J:
    case Keyboard::KEY_CAPITAL_K:
    case Keyboard::KEY_CAPITAL_L:
    case Keyboard::KEY_CAPITAL_M:
    case Keyboard::KEY_CAPITAL_N:
    case Keyboard::KEY_CAPITAL_O:
    case Keyboard::KEY_CAPITAL_P:
    case Keyboard::KEY_CAPITAL_Q:
    case Keyboard::KEY_CAPITAL_R:
    case Keyboard::KEY_CAPITAL_S:
    case Keyboard::KEY_CAPITAL_T:
    case Keyboard::KEY_CAPITAL_U:
    case Keyboard::KEY_CAPITAL_V:
    case Keyboard::KEY_CAPITAL_W:
    case Keyboard::KEY_CAPITAL_X:
    case Keyboard::KEY_CAPITAL_Y:
    case Keyboard::KEY_CAPITAL_Z:
    case Keyboard::KEY_LEFT_BRACKET:
    case Keyboard::KEY_BACK_SLASH:
    case Keyboard::KEY_RIGHT_BRACKET:
    case Keyboard::KEY_UNDERSCORE:
    case Keyboard::KEY_GRAVE:
    case Keyboard::KEY_A:
    case Keyboard::KEY_B:
    case Keyboard::KEY_C:
    case Keyboard::KEY_D:
    case Keyboard::KEY_E:
    case Keyboard::KEY_F:
    case Keyboard::KEY_G:
    case Keyboard::KEY_H:
    case Keyboard::KEY_I:
    case Keyboard::KEY_J:
    case Keyboard::KEY_K:
    case Keyboard::KEY_L:
    case Keyboard::KEY_M:
    case Keyboard::KEY_N:
    case Keyboard::KEY_O:
    case Keyboard::KEY_P:
    case Keyboard::KEY_Q:
    case Keyboard::KEY_R:
    case Keyboard::KEY_S:
    case Keyboard::KEY_T:
    case Keyboard::KEY_U:
    case Keyboard::KEY_V:
    case Keyboard::KEY_W:
    case Keyboard::KEY_X:
    case Keyboard::KEY_Y:
    case Keyboard::KEY_Z:
    case Keyboard::KEY_LEFT_BRACE:
    case Keyboard::KEY_BAR:
    case Keyboard::KEY_RIGHT_BRACE:
    case Keyboard::KEY_TILDE:
        return key;
    default:
        return 0;
    }
}

// Process the next input event.
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t action = AMotionEvent_getAction(event);
        size_t pointerIndex;
        size_t pointerId;
        size_t pointerCount;
        int x;
        int y;
        
        switch (action & AMOTION_EVENT_ACTION_MASK)
        {
            case AMOTION_EVENT_ACTION_DOWN:
                {
                    pointerId = AMotionEvent_getPointerId(event, 0);
                    x = AMotionEvent_getX(event, 0);
                    y = AMotionEvent_getY(event, 0);

                    // Gesture handling
                    if ( __gestureEventsProcessed.test(Gesture::GESTURE_TAP) ||
                         __gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) )
                    {
                        __pointer0.pressed = true;
                        __pointer0.time = Game::getInstance()->getAbsoluteTime();
                        __pointer0.pointerId = pointerId;
                        __pointer0.x = x;
                        __pointer0.y = y;
                    }

                    // Primary pointer down.
                    gameplay::Platform::touchEventInternal(Touch::TOUCH_PRESS, x, y, pointerId);
                    __primaryTouchId = pointerId;
                }
                break;

            case AMOTION_EVENT_ACTION_UP:
                {
                    pointerId = AMotionEvent_getPointerId(event, 0);
                    x = AMotionEvent_getX(event, 0);
                    y = AMotionEvent_getY(event, 0);
                    
                    // Gestures
                    bool gestureDetected = false;
                    if ( __pointer0.pressed &&  __pointer0.pointerId == pointerId)
                    {
                        int deltaX = x - __pointer0.x;
                        int deltaY = y - __pointer0.y;

                        // Test for swipe
                        if (__gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) &&
                            gameplay::Game::getInstance()->getAbsoluteTime() - __pointer0.time < GESTURE_SWIPE_DURATION_MAX && 
                            (abs(deltaX) > GESTURE_SWIPE_DISTANCE_MIN || abs(deltaY) > GESTURE_SWIPE_DISTANCE_MIN) )
                        {
                            int direction = 0;
                            if ( abs(deltaX) > abs(deltaY) )
                            {
                                if (deltaX > 0)
                                    direction = gameplay::Gesture::SWIPE_DIRECTION_RIGHT;
                                else if (deltaX < 0)
                                    direction = gameplay::Gesture::SWIPE_DIRECTION_LEFT;
                            }
                            else
                            {
                                if (deltaY > 0)
                                    direction = gameplay::Gesture::SWIPE_DIRECTION_DOWN;
                                else if (deltaY < 0)
                                    direction = gameplay::Gesture::SWIPE_DIRECTION_UP;
                            }
                            gameplay::Platform::gestureSwipeEventInternal(x, y, direction);
                            __pointer0.pressed = false;
                            gestureDetected = true;
                        }
                        else if(__gestureEventsProcessed.test(Gesture::GESTURE_TAP) &&
                               gameplay::Game::getInstance()->getAbsoluteTime() - __pointer0.time < GESTURE_TAP_DURATION_MAX)
                        {
                            gameplay::Platform::gestureTapEventInternal(x, y);
                            __pointer0.pressed = false;
                            gestureDetected = true;
                        }
                    }

                    if (!gestureDetected && (__multiTouch || __primaryTouchId == pointerId) )
                    {
                        gameplay::Platform::touchEventInternal(Touch::TOUCH_RELEASE, x, y, pointerId);
                    }
                    __primaryTouchId = -1;
                }
                break;

            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                {
                    pointerId = AMotionEvent_getPointerId(event, 0);
                    x = AMotionEvent_getX(event, 0);
                    y = AMotionEvent_getY(event, 0);

                    // Gesture handling
                    if ( __gestureEventsProcessed.test(Gesture::GESTURE_TAP) ||
                         __gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) )
                    {
                        __pointer1.pressed = true;
                        __pointer1.time = Game::getInstance()->getAbsoluteTime();
                        __pointer1.pointerId = pointerId;
                        __pointer1.x = x;
                        __pointer1.y = y;
                    }

                    // Non-primary pointer down.
                    if (__multiTouch)
                    {
                        pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                        pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                        gameplay::Platform::touchEventInternal(Touch::TOUCH_PRESS, AMotionEvent_getX(event, pointerIndex), AMotionEvent_getY(event, pointerIndex), pointerId);
                    }
                }
                break;

            case AMOTION_EVENT_ACTION_POINTER_UP:
                {
                    pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                    x = AMotionEvent_getX(event, 0);
                    y = AMotionEvent_getY(event, 0);

                    bool gestureDetected = false;
                    if ( __pointer1.pressed &&  __pointer1.pointerId == pointerId)
                    {
                        int deltaX = x - __pointer1.x;
                        int deltaY = y - __pointer1.y;

                        // Test for swipe
                        if (__gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) &&
                            gameplay::Game::getInstance()->getAbsoluteTime() - __pointer1.time < GESTURE_SWIPE_DURATION_MAX && 
                            (abs(deltaX) > GESTURE_SWIPE_DISTANCE_MIN || abs(deltaY) > GESTURE_SWIPE_DISTANCE_MIN) )
                        {
                            int direction = 0;
                            if (deltaX > 0)
                                direction |= gameplay::Gesture::SWIPE_DIRECTION_RIGHT;
                            else if (deltaX < 0)
                                direction |= gameplay::Gesture::SWIPE_DIRECTION_LEFT;
                            
                            if (deltaY > 0)
                                direction |= gameplay::Gesture::SWIPE_DIRECTION_DOWN;
                            else if (deltaY < 0)
                                direction |= gameplay::Gesture::SWIPE_DIRECTION_UP;

                            gameplay::Platform::gestureSwipeEventInternal(x, y, direction);
                            __pointer1.pressed = false;
                            gestureDetected = true;
                        }
                        else if(__gestureEventsProcessed.test(Gesture::GESTURE_TAP) &&
                               gameplay::Game::getInstance()->getAbsoluteTime() - __pointer1.time < GESTURE_TAP_DURATION_MAX)
                        {
                            gameplay::Platform::gestureTapEventInternal(x, y);
                            __pointer1.pressed = false;
                            gestureDetected = true;
                        }
                    }

                    if (!gestureDetected && (__multiTouch || __primaryTouchId == pointerId) )
                    {
                        gameplay::Platform::touchEventInternal(Touch::TOUCH_RELEASE, AMotionEvent_getX(event, pointerIndex), AMotionEvent_getY(event, pointerIndex), pointerId);
                    }
                    if (__primaryTouchId == pointerId)
                        __primaryTouchId = -1;
                }
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                {
                    // ACTION_MOVE events are batched, unlike the other events.
                    pointerCount = AMotionEvent_getPointerCount(event);
                    for (size_t i = 0; i < pointerCount; ++i)
                    {
                        pointerId = AMotionEvent_getPointerId(event, i);
                        if (__multiTouch || __primaryTouchId == pointerId)
                        {
                            gameplay::Platform::touchEventInternal(Touch::TOUCH_MOVE, AMotionEvent_getX(event, i), AMotionEvent_getY(event, i), pointerId);
                        }
                    }
                }
                break;
        }
        return 1;
    } 
    else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
    {
        int32_t action = AKeyEvent_getAction(event);
        int32_t keycode = AKeyEvent_getKeyCode(event);
        int32_t metastate = AKeyEvent_getMetaState(event); 
        
        switch(action)
        {
            case AKEY_EVENT_ACTION_DOWN:
                Game::getInstance()->keyEvent(Keyboard::KEY_PRESS, getKey(keycode, metastate));
                if (int character = getUnicode(keycode, metastate))
                    gameplay::Platform::keyEventInternal(Keyboard::KEY_CHAR, character);
                break;
                    
            case AKEY_EVENT_ACTION_UP:
                gameplay::Platform::keyEventInternal(Keyboard::KEY_RELEASE, getKey(keycode, metastate));
                break;
        }
    }
    return 0;
}

// Process the next main command.
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) 
    {
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL)
            {
                initEGL();
                __initialized = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            destroyEGLSurface();
            __initialized = false;
            break;
        case APP_CMD_DESTROY:
            Game::getInstance()->exit();
            destroyEGLMain();
            __initialized = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the sensors.
            if (__accelerometerSensor != NULL) 
            {
                ASensorEventQueue_enableSensor(__sensorEventQueue, __accelerometerSensor);
                // We'd like to get 60 events per second (in microseconds).
                ASensorEventQueue_setEventRate(__sensorEventQueue, __accelerometerSensor, (1000L/60)*1000);
            }
            if (__gyroscopeSensor != NULL)
            {
                ASensorEventQueue_enableSensor(__sensorEventQueue, __gyroscopeSensor);
                // We'd like to get 60 events per second (in microseconds).
                ASensorEventQueue_setEventRate(__sensorEventQueue, __gyroscopeSensor, (1000L/60)*1000);
            }

            if (Game::getInstance()->getState() == Game::UNINITIALIZED)
            {
                Game::getInstance()->run();
            }
            else
            {
                Game::getInstance()->resume();
            }
            break;
        case APP_CMD_RESUME:
            if (__initialized)
            {
                Game::getInstance()->resume();
            }
            __suspended = false;
            break;
        case APP_CMD_PAUSE:
            Game::getInstance()->pause();
            __suspended = true;
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the sensors.
            // This is to avoid consuming battery while not being used.
            if (__accelerometerSensor != NULL) 
            {
                ASensorEventQueue_disableSensor(__sensorEventQueue, __accelerometerSensor);
            }
            if (__gyroscopeSensor != NULL) 
            {
                ASensorEventQueue_disableSensor(__sensorEventQueue, __gyroscopeSensor);
            }
            break;
    }
}

Platform::Platform(Game* game)
    : _game(game)
{
}

Platform::~Platform()
{
}

Platform* Platform::create(Game* game, void* attachToWindow)
{
    Platform* platform = new Platform(game);
    return platform;
}

int Platform::enterMessagePump()
{
    GP_ASSERT(__state && __state->activity && __state->activity->vm);

    __initialized = false;
    __suspended = false;

    // Get the android application's activity.
    ANativeActivity* activity = __state->activity;
    JavaVM* jvm = __state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR)
    {
        GP_ERROR("Failed to retrieve JVM environment when entering message pump.");
        return -1; 
    }
    GP_ASSERT(env);

    /* Get external files directory on Android; this will result in a directory where all app files
     * should be stored, like /mnt/sdcard/android/<package-name>/files/
     */
    jboolean isCopy;

    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodGetExternalStorage = env->GetMethodID(clazz, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");

    jclass clazzFile = env->FindClass("java/io/File");
    jmethodID methodGetPath = env->GetMethodID(clazzFile, "getPath", "()Ljava/lang/String;");

    // Now has java.io.File object pointing to directory
    jobject objectFile  = env->CallObjectMethod(activity->clazz, methodGetExternalStorage, NULL);
    
    // Now has String object containing path to directory
    jstring stringExternalPath = static_cast<jstring>(env->CallObjectMethod(objectFile, methodGetPath));
    const char* externalPath = env->GetStringUTFChars(stringExternalPath, &isCopy);

    // Set the default path to store the resources.
    std::string assetsPath(externalPath);
    if (externalPath[strlen(externalPath)-1] != '/')
        assetsPath += "/";

    FileSystem::setResourcePath(assetsPath.c_str());    

    // Release string data
    env->ReleaseStringUTFChars(stringExternalPath, externalPath);
    jvm->DetachCurrentThread();
    
    // Get the asset manager to get the resources from the .apk file.
    __assetManager = activity->assetManager; 
    
    // Set the event call back functions.
    __state->onAppCmd = engine_handle_cmd;
    __state->onInputEvent = engine_handle_input;
    
    // Prepare to monitor accelerometer.
    __sensorManager = ASensorManager_getInstance();
    __accelerometerSensor = ASensorManager_getDefaultSensor(__sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    __gyroscopeSensor = ASensorManager_getDefaultSensor(__sensorManager, ASENSOR_TYPE_GYROSCOPE);
    __sensorEventQueue = ASensorManager_createEventQueue(__sensorManager, __state->looper, LOOPER_ID_USER, NULL, NULL);
    
    // Get the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    __timeStart = timespec2millis(&__timespec);
    __timeAbsolute = 0L;
    
    while (true)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;
        
        while ((ident=ALooper_pollAll(!__suspended ? 0 : -1, NULL, &events, (void**)&source)) >= 0) 
        {
            // Process this event.
            if (source != NULL)
                source->process(__state, source);
            
            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER && __accelerometerSensor != NULL)
            {
                ASensorEventQueue_getEvents(__sensorEventQueue, &__sensorEvent, 1);
                if (__sensorEvent.type == ASENSOR_TYPE_ACCELEROMETER)
                {
                    __accelRawX = __sensorEvent.acceleration.x;
                    __accelRawY = __sensorEvent.acceleration.y;
                    __accelRawZ = __sensorEvent.acceleration.z;
                }
                else if (__sensorEvent.type == ASENSOR_TYPE_GYROSCOPE)
                {
                    __gyroRawX = __sensorEvent.vector.x;
                    __gyroRawY = __sensorEvent.vector.y;
                    __gyroRawZ = __sensorEvent.vector.z;
                }
            }

            if (__state->destroyRequested != 0)
            {
                return 0;
            }
        }
        
        // Idle time (no events left to process) is spent rendering.
        // We skip rendering when the app is paused.
        if (__initialized && !__suspended)
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
                EGLint error = eglGetError();
                if (error == EGL_BAD_NATIVE_WINDOW)
                {
                    if (__state->window != NULL)
                    {
                        destroyEGLSurface();
                        initEGL();
                    }
                    __initialized = true;
                }
                else
                {
                    perror("eglSwapBuffers");
                    break;
                }
            }
        }
            
        // Display the keyboard.
        gameplay::displayKeyboard(__state, __displayKeyboard);
    }
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
    return __width;
}
    
unsigned int Platform::getDisplayHeight()
{
    return __height;
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
    double tx, ty, tz;
    ASensorEvent event;
    
    // By default, android accelerometer values are oriented to the portrait mode.
    // flipping the x and y to get the desired landscape mode values.
    switch (__orientationAngle)
    {
    case 90:
        tx = -__accelRawY;
        ty = __accelRawX;
        break;
    case 180:
        tx = -__accelRawX;
        ty = -__accelRawY;
        break;
    case 270:
        tx = __accelRawY;
        ty = -__accelRawX;
        break;
    default:
        tx = __accelRawX;
        ty = __accelRawY;
        break;
    }
    tz = __accelRawZ;

    if (pitch != NULL)
    {
        GP_ASSERT(tx * tx + tz * tz);
        *pitch = -atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
    }
    if (roll != NULL)
    {
        GP_ASSERT(ty * ty + tz * tz);
        *roll = -atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
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
        *argc = 0;
    if (argv)
        *argv = 0;
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
        __displayKeyboard = true;
    else
        __displayKeyboard = false;
}

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    // Pinch currently not implemented
    return evt == gameplay::Gesture::GESTURE_SWIPE || evt == gameplay::Gesture::GESTURE_TAP;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
    switch(evt)
    {
    case Gesture::GESTURE_ANY_SUPPORTED:
        __gestureEventsProcessed.set();
        break;

    case Gesture::GESTURE_TAP:
    case Gesture::GESTURE_SWIPE:
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

    case Gesture::GESTURE_TAP:
    case Gesture::GESTURE_SWIPE:
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

void Platform::pollGamepadState(Gamepad* gamepad)
{
}

bool Platform::launchURL(const char *url)
{
    if (url == NULL || *url == '\0')
        return false;

    bool result = true;

    android_app* state = __state;
    GP_ASSERT(state && state->activity && state->activity->vm);
    JavaVM* jvm = state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint r = jvm->AttachCurrentThread(&env, NULL);
    if (r == JNI_ERR)
    {
        GP_ERROR("Failed to retrieve JVM environment to display keyboard.");
        return false;
    }
    GP_ASSERT(env);

    jclass classActivity = env->FindClass("android/app/NativeActivity");
    jclass classIntent = env->FindClass("android/content/Intent");
    jclass classUri = env->FindClass("android/net/Uri");

    GP_ASSERT(classActivity && classIntent && classUri);

    // Get static field ID Intent.ACTION_VIEW
    jfieldID fieldActionView = env->GetStaticFieldID(classIntent, "ACTION_VIEW", "Ljava/lang/String;");
    GP_ASSERT(fieldActionView);

    // Get string value of Intent.ACTION_VIEW, we'll need that to pass to Intent's constructor later on
    jstring paramActionView = (jstring)env->GetStaticObjectField(classIntent, fieldActionView);
    GP_ASSERT(paramActionView);

    // Get method ID Uri.parse, will be needed to parse the url given into Uri object
    jmethodID methodUriParse = env->GetStaticMethodID(classUri, "parse","(Ljava/lang/String;)Landroid/net/Uri;");
    GP_ASSERT(methodUriParse);

    // Get method ID Activity.startActivity, so we can start the appropriate activity for the View action of our Uri
    jmethodID methodActivityStartActivity = env->GetMethodID(classActivity, "startActivity","(Landroid/content/Intent;)V");
    GP_ASSERT(methodActivityStartActivity);

    // Get method ID Intent constructor, the one that takes action and uri (String;Uri)
    jmethodID methodIntentInit = env->GetMethodID(classIntent, "<init>","(Ljava/lang/String;Landroid/net/Uri;)V");
    GP_ASSERT(methodIntentInit);

    // Convert our url to Java's string and parse it to Uri
    jstring paramUrlString = env->NewStringUTF(url);
    jobject paramUri = env->CallStaticObjectMethod(classUri, methodUriParse, paramUrlString);
    GP_ASSERT(paramUri);

    // Create Intent with Intent.ACTION_VIEW and parsed Uri arguments
    jobject paramIntent = env->NewObject(classIntent, methodIntentInit, paramActionView, paramUri);
    GP_ASSERT(paramIntent);

    // Launch NativeActivity.startActivity with our intent to view the url! state->activity->clazz holds
    // our NativeActivity object
    env->CallVoidMethod(state->activity->clazz, methodActivityStartActivity, paramIntent);

    /* startActivity may throw a ActivitNotFoundException if, well, activity is not found.
       Example: http://<url> is passed to the intent but there is no browser installed in the system
       we need to handle it. */
    jobject exception = env->ExceptionOccurred();

    // We're not lucky here
    if (exception)
    {
        // Print out the exception data to logcat
        env->ExceptionDescribe();

        // Exception needs to be cleared
        env->ExceptionClear();

        // Launching the url failed
        result = false;
    }

    // See you Space Cowboy
    jvm->DetachCurrentThread();
    return result;
}

}

#endif
