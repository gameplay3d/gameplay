#include "Base.h"
#include "PlatformMacOS.h"
#include "Game.h"
#include "FileSystem.h"
#include "Graphics.h"
#include "mtl/GraphicsMTL.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>


namespace gameplay
{
    
extern Platform* getPlatform()
{
    static Platform* platform = new PlatformMacOS();
    return platform;
}

Graphics* Graphics::getGraphics()
{
    if (!_graphics)
    {
        _graphics = new GraphicsMTL();
        _graphics->_api = Graphics::API_MTL;
    }
    return _graphics;
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
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    NSString* path = [bundlePath stringByAppendingString:@"/Contents/Resources/"];
    FileSystem::setAssetPath([path cStringUsingEncoding:NSASCIIStringEncoding]);

    // Get the game config
    std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();

    Graphics::getGraphics()->onInitialize((unsigned long)argc, 0);

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

extern void print(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

}
