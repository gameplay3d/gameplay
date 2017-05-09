#ifdef __APPLE__

#include "Base.h"
#include "PlatformMacOS.h"
#include "Game.h"
#include "Graphics.h"
#include "GraphicsVulkan.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

namespace gameplay
{
    
extern Platform* getPlatform()
{
    static Platform* platform = new PlatformMacOS();
    return platform;
}

PlatformMacOS::PlatformMacOS() :
    _graphics(nullptr)
{
}

PlatformMacOS::~PlatformMacOS()
{
}

bool PlatformMacOS::startup(int argc, char** argv)
{
    // Get the game config
    Game::Config* config = Game::getInstance()->getConfig();
    return true;
}

void PlatformMacOS::shutdown()
{
}

int PlatformMacOS::enterMessagePump()
{
    // Start the app here
    return 0;
}

unsigned long PlatformMacOS::getWindow()
{
    return (unsigned long)0;
}

unsigned long PlatformMacOS::getConnection()
{
    return (unsigned long)0;
}

Graphics* PlatformMacOS::getGraphics()
{
    return _graphics;
}

bool PlatformMacOS::isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex)
{
    return false;
}

void PlatformMacOS::getGamepadAxisValues(float* leftVertical, float* leftHorizontal,
                                         float* rightVertical, float* rightHorizontal,
                                         size_t gamepadIndex)
{
}

void PlatformMacOS::getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex)
{
}

void PlatformMacOS::getAccelerometerValues(float* pitch, float* roll)
{
}

void PlatformMacOS::getSensorValues(float* accelX, float* accelY, float* accelZ,
                                    float* gyroX, float* gyroY, float* gyroZ)
{
}

extern void print(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

}

#endif
