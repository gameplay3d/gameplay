#ifdef __RPI__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"

#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <fstream>
#include <bcm_host.h>

#define MAX_GAMEPADS 4

using namespace std;

int __argc = 0;
char** __argv = 0;

enum GamepadAxisInfoFlags
{
    GP_AXIS_SKIP = 0x1,
    GP_AXIS_IS_DPAD = 0x2,
    GP_AXIS_IS_NEG = 0x4,
    GP_AXIS_IS_XAXIS = 0x8,
    GP_AXIS_IS_TRIGGER = 0x10
};

enum GamepadAxisInfoNormalizeFunction
{
    NEG_TO_POS,
    ZERO_TO_POS
};

struct GamepadJoystickAxisInfo
{
    int axisIndex;
    unsigned int joystickIndex;
    unsigned long flags;
    int mappedPosArg;
    int mappedNegArg;
    float deadZone;
    GamepadAxisInfoNormalizeFunction mapFunc;
};

struct GamepadInfoEntry
{
    unsigned int vendorId;
    unsigned int productId;
    const char* productName;
    unsigned int numberOfJS;
    unsigned int numberOfAxes;
    unsigned int numberOfButtons;
    unsigned int numberOfTriggers;

    GamepadJoystickAxisInfo* axes;
    long* buttons;
};


struct ConnectedGamepadDevInfo
{
    dev_t deviceId;
    gameplay::GamepadHandle fd;
    const GamepadInfoEntry& gamepadInfo;
};

struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static bool __mouseCaptured = false;
static float __mouseCapturePointX = 0;
static float __mouseCapturePointY = 0;
static bool __multiSampling = false;
static bool __cursorVisible = true;
static list<ConnectedGamepadDevInfo> __connectedGamepads;

static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
static EGLContext __eglContext = EGL_NO_CONTEXT;
static EGLSurface __eglSurface = EGL_NO_SURFACE;
static EGLConfig __eglConfig = 0;
static int __width;
static int __height;

#include <linux/joystick.h> //included here so i avoid the naming conflict between KEY_* defined in input.h and the ones defined in gameplay/Keyboard.h 
namespace gameplay
{
    extern void print(const char* format, ...)
    {
        GP_ASSERT(format);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
    }
	
    double timespec2millis(struct timespec *a)
    {
        GP_ASSERT(a);
        return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
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

// Initialized EGL resources.
static bool initEGL()
{
    static EGL_DISPMANX_WINDOW_T nativeWindow;
    VC_RECT_T destRect, srcRect;


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

        // Try both 24 and 16-bit depths
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

	eglBindAPI(EGL_OPENGL_ES_API);
        __eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextAttrs);
        if (__eglContext == EGL_NO_CONTEXT)
        {
            checkErrorEGL("eglCreateContext");
            goto error;
        }
    }

    int w,h;
    graphics_get_display_size(0 , &w, &h);
    
    destRect.x=0; destRect.y=0;
    destRect.width = w; destRect.height = h;

    srcRect.x = 0; srcRect.y=0;
    srcRect.width = w << 16; srcRect.height = h << 16;
    
    DISPMANX_DISPLAY_HANDLE_T dispmanDisplay = vc_dispmanx_display_open(0);
    DISPMANX_UPDATE_HANDLE_T dispmanUpdate = vc_dispmanx_update_start(0);
    DISPMANX_ELEMENT_HANDLE_T dispmanElement = vc_dispmanx_element_add ( dispmanUpdate, dispmanDisplay,
		0, &dstRect, 0,&srcRect, DISPMANX_PROTECTION_NONE, 0 ,0,DISPMANX_NO_ROTATE);
    nativeWindow.element = dispmanElement;
    nativeWindow.width =_w;
    nativeWindow.height =_h;
    vc_dispmanx_update_submit_sync( dispmanUpdate );

    __eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, &nativeWindow, eglSurfaceAttrs);
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

    //__orientationAngle = getRotation() * 90;
    
    // Set vsync.
    eglSwapInterval(__eglDisplay, WINDOW_VSYNC ? 1 : 0);
    
    
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

    Platform::Platform(Game* game) : _game(game)
    {
        bcm_host_init();
    }

    Platform::~Platform()
    {
        bcm_host_deinit();
    }
	
