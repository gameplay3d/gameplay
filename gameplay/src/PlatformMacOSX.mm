#ifdef __APPLE__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"
#include <unistd.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/OpenGL.h>
#import <mach/mach_time.h>

using namespace std;
using namespace gameplay;

// Default to 720p
static int __width = 1280;
static int __height = 720;

static float ACCELEROMETER_FACTOR_X = 90.0f / __width;
static float ACCELEROMETER_FACTOR_Y = 90.0f / __height;

static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;
static int __lx;
static int __ly;
static bool __hasMouse = false;
static bool __leftMouseDown = false;
static bool __rightMouseDown = false;
static bool __otherMouseDown = false;
static bool __shiftDown = false;
static char* __title = NULL;
static bool __fullscreen = false;
static void* __attachToWindow = NULL;
static bool __mouseCaptured = false;
static CGPoint __mouseCapturePoint;
static bool __cursorVisible = true;

double getMachTimeInMilliseconds()
{
    static const double kOneMillion = 1000 * 1000;
    static mach_timebase_info_data_t s_timebase_info;
    
    if (s_timebase_info.denom == 0) 
        (void) mach_timebase_info(&s_timebase_info);
    
    // mach_absolute_time() returns billionth of seconds, so divide by one million to get milliseconds
    GP_ASSERT(s_timebase_info.denom);
    return ((double)mach_absolute_time() * (double)s_timebase_info.numer) / (kOneMillion * (double)s_timebase_info.denom);
}


@class View;

@interface View : NSOpenGLView <NSWindowDelegate> 
{
    CVDisplayLinkRef displayLink;
    NSRecursiveLock* lock;
    Game* _game;
}

@end

static View* __view = NULL;

@implementation View

-(void)windowWillClose:(NSNotification*)note 
{
    [lock lock];
    _game->exit();
    [lock unlock];
    [[NSApplication sharedApplication] terminate:self];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [self update];
    
    [pool release];
    
    return kCVReturnSuccess;
}

-(void) update
{       
    [lock lock];

    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    if (_game && _game->getState() == Game::RUNNING)
    {
        _game->frame();
    }
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);  
    
    [lock unlock];
}

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, 
                                      CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(View*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (id) initWithFrame: (NSRect) frame
{    
    NSOpenGLPixelFormatAttribute windowedAttrs[] = 
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        0
    };
    NSOpenGLPixelFormatAttribute fullscreenAttrs[] = 
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAScreenMask, (NSOpenGLPixelFormatAttribute)CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
        NSOpenGLPFAFullScreen,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        0
    };
    NSOpenGLPixelFormatAttribute* attrs = __fullscreen ? fullscreenAttrs : windowedAttrs;
    
    NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pf)
        NSLog(@"OpenGL pixel format not supported.");
    
    if((self = [super initWithFrame:frame pixelFormat:[pf autorelease]])) 
    {
        lock = [[NSRecursiveLock alloc] init];
        _game = Game::getInstance();
        __timeStart = getMachTimeInMilliseconds();
    }
    return self;
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    _game->run();
    
    if (__fullscreen)
    {
        [[self window] setLevel: NSMainMenuWindowLevel+1];
        [[self window] setHidesOnDeactivate:YES]; 
    }
    else
    {
        [[self window] setLevel: NSNormalWindowLevel];
    }
    [[self window] makeKeyAndOrderFront: self];
    [[self window] setTitle: [NSString stringWithUTF8String: __title ? __title : ""]];
    
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
    
    GLint dim[2] = {__width, __height};
    CGLSetParameter(cglContext, kCGLCPSurfaceBackingSize, dim);
    CGLEnable(cglContext, kCGLCESurfaceBackingSize);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
}

- (void) dealloc
{   
    [lock lock];
    
    // Release the display link
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    _game->exit();
    
    [lock unlock];

    [super dealloc];
}

- (void)resumeDisplayRenderer 
{
    [lock lock];
    CVDisplayLinkStop(displayLink);
    [lock unlock]; 
}

- (void)haltDisplayRenderer 
{
    [lock lock];
    CVDisplayLinkStop(displayLink);
    [lock unlock];
}

