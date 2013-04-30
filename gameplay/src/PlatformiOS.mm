#ifdef __APPLE__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"
#include <unistd.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <mach/mach_time.h>

#define UIInterfaceOrientationEnum(x) ([x isEqualToString:@"UIInterfaceOrientationPortrait"]?UIInterfaceOrientationPortrait:                        \
                                      ([x isEqualToString:@"UIInterfaceOrientationPortraitUpsideDown"]?UIInterfaceOrientationPortraitUpsideDown:    \
                                      ([x isEqualToString:@"UIInterfaceOrientationLandscapeLeft"]?UIInterfaceOrientationLandscapeLeft:              \
                                        UIInterfaceOrientationLandscapeRight)))
#define DeviceOrientedSize(o)         ((o == UIInterfaceOrientationPortrait || o == UIInterfaceOrientationPortraitUpsideDown)?                      \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale]):  \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale]))

using namespace std;
using namespace gameplay;

// UIScreen bounds are provided as if device was in portrait mode Gameplay defaults to landscape
extern const int WINDOW_WIDTH  = [[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale];
extern const int WINDOW_HEIGHT = [[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale];
extern const int WINDOW_SCALE = [[UIScreen mainScreen] scale];

int __argc = 0;
char** __argv = 0;

@class AppDelegate;
@class View;

static AppDelegate *__appDelegate = NULL;
static View* __view = NULL;

class TouchPoint
{
public:
    unsigned int hashId;
    int x;
    int y;
    bool down;
    
    TouchPoint()
    {
        hashId = 0;
        x = 0;
        y = 0;
        down = false;
    }
};

// more than we'd ever need, to be safe
#define TOUCH_POINTS_MAX (10)
static TouchPoint __touchPoints[TOUCH_POINTS_MAX];

static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;


double getMachTimeInMilliseconds();

int getKey(unichar keyCode);
int getUnicode(int key);

@interface View : UIView <UIKeyInput>
{
    EAGLContext* context;
    CADisplayLink* displayLink;
    BOOL updateFramebuffer;
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    GLint framebufferWidth;
    GLint framebufferHeight;
    GLuint multisampleFramebuffer;
    GLuint multisampleRenderbuffer;
    GLuint multisampleDepthbuffer;
    NSInteger swapInterval;
    BOOL updating;
    Game* game;
    BOOL oglDiscardSupported;
    
    UITapGestureRecognizer *_tapRecognizer;
    UIPinchGestureRecognizer *_pinchRecognizer;
    UISwipeGestureRecognizer *_swipeRecognizer;
}

@property (readonly, nonatomic, getter=isUpdating) BOOL updating;
@property (readonly, nonatomic, getter=getContext) EAGLContext* context;

- (void)startGame;
- (void)startUpdating;
- (void)stopUpdating;
- (void)update:(id)sender;
- (void)setSwapInterval:(NSInteger)interval;
- (int)swapInterval;
- (void)swapBuffers;
- (BOOL)showKeyboard;
- (BOOL)dismissKeyboard;
@end

@interface View (Private)
- (BOOL)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation View

@synthesize updating;
@synthesize context;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame]))
    {
        // A system version of 3.1 or greater is required to use CADisplayLink. 
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
        {
            // Log the system version
            NSLog(@"System Version: %@", currSysVer);
        }
        else
        {
            GP_ERROR("Invalid OS Version: %s\n", (currSysVer == NULL?"NULL":[currSysVer cStringUsingEncoding:NSASCIIStringEncoding]));
            [self release];
            return nil;
        }
        
        // Check for OS 4.0+ features
        if ([currSysVer compare:@"4.0" options:NSNumericSearch] != NSOrderedAscending)
        {
            oglDiscardSupported = YES;
        }
        else
        {
            oglDiscardSupported = NO;
        }
        
        // Configure the CAEAGLLayer and setup out the rendering context
        CGFloat scale = [[UIScreen mainScreen] scale];
        CAEAGLLayer* layer = (CAEAGLLayer *)self.layer;
        layer.opaque = TRUE;
        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, 
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        self.contentScaleFactor = scale;
        layer.contentsScale = scale;
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            GP_ERROR("Failed to make context current.");
            [self release];
            return nil;
        }

        // Initialize Internal Defaults
        displayLink = nil;
        updateFramebuffer = YES;
        defaultFramebuffer = 0;
        colorRenderbuffer = 0;
        depthRenderbuffer = 0;
        framebufferWidth = 0;
        framebufferHeight = 0;
        multisampleFramebuffer = 0;
        multisampleRenderbuffer = 0;
        multisampleDepthbuffer = 0;
        swapInterval = 1;        
        updating = FALSE;
        game = nil;
        
        // Set the resource path and initalize the game
        NSString* bundlePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
        FileSystem::setResourcePath([bundlePath fileSystemRepresentation]); 
    }
    return self;
}