Platform* Platform::create(Game* game, void* attachToWindow)
{
    Platform* platform = new Platform(game);
    return platform;
}

   //Will need to be dynamic, also should be handled in Gamepad class
    static const GamepadInfoEntry gamepadLookupTable[] = 
    {
        {0x0,0x0,"GENERIC XBOX360",2,6,20,2, 
                                             (GamepadJoystickAxisInfo[]) {
                                                                     {0,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {1,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {2,1,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {3,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {4,2,GP_AXIS_IS_TRIGGER,0,0,2240,ZERO_TO_POS},
                                                                     {5,2,GP_AXIS_IS_TRIGGER,1,0,2240,ZERO_TO_POS},
                                                                     {-1,0,0,0,0,0,NEG_TO_POS}
                                                                 },
                                             (long[]) {
                                                                          -1,    
                                                                          -1,  
                                                                          -1,  
                                                                          -1, 
                                                                          -1, 
                                                                          Gamepad::BUTTON_UP,
                                                                          Gamepad::BUTTON_DOWN,
                                                                          Gamepad::BUTTON_LEFT,
                                                                          Gamepad::BUTTON_RIGHT,
                                                                          Gamepad::BUTTON_MENU2,
                                                                          Gamepad::BUTTON_MENU1,
                                                                          Gamepad::BUTTON_L3,
                                                                          Gamepad::BUTTON_R3,
                                                                          Gamepad::BUTTON_L1,
                                                                          Gamepad::BUTTON_R1,
                                                                          Gamepad::BUTTON_MENU3,
                                                                          Gamepad::BUTTON_A,
                                                                          Gamepad::BUTTON_B,
                                                                          Gamepad::BUTTON_X,
                                                                          Gamepad::BUTTON_Y
                                                                         }
        },
        {0x79,0x6,"DragonRise Inc. Generic USB Joystick",2,7,12,0, 
                                             (GamepadJoystickAxisInfo[]) {
                                                                     {0,1, GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {1,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {2,0,GP_AXIS_SKIP,0,0,2240,NEG_TO_POS},
                                                                     {3,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {4,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {5,2,GP_AXIS_IS_DPAD, Gamepad::BUTTON_RIGHT, Gamepad::BUTTON_LEFT,2240,NEG_TO_POS},
                                                                     {6,2,GP_AXIS_IS_DPAD, Gamepad::BUTTON_DOWN, Gamepad::BUTTON_UP,2240,NEG_TO_POS},
                                                                     {-1,0,0,0,0,0,NEG_TO_POS}
                                                                 },
                                             (long[]) {
                                                                          Gamepad::BUTTON_Y,    
                                                                          Gamepad::BUTTON_B,  
                                                                          Gamepad::BUTTON_A,  
                                                                          Gamepad::BUTTON_X, 
                                                                          Gamepad::BUTTON_L1, 
                                                                          Gamepad::BUTTON_R1, 
                                                                          Gamepad::BUTTON_L2,    
                                                                          Gamepad::BUTTON_R2,   
                                                                          Gamepad::BUTTON_MENU1,   
                                                                          Gamepad::BUTTON_MENU2,   
                                                                          Gamepad::BUTTON_L3,
                                                                          Gamepad::BUTTON_R3,
                                                                         }
        },
        {0x54c,0x268,"Sony Corp. Batoh Device / PlayStation 3 Controller",2,27,19,2, 
                                             (GamepadJoystickAxisInfo[]) {
                                                                     {0,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {1,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {2,1,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                     {3,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                     {12,1,GP_AXIS_IS_TRIGGER,0,0,2240,ZERO_TO_POS},
                                                                     {13,2,GP_AXIS_IS_TRIGGER,1,0,2240,ZERO_TO_POS},
                                                                     {-1,0,0,0,0,0,NEG_TO_POS}
                                                                 },
                                             (long[]) {
                                                                          Gamepad::BUTTON_MENU1,    
                                                                          Gamepad::BUTTON_L3,  
                                                                          Gamepad::BUTTON_R3,  
                                                                          Gamepad::BUTTON_MENU2, 
                                                                          Gamepad::BUTTON_UP, 
                                                                          Gamepad::BUTTON_RIGHT, 
                                                                          Gamepad::BUTTON_DOWN,    
                                                                          Gamepad::BUTTON_LEFT,   
                                                                          Gamepad::BUTTON_L2,  //Use Trigger Instead of BUTTON_L2? or both should be called
                                                                          Gamepad::BUTTON_R2,  //Use Trigger Instead of BUTTON_R2? or both should be called                                                                        
                                                                          Gamepad::BUTTON_L1,
                                                                          Gamepad::BUTTON_R1,
                                                                          Gamepad::BUTTON_Y,    
                                                                          Gamepad::BUTTON_B,  
                                                                          Gamepad::BUTTON_A,  
                                                                          Gamepad::BUTTON_X, 
                                                                          Gamepad::BUTTON_MENU3, 
                                                                          -1,
                                                                          -1
                                                                         }
        }
    };

    bool isGamepadDevRegistered(dev_t devId)
    {
        for(list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end();++it)
        {
            if(devId == (*it).deviceId) return true;
        }
        return false;
    }

    void closeGamepad(const ConnectedGamepadDevInfo& gamepadDevInfo)
    {
        ::close(gamepadDevInfo.fd);
    }

    void unregisterGamepad(GamepadHandle handle)
    {
        for(list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end();++it)
        {
            if(handle == (*it).fd)
            {
                closeGamepad(*it);
                __connectedGamepads.erase(it);
                return;
            }
        }
    }

    void closeAllGamepads()
    {
        for(list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end();++it)
        {
            closeGamepad(*it);
            __connectedGamepads.erase(it);
        }
    }

    const GamepadInfoEntry& getGamepadMappedInfo(unsigned int vendorId, unsigned int productId, unsigned int numberOfAxes, unsigned int numberOfButtons)
    {
        for(int i=0;i<sizeof(gamepadLookupTable)/sizeof(GamepadInfoEntry);i++)
        {
            const GamepadInfoEntry& curEntry = gamepadLookupTable[i];
            if(curEntry.vendorId == vendorId && curEntry.productId == productId)
            {
                return curEntry;
            }
        }

        for(int i=0;i<sizeof(gamepadLookupTable)/sizeof(GamepadInfoEntry);i++)
        {
            const GamepadInfoEntry& curEntry = gamepadLookupTable[i];
            if(curEntry.vendorId == 0 && curEntry.productId == 0 && curEntry.numberOfAxes == numberOfAxes && curEntry.numberOfButtons == numberOfButtons)
            {
                return curEntry;
            }
        }

        return gamepadLookupTable[0];
    }

    const GamepadInfoEntry& getGamepadMappedInfo(const GamepadHandle handle)
    {
        GP_ASSERT(handle >= 0);

        for(list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end();++it)
        {
            if(handle == (*it).fd)
            {
                return it->gamepadInfo;
            }
        }
        GP_WARN("Gamepad not connected but yet trying to get its data. Falling back to generic one.");
        return gamepadLookupTable[0];
    }

    const GamepadJoystickAxisInfo* tryGetGamepadMappedAxisInfo(const GamepadInfoEntry& gpinfo, unsigned int axisNumber)
    {
        if(axisNumber >= 0 && axisNumber < gpinfo.numberOfAxes)
        {
            int i = 0;
            while(true)
            {
                const GamepadJoystickAxisInfo* curAxisInfo = &gpinfo.axes[i++];
                if(curAxisInfo->axisIndex == axisNumber)
                    return curAxisInfo;
                else if(curAxisInfo->axisIndex < 0)
                    return NULL;
            }
        }
        return NULL;
    }

    bool tryGetGamepadMappedButton(const GamepadInfoEntry& gpinfo, unsigned long btnNumber, long& outMap)
    {
        if(btnNumber >= 0 && btnNumber < gpinfo.numberOfButtons )
        {
            if(gpinfo.buttons[btnNumber] >= 0)
            {
                outMap = gpinfo.buttons[btnNumber];
                return true;
            }
            else
            {
                return false;
            }
        }
        GP_WARN("Unmapped gamepad button: %u.",btnNumber);
        return false;
    }

    unsigned int readIntegerGamepadIdPropery(const char* sysFSIdPath, const char* propertyName)
    {
        unsigned int ret = 0;
        try {
            ifstream propStream;
            propStream.open((string(sysFSIdPath) + propertyName).c_str(),ifstream::in);
            propStream >> std::hex >> ret;
            propStream.close();
        } catch (exception e) {
            GP_WARN("Could not read propery from SysFS for Gamepad: %s", propertyName);
        }
        return ret;
    }

    bool isBlackListed(unsigned int vendorId, unsigned int productId)
    {
        switch(vendorId)
        {
            case 0x0e0f: //virtual machine devices
                if(productId == 0x0003) // Virtual Mouse
                    return true;
        }
        return false;
    }

    void handleConnectedGamepad(dev_t devId, const char* devPath, const char* sysFSIdPath)
    {
        GP_ASSERT(devPath);

        unsigned int vendorId =readIntegerGamepadIdPropery(sysFSIdPath,"vendor");
        unsigned int productId =readIntegerGamepadIdPropery(sysFSIdPath,"product");

        if(isBlackListed(vendorId,productId)) return;

        GamepadHandle handle = ::open(devPath,O_RDONLY | O_NONBLOCK);
        if(handle < 0)
        {
            GP_WARN("Could not open Gamepad device.");
            return;
        }

        if(!(fcntl(handle, F_GETFL) != -1 || errno != EBADF))
            return;

        char axesNum, btnsNum, name[256];
        ioctl(handle, JSIOCGNAME(256), name);
        ioctl (handle, JSIOCGAXES, &axesNum);
        ioctl (handle, JSIOCGBUTTONS, &btnsNum);

        const GamepadInfoEntry& gpInfo = getGamepadMappedInfo(vendorId,productId,(unsigned int)axesNum,(unsigned int)btnsNum);
        unsigned int numJS = gpInfo.numberOfJS;
        unsigned int numTR = gpInfo.numberOfTriggers;


        Platform::gamepadEventConnectedInternal(handle,btnsNum,numJS,numTR,vendorId,productId,"",name);

        ConnectedGamepadDevInfo info = {devId,handle,gpInfo}; 
        __connectedGamepads.push_back(info);
    }

    static float normalizeJoystickAxis(int axisValue, int deadZone, bool zeroToOne)
    {
        int absAxisValue = 0;
        if(zeroToOne)
            absAxisValue = (axisValue + 32767) / 2.0;
        else
             absAxisValue = abs(axisValue);

        if (absAxisValue < deadZone)
        {
            return 0.0f;
        }
        else
        {
            int maxVal = 0;
            int value = 0;
            if(!zeroToOne)
            {
                value = axisValue;
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
                    return 0.0f;
                }
            }
            else
            {
                value = 1;
                maxVal = 32767;
            }

            float ret = value * (absAxisValue - deadZone) / (float)(maxVal - deadZone);
            return ret;
        }
    }

    void enumGamepads()
    {
        const int maxDevs = 16;
        const char* devPathFormat = "/dev/input/js%u";
        const char* sysfsPathFormat = "/sys/class/input/js%u/device/id/";
        char curDevPath[20];

        for(int i=0;i<maxDevs;i++)
        {
            sprintf(curDevPath,devPathFormat,i);
            struct stat gpstat;
            if(::stat(curDevPath,&gpstat) == 0)
            {
                dev_t devid = gpstat.st_rdev;
                if(!isGamepadDevRegistered(devid))
                {
                    char cursysFSPath[35];
                    sprintf(cursysFSPath,sysfsPathFormat,i);
                    handleConnectedGamepad(devid,curDevPath,cursysFSPath);
                }
            }
        }
    }

    void gamepadHandlingLoop()
    {
        enumGamepads();
    }


int Platform::enterMessagePump()
{
	GP_ASSERT(_game);
	initEGL();

    // Get the initial time.
	clock_gettime(CLOCK_REALTIME, &__timespec);
	__timeStart = timespec2millis(&__timespec);
	__timeAbsolute = 0L;

	// Run the game.
	_game->run();
	
	// Message loop.
	while (true)
	{
		if (_game)
		{
			// Game state will be uninitialized if game was closed through Game::exit()
			if (_game->getState() == Game::UNINITIALIZED)
				break;

			_game->frame();
		}

		if (_game)
		{
			// Game state will be uninitialized if game was closed through Game::exit()
			if (_game->getState() == Game::UNINITIALIZED)
				break;

			_game->frame();
		}
		eglSwapBuffers(__eglDisplay, __eglSurface);
	}
	destroyEGLSurface();
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
    // not supported
}

bool Platform::isMultiTouch()
{
    return false;
}

bool Platform::hasAccelerometer()
{
    return true;
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
    // not supported
}

void Platform::shutdownInternal()
{
	closeAllGamepads();
    Game::getInstance()->shutdown();
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

void Platform::pollGamepadState(Gamepad* gamepad)
{
	GP_ASSERT(gamepad);

	struct js_event jevent;
	const GamepadInfoEntry& gpInfo = getGamepadMappedInfo(gamepad->_handle);

	while (read(gamepad->_handle, &jevent, sizeof(struct js_event)) > 0)
	{
		switch (jevent.type)
		{
			case JS_EVENT_BUTTON:
			case JS_EVENT_BUTTON | JS_EVENT_INIT:
				{
					long curMappingIndex = -1;
					if(tryGetGamepadMappedButton(gpInfo, jevent.number, curMappingIndex))
					{
						unsigned int buttons = 0;
						if (jevent.value)
							buttons |= (1 << curMappingIndex);
						else
							buttons &= ~(1 << curMappingIndex);
						gamepad->setButtons(buttons);
					}
					break;
				}
			case JS_EVENT_AXIS:
			case JS_EVENT_AXIS | JS_EVENT_INIT:
				{
					if(jevent.number < gpInfo.numberOfAxes)
					{
						const GamepadJoystickAxisInfo* jsInfo = tryGetGamepadMappedAxisInfo(gpInfo,jevent.number);
						if(jsInfo)
						{
							float val = normalizeJoystickAxis(jevent.value,jsInfo->deadZone,jsInfo->mapFunc == ZERO_TO_POS);
							if(!(jsInfo->flags & GP_AXIS_SKIP))
							{
								if((jsInfo->flags & GP_AXIS_IS_NEG))
									val = -1.0f * val;

								bool not_js_axis = false;
								if((jsInfo->flags & GP_AXIS_IS_DPAD))
								{
									unsigned int buttons = 0;
									if(jevent.value != 0)
										buttons |= (1 << (jevent.value > 0 ? jsInfo->mappedPosArg : jsInfo->mappedNegArg));
									else
									{
										buttons &= ~(1 << jsInfo->mappedPosArg);
										buttons &= ~(1 << jsInfo->mappedNegArg);
									}
									gamepad->setButtons(buttons);
									not_js_axis = true;
								}
								if((jsInfo->flags & GP_AXIS_IS_TRIGGER))
								{
									gamepad->setTriggerValue(jsInfo->mappedPosArg, val);
									not_js_axis = true;
								}

								if(!not_js_axis)
								{
									Vector2 jsVals;
									gamepad->getJoystickValues(jsInfo->joystickIndex,&jsVals);
									if(jsInfo->flags & GP_AXIS_IS_XAXIS)
										jsVals.x = val;
									else
										jsVals.y = val;
									gamepad->setJoystickValue(jsInfo->joystickIndex,jsVals.x,jsVals.y);
								}
							}
						}
					}
				}
				break;

			default: 
				GP_WARN("unhandled gamepad event: %x\n", jevent.type);
		}
	}
	if(errno == ENODEV)
	{
		unregisterGamepad(gamepad->_handle);
		gamepadEventDisconnectedInternal(gamepad->_handle);
	}
}

bool Platform::launchURL(const char *url)
{
    return false;
}

}

#endif