- (void) mouse: (Mouse::MouseEvent) mouseEvent orTouchEvent: (Touch::TouchEvent) touchEvent x: (float) x y: (float) y s: (int) s 
{
    if (!gameplay::Platform::mouseEventInternal(mouseEvent, x, y, s))
    {
        gameplay::Platform::touchEventInternal(touchEvent, x, y, 0);
    }
}

- (void) mouseDown: (NSEvent*) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    __leftMouseDown = true;
    [self mouse: Mouse::MOUSE_PRESS_LEFT_BUTTON orTouchEvent: Touch::TOUCH_PRESS x: point.x y: __height - point.y s: 0];
}

- (void) mouseUp: (NSEvent*) event
{
     NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    __leftMouseDown = false;
    [self mouse: Mouse::MOUSE_RELEASE_LEFT_BUTTON orTouchEvent: Touch::TOUCH_RELEASE x: point.x y: __height - point.y s: 0];
}

- (void)mouseMoved:(NSEvent*) event 
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    

    if (__mouseCaptured)
    {
        point.x = [event deltaX];
        point.y = [event deltaY];

        NSWindow* window = __view.window;
        NSRect rect = window.frame;
        CGPoint centerPoint;
        centerPoint.x = rect.origin.x + (rect.size.width / 2);
        centerPoint.y = rect.origin.y + (rect.size.height / 2);
        CGDisplayMoveCursorToPoint(CGDisplayPrimaryDisplay(NULL), centerPoint);
    }
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_MOVE, point.x, point.y, 0);
}

- (void) mouseDragged: (NSEvent*) event
{
     NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    if (__leftMouseDown)
    {
        [self mouse: Mouse::MOUSE_MOVE orTouchEvent: Touch::TOUCH_MOVE x: point.x y: __height - point.y s: 0];
    }
}

- (void) rightMouseDown: (NSEvent*) event
{
    __rightMouseDown = true;
     NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    __lx = point.x;
    __ly = __height - point.y;    
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_PRESS_RIGHT_BUTTON, point.x, __height - point.y, 0);
}

- (void) rightMouseUp: (NSEvent*) event
{
   __rightMouseDown = false;
    NSPoint point = [event locationInWindow];
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_RELEASE_RIGHT_BUTTON, point.x, __height - point.y, 0);
}

- (void) rightMouseDragged: (NSEvent*) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    if (__rightMouseDown)
    {
        // Update the pitch and roll by adding the scaled deltas.
        __roll += (float)(point.x - __lx) * ACCELEROMETER_FACTOR_X;
        __pitch -= -(float)(point.y - (__height - __ly)) * ACCELEROMETER_FACTOR_Y;
    
        // Clamp the values to the valid range.
        __roll = max(min(__roll, 90.0f), -90.0f);
        __pitch = max(min(__pitch, 90.0f), -90.0f);
    
        // Update the last X/Y values.
        __lx = point.x;
        __ly = (__height - point.y);
    }
    
    // In right-mouse case, whether __rightMouseDown is true or false
    // this should not matter, mouse move is still occuring
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_MOVE, point.x, __height - point.y, 0);
}

- (void)otherMouseDown: (NSEvent *) event 
{
    __otherMouseDown = true;
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_PRESS_MIDDLE_BUTTON, point.x, __height - point.y, 0);
}

- (void)otherMouseUp: (NSEvent *) event 
{
    __otherMouseDown = false;
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_RELEASE_MIDDLE_BUTTON, point.x, __height - point.y, 0);
}

- (void)otherMouseDragged: (NSEvent *) event 
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_MOVE, point.x, __height - point.y, 0);
}

- (void) mouseEntered: (NSEvent*)event
{
    __hasMouse = true;
}

- (void)scrollWheel: (NSEvent *) event 
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameplay::Platform::mouseEventInternal(Mouse::MOUSE_WHEEL, point.x, __height - point.y, (int)([event deltaY] * 10.0f));
}