- (void) dealloc
{
    if (game)
        game->exit();
    [self deleteFramebuffer];
    
    if ([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    [context release];
    [super dealloc];
}

- (BOOL)canBecomeFirstResponder 
{
    // Override so we can control the keyboard
    return YES;
}

- (void) layoutSubviews
{
    // Called on 'resize'.
    // Mark that framebuffer needs to be updated.
    // NOTE: Current disabled since we need to have a way to reset the default frame buffer handle
    // in FrameBuffer.cpp (for FrameBuffer:bindDefault). This means that changing orientation at
    // runtime is currently not supported until we fix this.
    //updateFramebuffer = YES;
}

- (BOOL)createFramebuffer
{
    // iOS Requires all content go to a rendering buffer then it is swapped into the windows rendering surface
    assert(defaultFramebuffer == 0);
    
    // Create the default frame buffer
    GL_ASSERT( glGenFramebuffers(1, &defaultFramebuffer) );
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer) );
    
    // Create a color buffer to attach to the frame buffer
    GL_ASSERT( glGenRenderbuffers(1, &colorRenderbuffer) );
    GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer) );
    
    // Associate render buffer storage with CAEAGLLauyer so that the rendered content is display on our UI layer.
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    
    // Attach the color buffer to our frame buffer
    GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer) );
    
    // Retrieve framebuffer size
    GL_ASSERT( glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth) );
    GL_ASSERT( glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight) );
    
    NSLog(@"width: %d, height: %d", framebufferWidth, framebufferHeight);
    
    // If multisampling is enabled in config, create and setup a multisample buffer
    Properties* config = Game::getInstance()->getConfig()->getNamespace("window", true);
    int samples = config ? config->getInt("samples") : 0;
    if (samples < 0)
        samples = 0;
    if (samples)
    {
        // Create multisample framebuffer
        GL_ASSERT( glGenFramebuffers(1, &multisampleFramebuffer) );
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, multisampleFramebuffer) );
        
        // Create multisample render and depth buffers
        GL_ASSERT( glGenRenderbuffers(1, &multisampleRenderbuffer) );
        GL_ASSERT( glGenRenderbuffers(1, &multisampleDepthbuffer) );

        // Try to find a supported multisample configuration starting with the defined sample count
        while (samples)
        {
            GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, multisampleRenderbuffer) );
            GL_ASSERT( glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samples, GL_RGBA8_OES, framebufferWidth, framebufferHeight) );
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multisampleRenderbuffer) );

            GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, multisampleDepthbuffer) );
            GL_ASSERT( glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24_OES, framebufferWidth, framebufferHeight) );
            GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multisampleDepthbuffer) );
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
                break; // success!
            
            NSLog(@"Creation of multisample buffer with samples=%d failed. Attempting to use configuration with samples=%d instead: %x", samples, samples / 2, glCheckFramebufferStatus(GL_FRAMEBUFFER));
            samples /= 2;
        }
        
        //todo: __multiSampling = samples > 0;

        // Re-bind the default framebuffer
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer) );
        
        if (samples == 0)
        {
            // Unable to find a valid/supported multisample configuratoin - fallback to no multisampling
            GL_ASSERT( glDeleteRenderbuffers(1, &multisampleRenderbuffer) );
            GL_ASSERT( glDeleteRenderbuffers(1, &multisampleDepthbuffer) );
            GL_ASSERT( glDeleteFramebuffers(1, &multisampleFramebuffer) );
            multisampleFramebuffer = multisampleRenderbuffer = multisampleDepthbuffer = 0;
        }
    }
    
    // Create default depth buffer and attach to the frame buffer.
    // Note: If we are using multisample buffers, we can skip depth buffer creation here since we only
    // need the color buffer to resolve to.
    if (multisampleFramebuffer == 0)
    {
        GL_ASSERT( glGenRenderbuffers(1, &depthRenderbuffer) );
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer) );
        GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, framebufferWidth, framebufferHeight) );
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer) );
    }
    
    // Sanity check, ensure that the framebuffer is valid
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"ERROR: Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        [self deleteFramebuffer];
        return NO;
    }
    
    // If multisampling is enabled, set the currently bound framebuffer to the multisample buffer
    // since that is the buffer code should be drawing into (and FrameBuffr::initialize will detect
    // and set this bound buffer as the default one during initialization.
    if (multisampleFramebuffer)
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, multisampleFramebuffer) );
    
    return YES;
}

