#ifdef __APPLE__

#include "Base.h"
#include "Platform.h"
#import <GameKit/GameKit.h>
#import <mach/mach_time.h>
#import "PlatformiOS.h"
#import "BGPAppDelegate.h"
#import "BGPView.h"
#import "BGPViewController.h"


using namespace std;
using namespace gameplay;

int __argc = 0;
char** __argv = 0;

static bool __vsync;
static double __timeAbsolute;


namespace gameplay
{

void print(const char *format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

int strcmpnocase(const char *s1, const char *s2)
{
    return strcasecmp(s1, s2);
}

double getMachTimeInMilliseconds()
{
    static const double kOneMillion = 1000 * 1000;
    static mach_timebase_info_data_t s_timebase_info;

    if (s_timebase_info.denom == 0)
    {
        (void)mach_timebase_info(&s_timebase_info);
    }

    // mach_absolute_time() returns billionth of seconds, so divide by one million to get milliseconds
    GP_ASSERT(s_timebase_info.denom);
    return ((double)mach_absolute_time() * (double)s_timebase_info.numer) / (kOneMillion * (double)s_timebase_info.denom);
}

}

@implementation BGPPlatformManager

+ (BGPPlatformManager*)sharedInstance
{
    static dispatch_once_t predicate;
    static BGPPlatformManager* sharedInstance = nil;

    dispatch_once(&predicate, ^{
        sharedInstance = [[BGPPlatformManager alloc] init];
    });

    return sharedInstance;
}

- (void)getAccelerometerPitch:(float*)pitch roll:(float*)roll
{
    float p = 0.0f;
    float r = 0.0f;

    CMAccelerometerData* accelerometerData = [[self motionManager] accelerometerData];

    if(accelerometerData != nil)
    {
        float tx, ty, tz;

        switch ([[UIApplication sharedApplication] statusBarOrientation])
        {
            case UIInterfaceOrientationLandscapeRight:
                tx = -accelerometerData.acceleration.y;
                ty = accelerometerData.acceleration.x;
                break;

            case UIInterfaceOrientationLandscapeLeft:
                tx = accelerometerData.acceleration.y;
                ty = -accelerometerData.acceleration.x;
                break;

            case UIInterfaceOrientationPortraitUpsideDown:
                tx = -accelerometerData.acceleration.y;
                ty = -accelerometerData.acceleration.x;
                break;

            case UIInterfaceOrientationPortrait:
                tx = accelerometerData.acceleration.x;
                ty = accelerometerData.acceleration.y;
                break;
        }

        tz = accelerometerData.acceleration.z;

        p = atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
        r = atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
    }

    if(pitch != NULL)
        *pitch = p;
    if(roll != NULL)
        *roll = r;
}

- (void)getRawAccelX:(float*)x Y:(float*)y Z:(float*)z
{
    CMAccelerometerData* accelerometerData = [[self motionManager] accelerometerData];
    if(accelerometerData != nil)
    {
        *x = -9.81f * accelerometerData.acceleration.x;
        *y = -9.81f * accelerometerData.acceleration.y;
        *z = -9.81f * accelerometerData.acceleration.z;
    }
}

- (void)getRawGyroX:(float*)x Y:(float*)y Z:(float*)z
{
    CMGyroData* gyroData = [[self motionManager] gyroData];
    if(gyroData != nil)
    {
        *x = gyroData.rotationRate.x;
        *y = gyroData.rotationRate.y;
        *z = gyroData.rotationRate.z;
    }
}

@end

Platform::Platform(Game* game) : _game(game)
{
}

Platform::~Platform()
{
}

Platform* Platform::create(Game* game)
{
    Platform* platform = new Platform(game);
    return platform;
}

int Platform::enterMessagePump()
{
    [BGPAppDelegate load];
    UIApplicationMain(0, nil, nil, NSStringFromClass([BGPAppDelegate class]));
    return EXIT_SUCCESS;
}

void Platform::signalShutdown() 
{
    // Cannot 'exit' an iOS Application
    assert(false);
    [[[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView] stopUpdating];
    exit(0);
}

bool Platform::canExit()
{
    return false;
}

unsigned int Platform::getDisplayWidth()
{
    return [[[[BGPPlatformManager sharedInstance] activeViewController] view] bounds].size.width * [[UIScreen mainScreen] scale];
}

unsigned int Platform::getDisplayHeight()
{
    return [[[[BGPPlatformManager sharedInstance] activeViewController] view] bounds].size.height * [[UIScreen mainScreen] scale];
}

double Platform::getAbsoluteTime()
{
    __timeAbsolute = getMachTimeInMilliseconds();
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
    __vsync = enable;
}

void Platform::swapBuffers()
{
    BGPView* view = [[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView];

    if (view)
    {
        [view swapBuffers];
    }
}
void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

bool Platform::hasAccelerometer()
{
    return true;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    [[BGPPlatformManager sharedInstance] getAccelerometerPitch:pitch roll:roll];
}

void Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    float x, y, z;

    [[BGPPlatformManager sharedInstance] getRawAccelX:&x Y:&y Z:&z];

    if (accelX)
    {
        *accelX = x;
    }

    if (accelY)
    {
        *accelY = y;
    }

    if (accelZ)
    {
        *accelZ = z;
    }

    [[BGPPlatformManager sharedInstance] getRawGyroX:&x Y:&y Z:&z];

    if (gyroX)
    {
        *gyroX = x;
    }

    if (gyroY)
    {
        *gyroY = y;
    }

    if (gyroZ)
    {
        *gyroZ = z;
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

void Platform::setMultiSampling(bool enabled)
{
    //todo
}

bool Platform::isMultiSampling()
{
    return false; //todo
}

void Platform::setMultiTouch(bool enabled) 
{
    [[[[BGPPlatformManager sharedInstance] activeViewController] view] setMultipleTouchEnabled:enabled];
}

bool Platform::isMultiTouch() 
{
    return [[[[BGPPlatformManager sharedInstance] activeViewController] view] isMultipleTouchEnabled];
}

void Platform::displayKeyboard(bool display) 
{
    BGPView* view = [[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView];

    if (view)
    {
        if (display)
        {
            [view showKeyboard];
        }
        else
        {
            [view dismissKeyboard];
        }
    }
}

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return true;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
    [[[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView] registerGesture:evt];
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
    [[[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView] unregisterGesture:evt];
}

bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return [[[[BGPPlatformManager sharedInstance] activeViewController] gamePlayView] isGestureRegistered:evt];
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
}

bool Platform::launchURL(const char *url)
{
    if (url == NULL || *url == '\0')
        return false;

    return [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String: url]]];
}

std::string Platform::displayFileDialog(size_t mode, const char* title, const char* filterDescription, const char* filterExtensions, const char* initialDirectory)
{
    return "";
}

#endif
