#ifdef __APPLE__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <mach/mach_time.h>


using namespace std;
using namespace gameplay;

static const float ACCELEROMETER_X_FACTOR = 90.0f / WINDOW_WIDTH;
static const float ACCELEROMETER_Y_FACTOR = 90.0f / WINDOW_HEIGHT;

@class View;

static View* __view = NULL;
static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;
static bool __hasMouse = false;
static bool __leftMouseDown = false;
static bool __rightMouseDown = false;
static bool __shiftDown = false;

long getMachTimeInMilliseconds(); 
int getKey(unichar keyCode);

@interface UITouch (RotationPointExtension)
- (CGPoint)locationInViewWithRotation:(UIView *)view;
@end
@implementation UITouch (RotationPointExtension)
- (CGPoint)locationInViewWithRotation:(UIView *)view 
{
    CGPoint touchLoc = [self locationInView:view];
    CGPoint rotatedPoint = touchLoc;
    if(view == nil) view = self.window; 
    
    if([[UIDevice currentDevice] orientation] == UIDeviceOrientationPortrait) 
    {
        rotatedPoint.x = touchLoc.x;
        rotatedPoint.y = touchLoc.y;
    } 
    else if([[UIDevice currentDevice] orientation] == UIDeviceOrientationPortraitUpsideDown) 
    {
        rotatedPoint.x = view.frame.size.width - touchLoc.x;
        rotatedPoint.y = view.frame.size.height - touchLoc.y;
    } 
    else if([[UIDevice currentDevice] orientation] == UIDeviceOrientationLandscapeLeft)
    {
        rotatedPoint.x = touchLoc.y;
        rotatedPoint.y = view.frame.size.width - touchLoc.x;
    } 
    else if([[UIDevice currentDevice] orientation] == UIDeviceOrientationLandscapeRight) 
    {
        rotatedPoint.x = view.frame.size.height - touchLoc.y;
        rotatedPoint.y = touchLoc.x;
    }
    return rotatedPoint;
}
@end

@interface View : UIView <UIKeyInput>
{
    EAGLContext* context;	
    CADisplayLink* displayLink;
	GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    GLint framebufferWidth;
    GLint framebufferHeight;    
    NSInteger swapInterval;
    BOOL updating;
    Game* _game;
}

@property (readonly, nonatomic, getter=isUpdating) BOOL updating;
@property (readonly, nonatomic, getter=getContext) EAGLContext* context;

- (void)startUpdating;
- (void)stopUpdating;
- (void)update:(id)sender;
- (void)setSwapInterval:(NSInteger)interval;
- (int)swapInterval;

- (BOOL)showKeyboard;
- (BOOL)dismissKeyboard;
@end

@interface View (Private)
- (void)createFramebuffer;
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
        // Do a sanity check
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
            printError("Invalid OS Version: %s\n", (currSysVer == NULL?"NULL":[currSysVer cStringUsingEncoding:NSASCIIStringEncoding]));
            [self release];
            return nil;
        }
        
        
        // Configure the CAEAGLLayer and setup out the rendering context
        CAEAGLLayer* layer = (CAEAGLLayer *)self.layer;
        layer.opaque = TRUE;
        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, 
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context])
		{
			[self release];
			return nil;
		}
        
        // Initialize Internal Defaults
        displayLink = nil;
        defaultFramebuffer = 0;
        colorRenderbuffer = 0;
        depthRenderbuffer = 0;
        framebufferWidth = 0;
        framebufferHeight = 0;
		swapInterval = 1;        
        updating = FALSE;
        
        [self createFramebuffer];
        
        // Set the resource path and initalize the game
        NSString* bundlePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
        FileSystem::setResourcePath([bundlePath fileSystemRepresentation]); 
        
        _game = Game::getInstance();
        __timeStart = getMachTimeInMilliseconds();
        _game->run(WINDOW_WIDTH, WINDOW_HEIGHT);    // TODO: Handle based on current orientation            
    }
    return self;
}
- (void) dealloc
{
    _game->exit();
    [self deleteFramebuffer];
    
	if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
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
    // Called on 'resize'
    [self deleteFramebuffer];
}