- (void)deleteFramebuffer
{
    if (context) 
    {
        [EAGLContext setCurrentContext:context];        
        if (defaultFramebuffer) 
        {
            GL_ASSERT( glDeleteFramebuffers(1, &defaultFramebuffer) );
            defaultFramebuffer = 0;
        }        
        if (colorRenderbuffer) 
        {
            GL_ASSERT( glDeleteRenderbuffers(1, &colorRenderbuffer) );
            colorRenderbuffer = 0;
        }
        if (depthRenderbuffer) 
        {
            GL_ASSERT( glDeleteRenderbuffers(1, &depthRenderbuffer) );
            depthRenderbuffer = 0;
        }
        if (multisampleFramebuffer)
        {
            GL_ASSERT( glDeleteFramebuffers(1, &multisampleFramebuffer) );
            multisampleFramebuffer = 0;
        }
        if (multisampleRenderbuffer)
        {
            GL_ASSERT( glDeleteRenderbuffers(1, &multisampleRenderbuffer) );
            multisampleRenderbuffer = 0;
        }
        if (multisampleDepthbuffer)
        {
            GL_ASSERT( glDeleteRenderbuffers(1, &multisampleDepthbuffer) );
            multisampleDepthbuffer = 0;
        }
    }
}

- (void)setSwapInterval:(NSInteger)interval
{
    if (interval >= 1)
    {
        swapInterval = interval;        
        if (updating)
        {
            [self stopUpdating];
            [self startUpdating];
        }
    }
}

- (int)swapInterval 
{
    return swapInterval;
}

- (void)swapBuffers
{
    if (context)
    {
        if (multisampleFramebuffer)
        {
            // Multisampling is enabled: resolve the multisample buffer into the default framebuffer
            GL_ASSERT( glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer) );
            GL_ASSERT( glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, multisampleFramebuffer) );
            GL_ASSERT( glResolveMultisampleFramebufferAPPLE() );
            
            if (oglDiscardSupported)
            {
                // Performance hint that the GL driver can discard the contents of the multisample buffers
                // since they have now been resolved into the default framebuffer
                const GLenum discards[]  = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
                GL_ASSERT( glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, discards) );
            }
        }
        else
        {
            if (oglDiscardSupported)
            {
                // Performance hint to the GL driver that the depth buffer is no longer required.
                const GLenum discards[]  = { GL_DEPTH_ATTACHMENT };
                GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer) );
                GL_ASSERT( glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, discards) );
            }
        }
        
        // Present the color buffer
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer) );
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
}

- (void)startGame 
{
    if (game == nil)
    {
        game = Game::getInstance();
        __timeStart = getMachTimeInMilliseconds();
        game->run();
    }
}

- (void)startUpdating
{
    if (!updating)
    {
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
        [displayLink setFrameInterval:swapInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        if (game)
            game->resume();
        updating = TRUE;
    }
}

- (void)stopUpdating
{
    if (updating)
    {
        if (game)
            game->pause();
        [displayLink invalidate];
        displayLink = nil;
        updating = FALSE;
    }
}

- (void)update:(id)sender
{   
    if (context != nil)
    {
        // Ensure our context is current
        [EAGLContext setCurrentContext:context];
        
        // If the framebuffer needs (re)creating, do so
        if (updateFramebuffer)
        {
            updateFramebuffer = NO;
            [self deleteFramebuffer];
            [self createFramebuffer];
            
            // Start the game after our framebuffer is created for the first time.
            if (game == nil)
            {
                [self startGame];
                
                // HACK: Skip the first display update after creating buffers and initializing the game.
                // If we don't do this, the first frame (which includes any drawing during initialization)
                // does not make it to the display for some reason.
                return;
            }
        }

        // Bind our framebuffer for rendering.
        // If multisampling is enabled, bind the multisample buffer - otherwise bind the default buffer
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, multisampleFramebuffer ? multisampleFramebuffer : defaultFramebuffer) );
        GL_ASSERT( glViewport(0, 0, framebufferWidth, framebufferHeight) );
        
        // Execute a single game frame
        if (game)
            game->frame();
        
        // Present the contents of the color buffer
        [self swapBuffers];
    }
}

