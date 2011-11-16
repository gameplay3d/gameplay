/*
 * PlatformMacOSX.cpp
 */

#ifdef __APPLE__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Input.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/OpenGL.h>
#import <mach/mach_time.h>

using namespace std;
using namespace gameplay;


static const float ACCELEROMETER_X_FACTOR = 90.0f / WINDOW_WIDTH;
static const float ACCELEROMETER_Y_FACTOR = 90.0f / WINDOW_HEIGHT;

static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;
static int __lx, __ly;
static bool __hasMouse = false;
static bool __leftMouseDown = false;
static bool __rightMouseDown = false;
static bool __shiftDown = false;

long getMachTimeInMilliseconds()
{
    static const int64_t kOneMillion = 1000 * 1000;
    static mach_timebase_info_data_t s_timebase_info;
    
    if (s_timebase_info.denom == 0) 
        (void) mach_timebase_info(&s_timebase_info);
    
    // mach_absolute_time() returns billionth of seconds, so divide by one million to get milliseconds
    return (long)((mach_absolute_time() * s_timebase_info.numer) / (kOneMillion * s_timebase_info.denom));
}

@class View;

@interface View : NSOpenGLView <NSWindowDelegate> 
{
    CVDisplayLinkRef displayLink;
    
    Game* _game;
}
@end


@implementation View


-(void)windowWillClose:(NSNotification*)note 
{
    _game->exit();
    [[NSApplication sharedApplication] terminate:self];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    [self performSelectorOnMainThread:@selector(update) withObject:nil waitUntilDone:NO];
    
    [pool release];
    return kCVReturnSuccess;
}


-(void) update
{
    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    _game->frame();
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);  
}

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, 
                                      CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(View*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (id) initWithFrame: (NSRect) frame
{    
    _game = Game::getInstance();
    __timeStart = getMachTimeInMilliseconds();
    NSOpenGLPixelFormatAttribute attrs[] = 
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        0
    };
    
    NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pf)
        NSLog(@"OpenGL pixel format not supported.");
    
    self = [super initWithFrame:frame pixelFormat:[pf autorelease]];  
    
    return self;
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    NSString* path = [bundlePath stringByAppendingString:@"/Contents/Resources/"];
    FileSystem::setResourcePath([path cStringUsingEncoding:NSASCIIStringEncoding]);
    _game->run(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    [[self window] setLevel: NSFloatingWindowLevel];
    [[self window] makeKeyAndOrderFront: self];
    [[self window] setTitle: [NSString stringWithUTF8String: ""]];
    
    // Make all the OpenGL calls to setup rendering and build the necessary rendering objects
    [[self openGLContext] makeCurrentContext];
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = __vsync ? 1 : 0;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
}

- (void) dealloc
{       
    _game->exit();
    
    // Release the display link
    CVDisplayLinkRelease(displayLink);
    
    [super dealloc];
}

- (void) mouseDown: (NSEvent*) theEvent
{
    NSPoint point = [theEvent locationInWindow];
    _game->touch(point.x, WINDOW_HEIGHT - point.y, Input::TOUCHEVENT_PRESS);
    __leftMouseDown = true;
}

- (void) mouseUp: (NSEvent*) theEvent
{
    NSPoint point = [theEvent locationInWindow];
    __leftMouseDown = false;
    _game->touch(point.x, WINDOW_HEIGHT - point.y, Input::TOUCHEVENT_RELEASE);
}

- (void) mouseDragged: (NSEvent*) theEvent
{
    NSPoint point = [theEvent locationInWindow];
    if (__leftMouseDown)
    {
        gameplay::Game::getInstance()->touch(point.x, WINDOW_HEIGHT - point.y, gameplay::Input::TOUCHEVENT_MOVE);
    }
}

- (void) rightMouseDown: (NSEvent*) theEvent
{
    __rightMouseDown = true;
     NSPoint point = [theEvent locationInWindow];
    __lx = point.x;
    __ly = WINDOW_HEIGHT - point.y;
}

- (void) rightMouseUp: (NSEvent*) theEvent
{
   __rightMouseDown = false;
}

- (void) rightMouseDragged: (NSEvent*) theEvent
{
    NSPoint point = [theEvent locationInWindow];
    if (__rightMouseDown)
    {
        // Update the pitch and roll by adding the scaled deltas.
        __roll += -(float)(point.x - __lx) * ACCELEROMETER_X_FACTOR;
        __pitch -= (float)(point.y - (WINDOW_HEIGHT - __ly)) * ACCELEROMETER_Y_FACTOR;
    
        // Clamp the values to the valid range.
        __roll = fmaxf(fminf(__roll, 90.0), -90.0);
        __pitch = fmaxf(fminf(__pitch, 90.0), -90.0);
    
        // Update the last X/Y values.
        __lx = point.x;
        __ly = (WINDOW_HEIGHT - point.y);
    }
}

- (void) mouseEntered:(NSEvent *)theEvent
{
    __hasMouse = true;
}

- (void) mouseExited:(NSEvent *)theEvent
{
    __leftMouseDown = false;
    __rightMouseDown = false;
    __hasMouse = false;
}

@end


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
}

Platform* Platform::create(Game* game)
{
    Platform* platform = new Platform(game);
    
    return platform;
}

int Platform::enterMessagePump()
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    NSApplication* NSApp = [NSApplication sharedApplication];
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, 1024, 600);
    
    View* view = [[View alloc] initWithFrame:viewBounds];
    
    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width, 
                                 viewBounds.size.height);
    
    NSWindow *window = [[NSWindow alloc]
                        initWithContentRect:centered
                        styleMask:NSTitledWindowMask | NSClosableWindowMask
                        backing:NSBackingStoreBuffered
                        defer:NO];
    
    [window setContentView:view];
    [window setDelegate:view];
    [view release];
    
    [NSApp run];
    
    [pool release];
    return EXIT_SUCCESS;
}
    
long Platform::getAbsoluteTime()
{
    __timeAbsolute = getMachTimeInMilliseconds();
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
    __vsync = enable;
}

int Platform::getOrientationAngle()
{
    return 0;
}

bool Platform::isAccelerometerSupported()
{
    return true;
}

void Platform::getAccelerometerPitchAndRoll(float* pitch, float* roll)
{
    *pitch = __pitch;
    *roll = __roll;
}
    
}

#endif