- (void) mouseExited: (NSEvent*)event
{
    __leftMouseDown = false;
    __rightMouseDown = false;
    __otherMouseDown = false;
    __hasMouse = false;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

int getKey(unsigned short keyCode, unsigned int modifierFlags)
{
    __shiftDown = (modifierFlags & NSShiftKeyMask);
    switch(keyCode)
    {
        case 0x69:
            return Keyboard::KEY_PRINT;
        case 0x35:
            return Keyboard::KEY_ESCAPE;
        case 0x33:
            return Keyboard::KEY_BACKSPACE;
        case 0x30:
            return Keyboard::KEY_TAB;
        case 0x24:
            return Keyboard::KEY_RETURN;
        case 0x72:
            return Keyboard::KEY_INSERT;
        case 0x73:
            return Keyboard::KEY_HOME;
        case 0x74:
            return Keyboard::KEY_PG_UP;
        case 0x79:
            return Keyboard::KEY_PG_DOWN;
        case 0x75:
            return Keyboard::KEY_DELETE;
        case 0x77:
            return Keyboard::KEY_END;
        case 0x7B:
            return Keyboard::KEY_LEFT_ARROW;
        case 0x7C:
            return Keyboard::KEY_RIGHT_ARROW;
        case 0x7E:
            return Keyboard::KEY_UP_ARROW;
        case 0x7D:
            return Keyboard::KEY_DOWN_ARROW;
        case 0x47:
            return Keyboard::KEY_NUM_LOCK;
        case 0x45:
            return Keyboard::KEY_KP_PLUS;
        case 0x4E:
            return Keyboard::KEY_KP_MINUS;
        case 0x43:
            return Keyboard::KEY_KP_MULTIPLY;
        case 0x4B:
            return Keyboard::KEY_KP_DIVIDE;
        case 0x59:
            return Keyboard::KEY_KP_HOME;
        case 0x5B:
            return Keyboard::KEY_KP_UP;
        case 0x5C:
            return Keyboard::KEY_KP_PG_UP;
        case 0x56:
            return Keyboard::KEY_KP_LEFT;
        case 0x57:
            return Keyboard::KEY_KP_FIVE;
        case 0x58:
            return Keyboard::KEY_KP_RIGHT;
        case 0x53:
            return Keyboard::KEY_KP_END;
        case 0x54:
            return Keyboard::KEY_KP_DOWN;
        case 0x55:
            return Keyboard::KEY_KP_PG_DOWN;
        case 0x52:
            return Keyboard::KEY_KP_INSERT;
        case 0x41:
            return Keyboard::KEY_KP_DELETE;
        case 0x7A:
            return Keyboard::KEY_F1;
        case 0x78:
            return Keyboard::KEY_F2;
        case 0x63:
            return Keyboard::KEY_F3;
        case 0x76:
            return Keyboard::KEY_F4;
        case 0x60:
            return Keyboard::KEY_F5;
        case 0x61:
            return Keyboard::KEY_F6;
        case 0x62:
            return Keyboard::KEY_F7;
        case 0x64:
            return Keyboard::KEY_F8;
        case 0x65:
            return Keyboard::KEY_F9;
        case 0x6D:
            return Keyboard::KEY_F10;
        
        // MACOS reserved:
        //return Keyboard::KEY_F11;
        //return Keyboard::KEY_F12;
        // return Keyboard::KEY_PAUSE;
        // return Keyboard::KEY_SCROLL_LOCK;
            
        case 0x31:
            return Keyboard::KEY_SPACE;
        case 0x1D:
            return __shiftDown ? Keyboard::KEY_RIGHT_PARENTHESIS : Keyboard::KEY_ZERO;
        case 0x12:
            return __shiftDown ? Keyboard::KEY_EXCLAM : Keyboard::KEY_ONE;
        case 0x13:
            return __shiftDown ? Keyboard::KEY_AT : Keyboard::KEY_TWO;
        case 0x14:
            return __shiftDown ? Keyboard::KEY_NUMBER : Keyboard::KEY_THREE;
        case 0x15:
            return __shiftDown ? Keyboard::KEY_DOLLAR : Keyboard::KEY_FOUR;
        case 0x17:
            return __shiftDown ? Keyboard::KEY_PERCENT : Keyboard::KEY_FIVE;
        case 0x16:
            return __shiftDown ? Keyboard::KEY_CIRCUMFLEX : Keyboard::KEY_SIX;
        case 0x1A:
            return __shiftDown ? Keyboard::KEY_AMPERSAND : Keyboard::KEY_SEVEN;
        case 0x1C:
            return __shiftDown ? Keyboard::KEY_ASTERISK : Keyboard::KEY_EIGHT;
        case 0x19:
            return __shiftDown ? Keyboard::KEY_LEFT_PARENTHESIS : Keyboard::KEY_NINE;
        case 0x18:
            return __shiftDown ? Keyboard::KEY_EQUAL : Keyboard::KEY_PLUS;
        case 0x2B:
            return __shiftDown ? Keyboard::KEY_LESS_THAN : Keyboard::KEY_COMMA;
        case 0x1B:
            return __shiftDown ? Keyboard::KEY_UNDERSCORE : Keyboard::KEY_MINUS;
        case 0x2F:
            return __shiftDown ? Keyboard::KEY_GREATER_THAN : Keyboard::KEY_PERIOD;
        case 0x29:
            return __shiftDown ? Keyboard::KEY_COLON : Keyboard::KEY_SEMICOLON;
        case 0x2C:
            return __shiftDown ? Keyboard::KEY_QUESTION : Keyboard::KEY_SLASH;
        case 0x32:
            return __shiftDown ? Keyboard::KEY_GRAVE : Keyboard::KEY_TILDE;
        case 0x21:
            return __shiftDown ? Keyboard::KEY_LEFT_BRACE : Keyboard::KEY_LEFT_BRACKET;
        case 0x2A:
            return __shiftDown ? Keyboard::KEY_BAR : Keyboard::KEY_BACK_SLASH;
        case 0x1E:
            return __shiftDown ? Keyboard::KEY_RIGHT_BRACE : Keyboard::KEY_RIGHT_BRACKET;
        case 0x27:
            return __shiftDown ? Keyboard::KEY_QUOTE : Keyboard::KEY_APOSTROPHE;
            
        case 0x00:
             return __shiftDown ? Keyboard::KEY_CAPITAL_A : Keyboard::KEY_A;
        case 0x0B:
             return __shiftDown ? Keyboard::KEY_CAPITAL_B : Keyboard::KEY_B;
        case 0x08:
             return __shiftDown ? Keyboard::KEY_CAPITAL_C : Keyboard::KEY_C;
        case 0x02:
             return __shiftDown ? Keyboard::KEY_CAPITAL_D : Keyboard::KEY_D;
        case 0x0E:
             return __shiftDown ? Keyboard::KEY_CAPITAL_E : Keyboard::KEY_E;
        case 0x03:
             return __shiftDown ? Keyboard::KEY_CAPITAL_F : Keyboard::KEY_F;
        case 0x05:
             return __shiftDown ? Keyboard::KEY_CAPITAL_G : Keyboard::KEY_G;
        case 0x04:
             return __shiftDown ? Keyboard::KEY_CAPITAL_H : Keyboard::KEY_H;
        case 0x22:
             return __shiftDown ? Keyboard::KEY_CAPITAL_I : Keyboard::KEY_I;
        case 0x26:
             return __shiftDown ? Keyboard::KEY_CAPITAL_J : Keyboard::KEY_J;
        case 0x28:
             return __shiftDown ? Keyboard::KEY_CAPITAL_K : Keyboard::KEY_K;
        case 0x25:
             return __shiftDown ? Keyboard::KEY_CAPITAL_L : Keyboard::KEY_L;
        case 0x2E:
             return __shiftDown ? Keyboard::KEY_CAPITAL_M : Keyboard::KEY_M;
        case 0x2D:
             return __shiftDown ? Keyboard::KEY_CAPITAL_N : Keyboard::KEY_N;
        case 0x1F:
             return __shiftDown ? Keyboard::KEY_CAPITAL_O : Keyboard::KEY_O;
        case 0x23:
             return __shiftDown ? Keyboard::KEY_CAPITAL_P : Keyboard::KEY_P;
        case 0x0C:
             return __shiftDown ? Keyboard::KEY_CAPITAL_Q : Keyboard::KEY_Q;
        case 0x0F:
             return __shiftDown ? Keyboard::KEY_CAPITAL_R : Keyboard::KEY_R;
        case 0x01:
             return __shiftDown ? Keyboard::KEY_CAPITAL_S : Keyboard::KEY_S;
        case 0x11:
             return __shiftDown ? Keyboard::KEY_CAPITAL_T : Keyboard::KEY_T;
        case 0x20:
             return __shiftDown ? Keyboard::KEY_CAPITAL_U : Keyboard::KEY_U;
        case 0x09:
             return __shiftDown ? Keyboard::KEY_CAPITAL_V : Keyboard::KEY_V;
        case 0x0D:
             return __shiftDown ? Keyboard::KEY_CAPITAL_W : Keyboard::KEY_W;
        case 0x07:
             return __shiftDown ? Keyboard::KEY_CAPITAL_X : Keyboard::KEY_X;
        case 0x10:
            return __shiftDown ? Keyboard::KEY_CAPITAL_Y : Keyboard::KEY_Y;
        case 0x06:
            return __shiftDown ? Keyboard::KEY_CAPITAL_Z : Keyboard::KEY_Z;

        default:
            return Keyboard::KEY_NONE;
    }
}

- (void)flagsChanged: (NSEvent*)event
{
    unsigned int keyCode = [event keyCode];
    unsigned int flags = [event modifierFlags];
    switch (keyCode) 
    {
        case 0x39:
            gameplay::Platform::keyEventInternal((flags & NSAlphaShiftKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_CAPS_LOCK);
            break;
        case 0x38:
            gameplay::Platform::keyEventInternal((flags & NSShiftKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_SHIFT);
            break;
        case 0x3C:
            gameplay::Platform::keyEventInternal((flags & NSShiftKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_SHIFT);
            break;
        case 0x3A:
            gameplay::Platform::keyEventInternal((flags & NSAlternateKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_ALT);
            break;
        case 0x3D:
            gameplay::Platform::keyEventInternal((flags & NSAlternateKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_ALT);
            break;
        case 0x3B:
            gameplay::Platform::keyEventInternal((flags & NSControlKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_CTRL);
            break;
        case 0x3E:
            gameplay::Platform::keyEventInternal((flags & NSControlKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_CTRL);
            break;
        case 0x37:
            gameplay::Platform::keyEventInternal((flags & NSCommandKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_HYPER);
            break;
        case 0x36:
            gameplay::Platform::keyEventInternal((flags & NSCommandKeyMask) ? Keyboard::KEY_PRESS : Keyboard::KEY_RELEASE, Keyboard::KEY_HYPER);
            break;
    }
}

- (void) keyDown: (NSEvent*) event
{    
    if([event isARepeat] == NO)
    {
        gameplay::Platform::keyEventInternal(Keyboard::KEY_PRESS, getKey([event keyCode], [event modifierFlags]));
    }
}

- (void) keyUp: (NSEvent*) event
{    
    gameplay::Platform::keyEventInternal(Keyboard::KEY_RELEASE, getKey([event keyCode], [event modifierFlags]));
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

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
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
	__attachToWindow = attachToWindow;
    Platform* platform = new Platform(game);
    
    return platform;
}

int Platform::enterMessagePump()
{
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    NSString* path = [bundlePath stringByAppendingString:@"/Contents/Resources/"];
    FileSystem::setResourcePath([path cStringUsingEncoding:NSASCIIStringEncoding]);
    
    // Read window settings from config.
    if (_game->getConfig())
    {
        Properties* config = _game->getConfig()->getNamespace("window", true);
        if (config)
        {
            // Read window title.
            __title = const_cast<char *>(config->getString("title"));

            // Read window size.
            int width = config->getInt("width");
            if (width != 0)
                __width = width;
            int height = config->getInt("height");
            if (height != 0)
                __height = height;

            // Read fullscreen state.
            __fullscreen = config->getBool("fullscreen");
        }
    }

    // Set the scale factors for the mouse movement used to simulate the accelerometer.
    ACCELEROMETER_FACTOR_X = 90.0f / __width;
    ACCELEROMETER_FACTOR_Y = 90.0f / __height;

    NSAutoreleasePool* pool = [NSAutoreleasePool new];
    NSApplication* app = [NSApplication sharedApplication];
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, __width, __height);
    
    __view = [[View alloc] initWithFrame:viewBounds];
    
    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width, 
                                 viewBounds.size.height);
    
    NSWindow* window = NULL;
    if (__fullscreen)
    {
        window = [[FullscreenWindow alloc]
                   initWithContentRect:screenBounds
                   styleMask:NSBorderlessWindowMask
                   backing:NSBackingStoreBuffered
                   defer:NO];
    }
    else
    {
        window = [[NSWindow alloc]
                   initWithContentRect:centered
                   styleMask:NSTitledWindowMask | NSClosableWindowMask
                   backing:NSBackingStoreBuffered
                   defer:NO];
    }
    
    [window setAcceptsMouseMovedEvents:YES];
    [window setContentView:__view];
    [window setDelegate:__view];
    [__view release];
    
    [app run];
    
    [pool release];
    return EXIT_SUCCESS;
}

void Platform::signalShutdown() 
{
    [__view haltDisplayRenderer];

    // Don't perform terminate right away, enqueue to give game object
    // a chance to cleanup
    NSApplication* app = [NSApplication sharedApplication];
    [app performSelectorOnMainThread:@selector(terminate:) withObject:nil waitUntilDone:NO];
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

void Platform::setMultiTouch(bool enabled)
{
}
    
bool Platform::isMultiTouch()
{
    return false;
}
    
void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = __pitch;
    *roll = __roll;
}

bool Platform::hasMouse()
{
    return true;
}

void Platform::setMouseCaptured(bool captured)
{
    if (captured != __mouseCaptured)
    {
        if (captured)
        {
            [NSCursor hide];
        }
        else
        {   
            [NSCursor unhide];
        }
        NSWindow* window = __view.window;
        NSRect rect = window.frame;
        CGPoint centerPoint;
        centerPoint.x = rect.origin.x + (rect.size.width / 2);
        centerPoint.y = rect.origin.y + (rect.size.height / 2);
        CGDisplayMoveCursorToPoint(CGDisplayPrimaryDisplay(NULL), centerPoint);
        __mouseCaptured = captured;
    }
}

bool Platform::isMouseCaptured()
{
    return __mouseCaptured;
}

void Platform::setCursorVisible(bool visible)
{
    if (visible != __cursorVisible)
    {
        if (visible)
        {
             [NSCursor unhide];
        }
        else 
        {
             [NSCursor hide];
        }
        __cursorVisible = visible;
    }
}

bool Platform::isCursorVisible()
{
    return __cursorVisible;
}

void Platform::swapBuffers()
{
    if (__view)
        CGLFlushDrawable((CGLContextObj)[[__view openGLContext] CGLContextObj]);
}

void Platform::displayKeyboard(bool display)
{
    // Do nothing.
}

void Platform::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!Form::touchEventInternal(evt, x, y, contactIndex))
    {
        Game::getInstance()->touchEvent(evt, x, y, contactIndex);
        Game::getInstance()->getScriptController()->touchEvent(evt, x, y, contactIndex);
    }
}
    
void Platform::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (!Form::keyEventInternal(evt, key))
    {
        Game::getInstance()->keyEvent(evt, key);
        Game::getInstance()->getScriptController()->keyEvent(evt, key);
    }
}

bool Platform::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
    {
        return true;
    }
    else if (Game::getInstance()->mouseEvent(evt, x, y, wheelDelta))
    {
        return true;
    }
    else
    {
        return Game::getInstance()->getScriptController()->mouseEvent(evt, x, y, wheelDelta);
    }
}

void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

unsigned int Platform::getGamepadsConnected()
{
    return 0;
}

bool Platform::isGamepadConnected(unsigned int gamepadHandle)
{
    return false;
}

const char* Platform::getGamepadId(unsigned int gamepadHandle)
{
    return NULL;
}

unsigned int Platform::getGamepadButtonCount(unsigned int gamepadHandle)
{
    return 0;
}

bool Platform::getGamepadButtonState(unsigned int gamepadHandle, unsigned int buttonIndex)
{
    return false;
}

unsigned int Platform::getGamepadJoystickCount(unsigned int gamepadHandle)
{
    return 0;
}

bool Platform::isGamepadJoystickActive(unsigned int gamepadHandle, unsigned int joystickIndex)
{
    return false;
}

float Platform::getGamepadJoystickAxisY(unsigned int gamepadHandle, unsigned int joystickIndex)
{
    return 0.0f;
}

float Platform::getGamepadJoystickAxisY(unsigned int gamepadHandle, unsigned int joystickIndex)
{
    return 0.0f;
}

void Platform::getGamepadJoystickAxisValues(unsigned int gamepadHandle, unsigned int joystickIndex, Vector2* outValue)
{
}

unsigned int Platform::getGamepadTriggerCount(unsigned int gamepadHandle)
{
    return 0;
}

float Platform::getGamepadTriggerValue(unsigned int gamepadHandle, unsigned int triggerIndex)
{
    return 0.0f;
}

}

#endif