- (BOOL)showKeyboard 
{
    return [self becomeFirstResponder];
}

- (BOOL)dismissKeyboard 
{
    return [self resignFirstResponder];
}

- (void)insertText:(NSString*)text 
{
    if([text length] == 0) return;
    assert([text length] == 1);
    unichar c = [text characterAtIndex:0];
    int key = getKey(c);
    Platform::keyEventInternal(Keyboard::KEY_PRESS, key);

    int character = getUnicode(key);
    if (character)
    {
        Platform::keyEventInternal(Keyboard::KEY_CHAR, /*character*/c);
    }
    
    Platform::keyEventInternal(Keyboard::KEY_RELEASE, key);
}

- (void)deleteBackward 
{
    Platform::keyEventInternal(Keyboard::KEY_PRESS, Keyboard::KEY_BACKSPACE);    
    Platform::keyEventInternal(Keyboard::KEY_CHAR, getUnicode(Keyboard::KEY_BACKSPACE));
    Platform::keyEventInternal(Keyboard::KEY_RELEASE, Keyboard::KEY_BACKSPACE);    
}

- (BOOL)hasText 
{
    return YES;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned int touchID = 0;
    for(UITouch* touch in touches) 
    {
        CGPoint touchPoint = [touch locationInView:self];
        if(self.multipleTouchEnabled == YES)
        {
            touchID = [touch hash];
        }

        // Nested loop efficiency shouldn't be a concern since both loop sizes are small (<= 10)
        int i = 0;
        while (i < TOUCH_POINTS_MAX && __touchPoints[i].down)
        {
            i++;
        }

        if (i < TOUCH_POINTS_MAX)
        {
            __touchPoints[i].hashId = touchID;
            __touchPoints[i].x = touchPoint.x * WINDOW_SCALE;
            __touchPoints[i].y = touchPoint.y * WINDOW_SCALE;
            __touchPoints[i].down = true;

            Platform::touchEventInternal(Touch::TOUCH_PRESS, __touchPoints[i].x, __touchPoints[i].y, i);
        }
        else
        {
            print("touchesBegan: unable to find free element in __touchPoints");
        }
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned int touchID = 0;
    for(UITouch* touch in touches) 
    {
        CGPoint touchPoint = [touch locationInView:self];
        if(self.multipleTouchEnabled == YES) 
            touchID = [touch hash];

        // Nested loop efficiency shouldn't be a concern since both loop sizes are small (<= 10)
        bool found = false;
        for (int i = 0; !found && i < TOUCH_POINTS_MAX; i++)
        {
            if (__touchPoints[i].down && __touchPoints[i].hashId == touchID)
            {
                __touchPoints[i].down = false;
                Platform::touchEventInternal(Touch::TOUCH_RELEASE, touchPoint.x * WINDOW_SCALE, touchPoint.y * WINDOW_SCALE, i);
                found = true;
            }
        }
        
        if (!found)
        {
            // It seems possible to receive an ID not in the array.
            // The best we can do is clear the whole array.
            for (int i = 0; i < TOUCH_POINTS_MAX; i++)
            {
                if (__touchPoints[i].down)
                {
                    __touchPoints[i].down = false;
                    Platform::touchEventInternal(Touch::TOUCH_RELEASE, __touchPoints[i].x, __touchPoints[i].y, i);
                }
            }
        }
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event 
{
    // No equivalent for this in GamePlay -- treat as touch end
    [self touchesEnded:touches withEvent:event];
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned int touchID = 0;
    for(UITouch* touch in touches) 
    {
        CGPoint touchPoint = [touch locationInView:self];
        if(self.multipleTouchEnabled == YES) 
            touchID = [touch hash];

        // Nested loop efficiency shouldn't be a concern since both loop sizes are small (<= 10)
        for (int i = 0; i < TOUCH_POINTS_MAX; i++)
        {
            if (__touchPoints[i].down && __touchPoints[i].hashId == touchID)
            {
                __touchPoints[i].x = touchPoint.x * WINDOW_SCALE;
                __touchPoints[i].y = touchPoint.y * WINDOW_SCALE;
                Platform::touchEventInternal(Touch::TOUCH_MOVE, __touchPoints[i].x, __touchPoints[i].y, i);
                break;
            }
        }
    }
}

// Gesture support for Mac OS X Trackpads
- (bool)isGestureRegistered: (Gesture::GestureEvent) evt
{
    switch(evt) {
        case Gesture::GESTURE_SWIPE:
            return (_swipeRecognizer != NULL);
        case Gesture::GESTURE_PINCH:
            return (_pinchRecognizer != NULL);
        case Gesture::GESTURE_TAP:
            return (_tapRecognizer != NULL);
        default:
            break;
    }
    return false;
}

- (void)registerGesture: (Gesture::GestureEvent) evt
{
    if((evt & Gesture::GESTURE_SWIPE) == Gesture::GESTURE_SWIPE && _swipeRecognizer == NULL)
    {
        // right swipe (default)
        _swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
        [self addGestureRecognizer:_swipeRecognizer];

        // left swipe
        UISwipeGestureRecognizer *swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
        swipeGesture.direction = UISwipeGestureRecognizerDirectionLeft;
        [self addGestureRecognizer:swipeGesture];
        [swipeGesture release];
        
        // up swipe
        UISwipeGestureRecognizer *swipeGesture2 = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
        swipeGesture2.direction = UISwipeGestureRecognizerDirectionUp;
        [self addGestureRecognizer:swipeGesture2];
        [swipeGesture2 release];
        
        // down swipe
        UISwipeGestureRecognizer *swipeGesture3 = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
        swipeGesture3.direction = UISwipeGestureRecognizerDirectionDown;
        [self addGestureRecognizer:swipeGesture3];
        [swipeGesture3 release];
    }
    if((evt & Gesture::GESTURE_PINCH) == Gesture::GESTURE_PINCH && _pinchRecognizer == NULL)
    {
        _pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinchGesture:)];
        [self addGestureRecognizer:_pinchRecognizer];
    }
    if((evt & Gesture::GESTURE_TAP) == Gesture::GESTURE_TAP && _tapRecognizer == NULL)
    {
        _tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapGesture:)];
        [self addGestureRecognizer:_tapRecognizer];
    }
}

