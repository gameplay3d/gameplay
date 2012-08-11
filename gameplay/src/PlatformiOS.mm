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

@class AppDelegate;
@class View;

static AppDelegate *__appDelegate = NULL;
static View* __view = NULL;

static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;


double getMachTimeInMilliseconds();

int getKey(unichar keyCode);

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
            print("Invalid OS Version: %s\n", (currSysVer == NULL?"NULL":[currSysVer cStringUsingEncoding:NSASCIIStringEncoding]));
            [self release];
            return nil;
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
            [self release];
            return nil;
        }

        if (!defaultFramebuffer)
        {
            [self createFramebuffer];
        }
            
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

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
        
    }
    return self;
}


- (void) dealloc
{
    _game->exit();
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
    // Called on 'resize'
    [self deleteFramebuffer];
}

- (void)createFramebuffer
{
    // iOS Requires all content go to a rendering buffer then it is swapped into the windows rendering surface
    assert(defaultFramebuffer == 0);
    
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

- (void)swapBuffers
{
    if (context != nil)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
}

- (void)startGame 
{
    _game = Game::getInstance();
    __timeStart = getMachTimeInMilliseconds();
    _game->run();  
}

- (void)startUpdating
{
    if (!updating)
    {
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
        [displayLink setFrameInterval:swapInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        _game->resume();
        updating = TRUE;
    }
}

- (void)stopUpdating
{
    if (updating)
    {
        _game->pause();
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
    Platform::keyEventInternal(Keyboard::KEY_RELEASE, key);    
}

- (void)deleteBackward 
{
    Platform::keyEventInternal(Keyboard::KEY_PRESS, Keyboard::KEY_BACKSPACE);    
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
        Platform::touchEventInternal(Touch::TOUCH_PRESS, touchPoint.x * WINDOW_SCALE, touchPoint.y * WINDOW_SCALE, touchID);
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
        Platform::touchEventInternal(Touch::TOUCH_RELEASE, touchPoint.x * WINDOW_SCALE, touchPoint.y * WINDOW_SCALE, touchID);
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
        Platform::touchEventInternal(Touch::TOUCH_MOVE, touchPoint.x * WINDOW_SCALE, touchPoint.y * WINDOW_SCALE, touchID);
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
}

#pragma mark - View lifecycle
- (void)loadView
{
    self.view = [[[View alloc] init] autorelease];
    if(__view == nil) 
    {
        __view = (View*)self.view;
    }
    // Start the game after assigning __view so Platform object knows about it on game start
    [(View*)self.view startGame];
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

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    [__appDelegate getAccelerometerPitch:pitch roll:roll];
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
        [__view swapBuffers];
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
