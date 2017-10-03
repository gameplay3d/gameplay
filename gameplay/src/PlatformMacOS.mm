#include "Base.h"
#include "PlatformMacOS.h"
#include "Game.h"
#include "FileSystem.h"
#include "Graphics.h"
#include "mtl/GraphicsMTL.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <MetalKit/MTKView.h>

@class View;

static View* __view = nullptr;
static id<MTLDevice> __device = nullptr;
static NSDate*__timeStart = nullptr;
static NSTimeInterval __timeLast;



@interface View : MTKView <NSWindowDelegate, MTKViewDelegate>
{
}
@end


@implementation View

- (instancetype)initWithFrame:(CGRect)frameRect
                       device:(id<MTLDevice>)device
{
    if ((self = [super initWithFrame:frameRect device:device]))
    {
        gameplay::Graphics::getGraphics()->onInitialize((unsigned long)self,
                                                        (unsigned long)device);
        __timeStart = [NSDate date];
        __timeLast = [__timeStart timeIntervalSinceNow];
    }
    return self;
}

- (void)awakeFromNib
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowWillClose:)
                                             name:NSWindowWillCloseNotification
                                             object:[self window]];
}

- (void) windowWillClose:(NSNotification*)notification
{
    [[NSApplication sharedApplication] terminate:self];
}

- (void) drawView
{
    NSTimeInterval timeNow = [__timeStart timeIntervalSinceNow];
    NSTimeInterval elapsedTime = __timeLast - timeNow;
    gameplay::Game::getInstance()->onRender((float)elapsedTime);
    __timeLast = timeNow;
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    int width = (int)size.width;
    int height = (int)size.height;
    gameplay::Graphics::getGraphics()->onResize(width, height);
    gameplay::Game::getInstance()->onResize(width, height);
}

- (void)dealloc
{
    [super dealloc];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [self drawView];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)mouseDown:(NSEvent*)anEvent
{
    puts("mousedown!");
}

- (void)mouseDragged:(NSEvent*)anEvent
{
    puts("mousemoved!");
}

- (void)keyDown:(NSEvent*)anEvent
{
    puts("keydown!");
}

@end

@interface FullscreenWindow : NSWindow
{
}
@end

@implementation FullscreenWindow
- (BOOL)canBecomeKeyWindow
{
    return YES;
}
@end


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
    return true;
}

void PlatformMacOS::shutdown()
{
}

int PlatformMacOS::enterMessagePump()
{
    // Get the game config
    std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();

    // Setup the filesytem path
    //NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    //NSString* path = [bundlePath stringByAppendingString:@"/Contents/Resources/"];
    //FileSystem::setAssetPath([path cStringUsingEncoding:NSASCIIStringEncoding]);

    // Setup the view
    NSAutoreleasePool* pool = [NSAutoreleasePool new];
    NSApplication* app = [NSApplication sharedApplication];
    NSRect viewBounds = NSMakeRect(0, 0, config->width, config->height);
    __device = MTLCreateSystemDefaultDevice();
    __view = [[View alloc] initWithFrame:viewBounds device:__device];

    // Setup fullscreen vs. windowed mode
    NSWindow* window = nullptr;
    if (config->fullscreen)
    {
        NSRect screenBounds = [[NSScreen mainScreen] frame];
        window = [[FullscreenWindow alloc]
                   initWithContentRect:screenBounds
                   styleMask:NSBorderlessWindowMask
                   backing:NSBackingStoreBuffered
                   defer:YES];
    }
    else
    {
        NSRect contentRect = NSMakeRect(0, 0, viewBounds.size.width, viewBounds.size.height);
        window = [[NSWindow alloc]
                   initWithContentRect:contentRect
                   styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask
                   backing:NSBackingStoreBuffered
                   defer:YES];
    }

    [__view setClearColor:MTLClearColorMake(0, 0, 0, 1)];
    [__view setColorPixelFormat:MTLPixelFormatBGRA8Unorm];
    [__view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];
    [__view setDelegate:__view];

    [window setAcceptsMouseMovedEvents:YES];
    [window setContentView:__view];
    [window setDelegate:__view];
    [window makeKeyAndOrderFront:__view];
    [__view release];

    [app run];
    [pool release];

    return 0;
}

unsigned long PlatformMacOS::getWindow()
{
    return (unsigned long)__view;
}

unsigned long PlatformMacOS::getConnection()
{
    return (unsigned long)__device;
}

extern void print(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

}