- (void)unregisterGesture: (Gesture::GestureEvent) evt
{
    if((evt & Gesture::GESTURE_SWIPE) == Gesture::GESTURE_SWIPE && _swipeRecognizer != NULL)
    {
        [self removeGestureRecognizer:_swipeRecognizer];
        [_swipeRecognizer release];
        _swipeRecognizer = NULL;
    }
    if((evt & Gesture::GESTURE_PINCH) == Gesture::GESTURE_PINCH && _pinchRecognizer != NULL)
    {
        [self removeGestureRecognizer:_pinchRecognizer];
        [_pinchRecognizer release];
        _pinchRecognizer = NULL;
    }
    if((evt & Gesture::GESTURE_TAP) == Gesture::GESTURE_TAP && _tapRecognizer != NULL)
    {
        [self removeGestureRecognizer:_tapRecognizer];
        [_tapRecognizer release];
        _tapRecognizer = NULL;
    }
}

- (void)handleTapGesture:(UITapGestureRecognizer*)sender
{
    CGPoint location = [sender locationInView:self];
    gameplay::Platform::gestureTapEventInternal(location.x, location.y);
}

- (void)handlePinchGesture:(UIPinchGestureRecognizer*)sender
{
    CGFloat factor = [sender scale];
    CGPoint location = [sender locationInView:self];
    gameplay::Platform::gesturePinchEventInternal(location.x, location.y, factor);
}

- (void)handleSwipeGesture:(UISwipeGestureRecognizer*)sender
{
    UISwipeGestureRecognizerDirection direction = [sender direction];
    CGPoint location = [sender locationInView:self];
    int gameplayDirection = 0;
    switch(direction) {
        case UISwipeGestureRecognizerDirectionRight:
            gameplayDirection = Gesture::SWIPE_DIRECTION_RIGHT;
            break;
        case UISwipeGestureRecognizerDirectionLeft:
            gameplayDirection = Gesture::SWIPE_DIRECTION_LEFT;
            break;
        case UISwipeGestureRecognizerDirectionUp:
            gameplayDirection = Gesture::SWIPE_DIRECTION_UP;
            break;
        case UISwipeGestureRecognizerDirectionDown:
            gameplayDirection = Gesture::SWIPE_DIRECTION_DOWN;
            break;
    }
    gameplay::Platform::gestureSwipeEventInternal(location.x, location.y, gameplayDirection);
}