- (void)createFramebuffer
{
    // iOS Requires all content go to a rendering buffer then it is swapped into the windows rendering surface
    assert(defaultFramebuffer == 0);
    //NSLog(@"EAGLView: creating Framebuffer");
    
    // Create the default frame buffer, and render buffer
    glGenFramebuffers(1, &defaultFramebuffer);
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    // request storage, width, and height of the view that we will render in
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    // Sanity check, ensure that the framebuffer is valid
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NSLog(@"ERROR: Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

- (void)deleteFramebuffer
{
    // Deleting the framebuffer and all the buffers it contains
    if (context) 
    {
        [EAGLContext setCurrentContext:context];        
        if (defaultFramebuffer) 
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }        
        if (colorRenderbuffer) 
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
        if (depthRenderbuffer) 
        {
            glDeleteRenderbuffers(1, &depthRenderbuffer);
            depthRenderbuffer = 0;
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

- (void)startUpdating
{
	if (!updating)
	{
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
        [displayLink setFrameInterval:swapInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		updating = TRUE;
	}
}

- (void)stopUpdating
{
	if (updating)
	{
		[displayLink invalidate];
        displayLink = nil;
		updating = FALSE;
	}
}

- (void)update:(id)sender
{   
    if (context != nil)
    {
        [EAGLContext setCurrentContext:context];
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        if (_game && _game->getState() == Game::RUNNING)       
            _game->frame();
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
}

- (BOOL)showKeyboard {
    return [self becomeFirstResponder];
}
- (BOOL)dismissKeyboard {
    return [self resignFirstResponder];
}

/*
 * Virtual Keyboard Support
 */
- (void)insertText:(NSString *)text 
{
    if([text length] == 0) return;
    assert([text length] == 1);
    unichar c = [text characterAtIndex:0];
    int gpk = getKey(c);
    Game::getInstance()->keyEvent(Keyboard::KEY_PRESS, gpk);    
    Game::getInstance()->keyEvent(Keyboard::KEY_RELEASE, gpk);    
}
- (void)deleteBackward 
{
    Game::getInstance()->keyEvent(Keyboard::KEY_PRESS, Keyboard::KEY_BACKSPACE);    
    Game::getInstance()->keyEvent(Keyboard::KEY_RELEASE, Keyboard::KEY_BACKSPACE);    
}
- (BOOL)hasText 
{
    return YES;
}

/*
 * Touch Support
 */
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned int uniqueTouch = 0;
    for(UITouch *t in touches) 
    {
        CGPoint touchLoc = [t locationInViewWithRotation:nil];
        Game::getInstance()->touchEvent(Touch::TOUCH_PRESS, touchLoc.y,  WINDOW_WIDTH - touchLoc.x, uniqueTouch);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent *)event 
{
    unsigned int uniqueTouch = 0;
    for(UITouch* t in touches) 
    {
        CGPoint touchLoc = [t locationInViewWithRotation:nil];
        Game::getInstance()->touchEvent(Touch::TOUCH_RELEASE, touchLoc.y, WINDOW_WIDTH - touchLoc.x, uniqueTouch);
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event 
{
    // No equivalent for this in GamePlay -- treat as touch end
    [self touchesEnded:touches withEvent:event];
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned 
    int uniqueTouch = 0;
    for(UITouch* t in touches) 
    {
        CGPoint touchLoc = [t locationInViewWithRotation:nil];
        Game::getInstance()->touchEvent(Touch::TOUCH_MOVE, touchLoc.y,  WINDOW_WIDTH - touchLoc.x, uniqueTouch);
    }
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
    // Release any cached data, images, etc that aren't in use.
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
    // Return YES for supported orientation, currently support landscape only?
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
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


@interface AppDelegate : UIApplication <UIApplicationDelegate, UIAccelerometerDelegate>
{
    UIWindow* window;
    ViewController* viewController;
}
@end


@implementation AppDelegate

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    [UIApplication sharedApplication].statusBarHidden = YES;
    UIAccelerometer*  accelerometer = [UIAccelerometer sharedAccelerometer];
    accelerometer.updateInterval = 1 / 40.0;    // 40Hz
    accelerometer.delegate = self;
    
    window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    viewController = [[ViewController alloc] init];
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    return YES;
}


- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
    UIAccelerationValue x, y, z;
    x = acceleration.x;
    y = acceleration.y;
    z = acceleration.z;
    
    // Do something with the values.
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
	[super dealloc];
}

@end

long getMachTimeInMilliseconds()
{
    static const int64_t kOneMillion = 1000 * 1000;
    static mach_timebase_info_data_t s_timebase_info;
    
    if (s_timebase_info.denom == 0) 
        (void) mach_timebase_info(&s_timebase_info);
    
    // mach_absolute_time() returns billionth of seconds, so divide by one million to get milliseconds
    return (long)((mach_absolute_time() * s_timebase_info.numer) / (kOneMillion * s_timebase_info.denom));
}

int getKey(unichar keyCode) 
{
    switch(keyCode) {
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
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        [AppDelegate load];
        UIApplicationMain(0, nil, NSStringFromClass([AppDelegate class]), NSStringFromClass([AppDelegate class]));
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
    
    void Platform::getAccelerometerValues(float* pitch, float* roll)
    {
        *pitch = __pitch;
        *roll = __roll;
    }
    
    void Platform::setMultiTouch(bool enabled) 
    {
        __view.multipleTouchEnabled = enabled;
    }
    
    bool Platform::isMultiTouch() 
    {
        return __view.multipleTouchEnabled;
    }
    
    void Platform::swapBuffers()
    {
        if (__view)
            [[__view getContext] presentRenderbuffer:GL_RENDERBUFFER];
    }
    
}

#endif
