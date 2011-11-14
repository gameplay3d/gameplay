#ifndef GAMEPLAYMAINMACOSX__H
#define GAMEPLAYMAINMACOSX__H

#include "gameplay.h"

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <mach/mach_time.h>

@class View;

@interface View : NSOpenGLView <NSWindowDelegate> 
{
    NSRect _frameRect;
    bool _initialized;
    uint64_t _timeLast;
    NSTimer* _timer;
}

- (void) animate;

@end


@implementation View

-(void)windowWillClose:(NSNotification *)note 
{
    [[NSApplication sharedApplication] terminate:self];
}

- (void) timerFired:(NSTimer*) timer
{
    // _game->update();
    
    [self display];    
}

- (id) initWithFrame: (NSRect) frame
{
    _initialized = FALSE;
    
    int attribs[] = 
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFANoRecovery,
        0
    };
    
    NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:(NSOpenGLPixelFormatAttribute*) attribs];
    
    self = [self initWithFrame:frame pixelFormat:fmt];
    
    [fmt release];
    
    _frameRect = frame;
    _timeLast = mach_absolute_time();
    _timeLast = [[NSTimer
                scheduledTimerWithTimeInterval:1.0f/60.0f
                target:self 
                selector:@selector(timerFired:)
                userInfo:nil
                repeats:YES] retain];
    
    return self;
}

- (void) drawRect:(NSRect) theRect
{
    if (!_initialized) 
    {
        
        int transparentWindow = 0;
        if (transparentWindow) 
        {
            int opaque = NO;
            [[self openGLContext]
             setValues:&opaque
             forParameter:NSOpenGLCPSurfaceOpacity];
            [[self window] setOpaque:NO];
            [[self window] setAlphaValue:0.99];
        }
        
        //glewInit();
        // _game->initialize();
        //const char* szTitle = PezInitialize(PEZ_VIEWPORT_WIDTH, PEZ_VIEWPORT_HEIGHT);
        _initialized = true;
        
        [[self window] setLevel: NSFloatingWindowLevel];
        [[self window] makeKeyAndOrderFront: self];
        [[self window] setTitle: [NSString stringWithUTF8String: ""]];
    }
    
    //_game->render();
    glClear(GL_COLOR_BUFFER_BIT);
    
    [[self openGLContext] flushBuffer]; 
}

- (void) mouseUp: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    // _game->touchEvent
}

- (void) mouseDown: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    // _game->touchEvent
}

- (void) onKey: (unichar) character downEvent: (BOOL) flag
{
    switch(character) 
    {
        case 27:
        case 'q':
            [[NSApplication sharedApplication] terminate:self];
            break;
    }
}

- (void) keyDown:(NSEvent *)theEvent
{
    NSString* characters;
    unsigned int characterIndex, characterCount;
    
    characters = [theEvent charactersIgnoringModifiers];
    characterCount = [characters length];
    
    for (characterIndex = 0; characterIndex < characterCount; characterIndex++)
    {
        [self onKey:[characters characterAtIndex:characterIndex] downEvent:YES];
    }
}

@end

int main(int argc, const char* *argv)
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    NSApplication *NSApp = [NSApplication sharedApplication];
    NSRect frame = NSMakeRect( 0, 0, 1024, 600 );
    
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, 1024, 600);
    
    View* view = [[View alloc] initWithFrame:viewBounds];
    
    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width, viewBounds.size.height);
    
    NSWindow *window = [[NSWindow alloc]
                        initWithContentRect:centered
                        styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
                        backing:NSBackingStoreBuffered
                        defer:NO];
    
    [window setContentView:view];
    [window setDelegate:view];
    [view release];
    
    [NSApp run];
    
    [pool release];
    return EXIT_SUCCESS;
}

#endif

#endif