@end


@interface ViewController : UIViewController
- (void)startUpdating;
- (void)stopUpdating;
@end


@implementation ViewController 

- (id)init 
{
    if((self = [super init])) 
    {
    }
    return self;
}

- (void)dealloc 
{
    __view = nil;
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - View lifecycle
- (void)loadView
{
    self.view = [[[View alloc] init] autorelease];
    if(__view == nil) 
    {
        __view = (View*)self.view;
    }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Fetch the supported orientations array
    NSArray *supportedOrientations = NULL;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) 
    {
        supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations~ipad"];    
    }
    else if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
    { 
        supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations~iphone"];
    }
    
    if(supportedOrientations == NULL) 
    {
       supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations"]; 
    }

    // If no supported orientations default to v1.0 handling (landscape only)
    if(supportedOrientations == nil) {
        return UIInterfaceOrientationIsLandscape(interfaceOrientation);
    }
    for(NSString *s in supportedOrientations) {
        if(interfaceOrientation == UIInterfaceOrientationEnum(s)) return YES;
    }    
    return NO;
}

- (void)startUpdating 
{
    [(View*)self.view startUpdating];
}

- (void)stopUpdating 
{
    [(View*)self.view stopUpdating];
}

@end


@interface AppDelegate : UIApplication <UIApplicationDelegate>
{
    UIWindow* window;
    ViewController* viewController;
    CMMotionManager *motionManager;
}
@property (nonatomic, retain) ViewController *viewController;
@end


@implementation AppDelegate

@synthesize viewController;

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    __appDelegate = self;
    [UIApplication sharedApplication].statusBarHidden = YES;
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    motionManager = [[CMMotionManager alloc] init];
    if([motionManager isAccelerometerAvailable] == YES) 
    {
        motionManager.accelerometerUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startAccelerometerUpdates];
    }
    if([motionManager isGyroAvailable] == YES)
    {
        motionManager.gyroUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startGyroUpdates];
    }
    
    window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    viewController = [[ViewController alloc] init];
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    return YES;
}

- (void)getAccelerometerPitch:(float*)pitch roll:(float*)roll 
{
    float p = 0.0f;
    float r = 0.0f;
    CMAccelerometerData* accelerometerData = motionManager.accelerometerData;
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
    CMAccelerometerData* accelerometerData = motionManager.accelerometerData;
    if(accelerometerData != nil)
    {
        *x = -9.81f * accelerometerData.acceleration.x;
        *y = -9.81f * accelerometerData.acceleration.y;
        *z = -9.81f * accelerometerData.acceleration.z;
    }
}

- (void)getRawGyroX:(float*)x Y:(float*)y Z:(float*)z
{
    CMGyroData* gyroData = motionManager.gyroData;
    if(gyroData != nil)
    {
        *x = gyroData.rotationRate.x;
        *y = gyroData.rotationRate.y;
        *z = gyroData.rotationRate.z;
    }
}

- (void)applicationWillResignActive:(UIApplication*)application
{    
    [viewController stopUpdating];
}

- (void)applicationDidEnterBackground:(UIApplication*)application 
{
    [viewController stopUpdating];
}

- (void)applicationWillEnterForeground:(UIApplication*)application 
{    
    [viewController startUpdating];
}

- (void)applicationDidBecomeActive:(UIApplication*)application 
{    
    [viewController startUpdating];
}

- (void)applicationWillTerminate:(UIApplication*)application 
{    
    [viewController stopUpdating];
}

- (void)dealloc 
{
    [window setRootViewController:nil];
    [viewController release];
    [window release];
    [motionManager release];
    [super dealloc];
}

@end


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

