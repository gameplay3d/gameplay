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

static UIView *__rootView;
static long __timeStart;
static long __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;
static int __lx;
static int __ly;
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


@interface View : UIView 
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
- (void)createFramebuffer;
- (void)deleteFramebuffer;

@end

static View* __view = NULL;

@implementation View

@synthesize updating;
@synthesize context;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
    __view = self;
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

- (void) layoutSubviews
{
    [self deleteFramebuffer];
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event 
{
    unsigned int uniqueTouch = 0;
    for(UITouch *t in touches) 
    {
        CGPoint touchLoc = [t locationInView:nil];
        // TODO: Handle this based on orientation
        Game::getInstance()->touchEvent(Touch::TOUCH_PRESS, touchLoc.y,  WINDOW_WIDTH - touchLoc.x, uniqueTouch);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent *)event 
{
    unsigned int uniqueTouch = 0;
    for(UITouch* t in touches) 
    {
        CGPoint touchLoc = [t locationInView:nil];
        // TODO: Handle this based on orientation
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
        CGPoint touchLoc = [t locationInView:nil];
        // TODO: Handle this based on orientation
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
    __rootView = nil;
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
    if(__rootView == nil) 
    {
        __rootView = self.view;
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

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
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
        __rootView.multipleTouchEnabled = enabled;
    }
    
    bool Platform::isMultiTouch() 
    {
        return __rootView.multipleTouchEnabled;
    }
    
    void Platform::swapBuffers()
    {
        if (__view)
            [[__view getContext] presentRenderbuffer:GL_RENDERBUFFER];
    }
    
}

#endif
