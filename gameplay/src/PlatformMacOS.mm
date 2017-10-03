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

@interface View : MTKView <NSWindowDelegate>
{
}
@end


@implementation View

  - (instancetype)initWithCoder:(NSCoder*)coder
  {
      if ((self = [super initWithCoder:coder]))
      {
          // Initialization code here.
      }
      return self;
  }

- (instancetype)initWithFrame:(CGRect)frameRect
                       device:(id<MTLDevice>)device
{
    if ((self = [super initWithFrame:frameRect device:device]))
    {
        // Initialization code here.
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
    id<CAMetalDrawable> drawable = self.currentDrawable;
    id<MTLTexture> texture = drawable.texture;

    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 1.0, 0.0, 1.0);

    id<MTLCommandQueue> commandQueue = [self.device newCommandQueue];
    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    [commandEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];

    [commandQueue release];
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
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    __view = [[View alloc] initWithFrame:viewBounds device:device];

    // Setup fullscreen vs. windowed mode
    NSWindow* window = nullptr;
    if (config->fullscreen)
    {
        NSRect screenBounds = [[NSScreen mainScreen] frame];
        window = [[FullscreenWindow alloc]
                   initWithContentRect:screenBounds
                   styleMask:NSBorderlessWindowMask
                   backing:NSBackingStoreBuffered
                   defer:NO];
    }
    else
    {
        NSRect contentRect = NSMakeRect(0, 0, viewBounds.size.width, viewBounds.size.height);
        window = [[NSWindow alloc]
                   initWithContentRect:contentRect
                   styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask
                   backing:NSBackingStoreBuffered
                   defer:NO];
    }

    [__view setClearColor:MTLClearColorMake(1, 0, 0, 1)];
    [__view setColorPixelFormat:MTLPixelFormatBGRA8Unorm];
    [__view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];

    [window setAcceptsMouseMovedEvents:YES];
    [window setContentView:__view];
    [window setDelegate:__view];
    [__view release];

    [app run];
    [pool release];

    Graphics::getGraphics()->onInitialize((unsigned long)__view, (unsigned long) device);

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