int getKey(unichar keyCode) 
{
    switch(keyCode) 
    {
        case 0x0A:
            return Keyboard::KEY_RETURN;
        case 0x20:
            return Keyboard::KEY_SPACE;
            
        case 0x30:
            return Keyboard::KEY_ZERO;
        case 0x31:
            return Keyboard::KEY_ONE;
        case 0x32:
            return Keyboard::KEY_TWO;
        case 0x33:
            return Keyboard::KEY_THREE;
        case 0x34:
            return Keyboard::KEY_FOUR;
        case 0x35:
            return Keyboard::KEY_FIVE;
        case 0x36:
            return Keyboard::KEY_SIX;
        case 0x37:
            return Keyboard::KEY_SEVEN;
        case 0x38:
            return Keyboard::KEY_EIGHT;
        case 0x39:
            return Keyboard::KEY_NINE;
            
        case 0x41:
            return Keyboard::KEY_CAPITAL_A;
        case 0x42:
            return Keyboard::KEY_CAPITAL_B;
        case 0x43:
            return Keyboard::KEY_CAPITAL_C;
        case 0x44:
            return Keyboard::KEY_CAPITAL_D;
        case 0x45:
            return Keyboard::KEY_CAPITAL_E;
        case 0x46:
            return Keyboard::KEY_CAPITAL_F;
        case 0x47:
            return Keyboard::KEY_CAPITAL_G;
        case 0x48:
            return Keyboard::KEY_CAPITAL_H;
        case 0x49:
            return Keyboard::KEY_CAPITAL_I;
        case 0x4A:
            return Keyboard::KEY_CAPITAL_J;
        case 0x4B:
            return Keyboard::KEY_CAPITAL_K;
        case 0x4C:
            return Keyboard::KEY_CAPITAL_L;
        case 0x4D:
            return Keyboard::KEY_CAPITAL_M;
        case 0x4E:
            return Keyboard::KEY_CAPITAL_N;
        case 0x4F:
            return Keyboard::KEY_CAPITAL_O;
        case 0x50:
            return Keyboard::KEY_CAPITAL_P;
        case 0x51:
            return Keyboard::KEY_CAPITAL_Q;
        case 0x52:
            return Keyboard::KEY_CAPITAL_R;
        case 0x53:
            return Keyboard::KEY_CAPITAL_S;
        case 0x54:
            return Keyboard::KEY_CAPITAL_T;
        case 0x55:
            return Keyboard::KEY_CAPITAL_U;
        case 0x56:
            return Keyboard::KEY_CAPITAL_V;
        case 0x57:
            return Keyboard::KEY_CAPITAL_W;
        case 0x58:
            return Keyboard::KEY_CAPITAL_X;
        case 0x59:
            return Keyboard::KEY_CAPITAL_Y;
        case 0x5A:
            return Keyboard::KEY_CAPITAL_Z;
            
            
        case 0x61:
            return Keyboard::KEY_A;
        case 0x62:
            return Keyboard::KEY_B;
        case 0x63:
            return Keyboard::KEY_C;
        case 0x64:
            return Keyboard::KEY_D;
        case 0x65:
            return Keyboard::KEY_E;
        case 0x66:
            return Keyboard::KEY_F;
        case 0x67:
            return Keyboard::KEY_G;
        case 0x68:
            return Keyboard::KEY_H;
        case 0x69:
            return Keyboard::KEY_I;
        case 0x6A:
            return Keyboard::KEY_J;
        case 0x6B:
            return Keyboard::KEY_K;
        case 0x6C:
            return Keyboard::KEY_L;
        case 0x6D:
            return Keyboard::KEY_M;
        case 0x6E:
            return Keyboard::KEY_N;
        case 0x6F:
            return Keyboard::KEY_O;
        case 0x70:
            return Keyboard::KEY_P;
        case 0x71:
            return Keyboard::KEY_Q;
        case 0x72:
            return Keyboard::KEY_R;
        case 0x73:
            return Keyboard::KEY_S;
        case 0x74:
            return Keyboard::KEY_T;
        case 0x75:
            return Keyboard::KEY_U;
        case 0x76:
            return Keyboard::KEY_V;
        case 0x77:
            return Keyboard::KEY_W;
        case 0x78:
            return Keyboard::KEY_X;
        case 0x79:
            return Keyboard::KEY_Y;
        case 0x7A:
            return Keyboard::KEY_Z;
        default:
            break;
            
       // Symbol Row 3
        case 0x2E:
            return Keyboard::KEY_PERIOD;
        case 0x2C:
            return Keyboard::KEY_COMMA;
        case 0x3F:
            return Keyboard::KEY_QUESTION;
        case 0x21:
            return Keyboard::KEY_EXCLAM;
        case 0x27:
            return Keyboard::KEY_APOSTROPHE;
            
        // Symbols Row 2
        case 0x2D:
            return Keyboard::KEY_MINUS;
        case 0x2F:
            return Keyboard::KEY_SLASH;
        case 0x3A:
            return Keyboard::KEY_COLON;
        case 0x3B:
            return Keyboard::KEY_SEMICOLON;
        case 0x28:
            return Keyboard::KEY_LEFT_PARENTHESIS;
        case 0x29:
            return Keyboard::KEY_RIGHT_PARENTHESIS;
        case 0x24:
            return Keyboard::KEY_DOLLAR;
        case 0x26:
            return Keyboard::KEY_AMPERSAND;
        case 0x40:
            return Keyboard::KEY_AT;
        case 0x22:
            return Keyboard::KEY_QUOTE;
            
        // Numeric Symbols Row 1
        case 0x5B:
            return Keyboard::KEY_LEFT_BRACKET;
        case 0x5D:
            return Keyboard::KEY_RIGHT_BRACKET;
        case 0x7B:
            return Keyboard::KEY_LEFT_BRACE;
        case 0x7D:
            return Keyboard::KEY_RIGHT_BRACE;
        case 0x23:
            return Keyboard::KEY_NUMBER;
        case 0x25:
            return Keyboard::KEY_PERCENT;
        case 0x5E:
            return Keyboard::KEY_CIRCUMFLEX;
        case 0x2A:
            return Keyboard::KEY_ASTERISK;
        case 0x2B:
            return Keyboard::KEY_PLUS;
        case 0x3D:
            return Keyboard::KEY_EQUAL;
            
        // Numeric Symbols Row 2
        case 0x5F:
            return Keyboard::KEY_UNDERSCORE;
        case 0x5C:
            return Keyboard::KEY_BACK_SLASH;
        case 0x7C:
            return Keyboard::KEY_BAR;
        case 0x7E:
            return Keyboard::KEY_TILDE;
        case 0x3C:
            return Keyboard::KEY_LESS_THAN;
        case 0x3E:
            return Keyboard::KEY_GREATER_THAN;
        case 0x80:
            return Keyboard::KEY_EURO;
        case 0xA3:
            return Keyboard::KEY_POUND;
        case 0xA5:
            return Keyboard::KEY_YEN;
        case 0xB7:
            return Keyboard::KEY_MIDDLE_DOT;
    }
    return Keyboard::KEY_NONE;
}

/**
 * Returns the unicode value for the given keycode or zero if the key is not a valid printable character.
 */
int getUnicode(int key)
{
    
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

Platform::Platform(Game* game) : _game(game)
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
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    [AppDelegate load];
    UIApplicationMain(0, nil, NSStringFromClass([AppDelegate class]), NSStringFromClass([AppDelegate class]));
    [pool release];
    return EXIT_SUCCESS;
}

void Platform::signalShutdown() 
{
    // Cannot 'exit' an iOS Application
    assert(false);
    [__view stopUpdating];
    exit(0);
}

bool Platform::canExit()
{
    return false;
}

unsigned int Platform::getDisplayWidth()
{
    CGSize size = DeviceOrientedSize([__appDelegate.viewController interfaceOrientation]);
    return size.width;
}

unsigned int Platform::getDisplayHeight()
{
    CGSize size = DeviceOrientedSize([__appDelegate.viewController interfaceOrientation]);
    return size.height;
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
    if (__view)
        [__view swapBuffers];
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
    [__appDelegate getAccelerometerPitch:pitch roll:roll];
}

void Platform::getRawSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    float x, y, z;
    [__appDelegate getRawAccelX:&x Y:&y Z:&z];
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

    [__appDelegate getRawGyroX:&x Y:&y Z:&z];
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
    __view.multipleTouchEnabled = enabled;
}

bool Platform::isMultiTouch() 
{
    return __view.multipleTouchEnabled;
}

void Platform::displayKeyboard(bool display) 
{
    if(__view) 
    {
        if(display)
        {
            [__view showKeyboard];
        }
        else
        {
            [__view dismissKeyboard];
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
    [__view registerGesture:evt];
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
    [__view unregisterGesture:evt];
}

bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return [__view isGestureRegistered:evt];
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
    
}

#endif
