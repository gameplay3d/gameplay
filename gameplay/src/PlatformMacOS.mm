#include "Base.h"
#ifdef GP_PLATFORM_MACOS
#include "Platform.h"
#include "Input.h"

#include <bx/bx.h>
#include <bx/mutex.h>
#include <bx/handlealloc.h>
#include <bx/os.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include <bgfx/platform.h>

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject<NSApplicationDelegate>
{
	bool terminated;
}

+ (AppDelegate *)sharedDelegate;
- (id)init;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
- (bool)applicationHasTerminated;

@end

@interface Window : NSObject<NSWindowDelegate>
{
	uint32_t windowCount;
}

+ (Window*)sharedDelegate;
- (id)init;
- (void)windowCreated:(NSWindow*)window;
- (void)windowWillClose:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)window;
- (void)windowDidResize:(NSNotification*)notification;
- (void)windowDidBecomeKey:(NSNotification *)notification;
- (void)windowDidResignKey:(NSNotification *)notification;

@end

namespace gameplay
{

void setNSWindow(void* window, void* nsgl = nullptr)
{
	bgfx::PlatformData pd;
	pd.ndt = nullptr;
	pd.nwh = window;
	pd.context = nsgl;
	pd.backBuffer = nullptr;
	pd.backBufferDS = nullptr;
	bgfx::setPlatformData(pd);
}

static gameplay::Platform::WindowHandle __defaultWindow = { 0 };
static uint8_t __translateKey[256];


struct MainThreadEntry
{
	int argc;
	const char* const* argv;

	static int32_t threadFunc(bx::Thread*, void* userData)
	{
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		if (mainBundle != nil)
		{
			CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
			if (resourcesURL != nil)
			{
				char path[PATH_MAX];
				if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX) )
				{
					chdir(path);
				}
				CFRelease(resourcesURL);
			}
		}
		MainThreadEntry* self = (MainThreadEntry*)userData;
		return main(self->argc, self->argv);
	}
};


struct Context
{
	Context() :
		mx(0),
		my(0),
		scroll(0),
		scrollf(0.0f),
		style(0),
		fullscreen(false),
		exit(false)
	{
		__translateKey[27]             = gameplay::Input::KEY_ESC;
		__translateKey[uint8_t('\r')]  = gameplay::Input::KEY_RETURN;
		__translateKey[uint8_t('\t')]  = gameplay::Input::KEY_TAB;
		__translateKey[127]            = gameplay::Input::KEY_BACKSPACE;
		__translateKey[uint8_t(' ')]   = gameplay::Input::KEY_SPACE;
		__translateKey[uint8_t('+')]   =
		__translateKey[uint8_t('=')]   = gameplay::Input::KEY_PLUS;
		__translateKey[uint8_t('_')]   =
		__translateKey[uint8_t('-')]   = gameplay::Input::KEY_MINUS;
		__translateKey[uint8_t('~')]   =
		__translateKey[uint8_t('`')]   = gameplay::Input::KEY_TILDE;
		__translateKey[uint8_t(':')]   =
		__translateKey[uint8_t(';')]   = gameplay::Input::KEY_SEMICOLON;
		__translateKey[uint8_t('"')]   =
		__translateKey[uint8_t('\'')]  = gameplay::Input::KEY_QUOTE;
		__translateKey[uint8_t('{')]   =
		__translateKey[uint8_t('[')]   = gameplay::Input::KEY_LEFT_BRACKET;
		__translateKey[uint8_t('}')]   =
		__translateKey[uint8_t(']')]   = gameplay::Input::KEY_RIGHT_BRACKET;
		__translateKey[uint8_t('<')]   =
		__translateKey[uint8_t(',')]   = gameplay::Input::KEY_COMMA;
		__translateKey[uint8_t('>')]   =
		__translateKey[uint8_t('.')]   = gameplay::Input::KEY_PERIOD;
		__translateKey[uint8_t('?')]   =
		__translateKey[uint8_t('/')]   = gameplay::Input::KEY_SLASH;
		__translateKey[uint8_t('|')]   =
		__translateKey[uint8_t('\\')]  = gameplay::Input::KEY_BACKSLASH;
		__translateKey[uint8_t('0')]   = gameplay::Input::KEY_0;
		__translateKey[uint8_t('1')]   = gameplay::Input::KEY_1;
		__translateKey[uint8_t('2')]   = gameplay::Input::KEY_2;
		__translateKey[uint8_t('3')]   = gameplay::Input::KEY_3;
		__translateKey[uint8_t('4')]   = gameplay::Input::KEY_4;
		__translateKey[uint8_t('5')]   = gameplay::Input::KEY_5;
		__translateKey[uint8_t('6')]   = gameplay::Input::KEY_6;
		__translateKey[uint8_t('7')]   = gameplay::Input::KEY_7;
		__translateKey[uint8_t('8')]   = gameplay::Input::KEY_8;
		__translateKey[uint8_t('9')]   = gameplay::Input::KEY_9;

		for (char ch = 'a'; ch <= 'z'; ++ch)
		{
			__translateKey[uint8_t(ch)]       =
			__translateKey[uint8_t(ch - ' ')] = gameplay::Input::KEY_A + (ch - 'a');
		}
	}

	NSEvent* waitEvent()
	{
		return [NSApp
                nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantFuture] // Wait for event
			inMode:NSDefaultRunLoopMode
			dequeue:YES
			];
	}

	NSEvent* peekEvent()
	{
		return [NSApp
			nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantPast] // Do not wait for event
			inMode:NSDefaultRunLoopMode
			dequeue:YES
			];
	}

	void getMousePos(int* outX, int* outY)
	{
		gameplay::Platform::WindowHandle handle = { 0 };
		NSWindow* window = this->window[handle.idx];
		NSRect originalFrame = [window frame];
		NSPoint location = [window mouseLocationOutsideOfEventStream];
		NSRect adjustFrame = [window contentRectForFrameRect: originalFrame];

		// Clamp within the range of the window
		int x = location.x;
		int y = (int)adjustFrame.size.height - (int)location.y;
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x > (int)adjustFrame.size.width) x = (int)adjustFrame.size.width;
		if (y > (int)adjustFrame.size.height) y = (int)adjustFrame.size.height;
		*outX = x;
		*outY = y;
	}

	uint8_t translateModifiers(int flags)
	{
		uint8_t mask = 0;

		if (flags & NSEventModifierFlagShift)
			mask |= gameplay::Input::KEY_MODIFIER_LEFT_SHIFT | gameplay::Input::KEY_MODIFIER_RIGHT_SHIFT;

		if (flags & NSEventModifierFlagOption)
			mask |= gameplay::Input::KEY_MODIFIER_LEFT_ALT | gameplay::Input::KEY_MODIFIER_RIGHT_ALT;

		if (flags & NSEventModifierFlagControl)
			mask |= gameplay::Input::KEY_MODIFIER_LEFT_CTRL | gameplay::Input::KEY_MODIFIER_RIGHT_CTRL;

		if (flags & NSEventModifierFlagCommand)
			mask |= gameplay::Input::KEY_MODIFIER_LEFT_META | gameplay::Input::KEY_MODIFIER_RIGHT_META;

		return mask;
	}

	gameplay::Input::Key handleKeyEvent(NSEvent* event, uint8_t* specialKeys, uint8_t* pressedChar)
	{
		NSString* key = [event charactersIgnoringModifiers];
		unichar keyChar = 0;
		if ([key length] == 0)
		{
			return gameplay::Input::KEY_NONE;
		}

		keyChar = [key characterAtIndex:0];
		*pressedChar = (uint8_t)keyChar;

		int keyCode = keyChar;
		*specialKeys = translateModifiers(int([event modifierFlags]));

		// if this is a unhandled key just return None
		if (keyCode < 256)
		{
			return (gameplay::Input::Key)__translateKey[keyCode];
		}

		switch (keyCode)
		{
		case NSF1FunctionKey:
			return gameplay::Input::KEY_F1;
		case NSF2FunctionKey:
			return gameplay::Input::KEY_F2;
		case NSF3FunctionKey:
			return gameplay::Input::KEY_F3;
		case NSF4FunctionKey:
			return gameplay::Input::KEY_F4;
		case NSF5FunctionKey:
			return gameplay::Input::KEY_F5;
		case NSF6FunctionKey:
			return gameplay::Input::KEY_F6;
		case NSF7FunctionKey:
			return gameplay::Input::KEY_F7;
		case NSF8FunctionKey:
			return gameplay::Input::KEY_F8;
		case NSF9FunctionKey:
			return gameplay::Input::KEY_F9;
		case NSF10FunctionKey:
			return gameplay::Input::KEY_F10;
		case NSF11FunctionKey:
			return gameplay::Input::KEY_F11;
		case NSF12FunctionKey:
			return gameplay::Input::KEY_F12;
		case NSLeftArrowFunctionKey:
			return gameplay::Input::KEY_LEFT;
		case NSRightArrowFunctionKey:
			return gameplay::Input::KEY_RIGHT;
		case NSUpArrowFunctionKey:
			return gameplay::Input::KEY_UP;
		case NSDownArrowFunctionKey:
			return gameplay::Input::KEY_DOWN;
		case NSPageUpFunctionKey:
			return gameplay::Input::KEY_PAGE_UP;
		case NSPageDownFunctionKey:
			return gameplay::Input::KEY_PAGE_DOWN;
		case NSHomeFunctionKey:
			return gameplay::Input::KEY_HOME;
		case NSEndFunctionKey:
			return gameplay::Input::KEY_END;
		case NSPrintScreenFunctionKey: 
			return gameplay::Input::KEY_PRINT;
		}
		return gameplay::Input::KEY_NONE;
	}

	bool dispatchEvent(NSEvent* event)
	{
		if (event)
		{
			NSEventType eventType = [event type];

			switch (eventType)
			{
			case NSEventTypeMouseMoved:
			case NSEventTypeLeftMouseDragged:
			case NSEventTypeRightMouseDragged:
			case NSEventTypeOtherMouseDragged:
                {
				getMousePos(&mx, &my);
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll);
                }
				break;

			case NSEventTypeLeftMouseDown:
                {
				gameplay::Input::MouseButton mb = ([event modifierFlags] & NSEventModifierFlagCommand) ? gameplay::Input::MOUSE_BUTTON_MIDDLE : gameplay::Input::MOUSE_BUTTON_LEFT;
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, mb, true);
                }
				break;

			case NSEventTypeLeftMouseUp:
                {
                    eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_LEFT, false);
                    eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_MIDDLE, false);
                }
                break;

			case NSEventTypeRightMouseDown:
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_RIGHT, true);
				break;

			case NSEventTypeRightMouseUp:
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_RIGHT, false);
				break;

			case NSEventTypeOtherMouseDown:
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_MIDDLE, true);
				break;

			case NSEventTypeOtherMouseUp:
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_MIDDLE, false);
				break;

			case NSEventTypeScrollWheel:
				scrollf += [event deltaY];
				scroll = (int32_t)scrollf;
				eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll);
				break;

			case NSEventTypeKeyDown:
				{
					uint8_t modifiers = 0;
					uint8_t pressedChar[4];
					gameplay::Input::Key key = handleKeyEvent(event, &modifiers, &pressedChar[0]);

					// Returning false means that we take care of the key (instead of the default behavior)
					if (key != gameplay::Input::KEY_NONE)
					{
						if (key == gameplay::Input::KEY_Q && (modifiers & gameplay::Input::KEY_MODIFIER_RIGHT_META) )
						{
							eventQueue.postExitEvent();
						}
						else
						{
							enum { ShiftMask = gameplay::Input::KEY_MODIFIER_LEFT_SHIFT|gameplay::Input::KEY_MODIFIER_RIGHT_SHIFT};
							eventQueue.postCharEvent(__defaultWindow, 1, pressedChar);
							eventQueue.postKeyEvent(__defaultWindow, key, modifiers, true);
							return false;
						}
					}
				}
				break;

			case NSEventTypeKeyUp:
				{
					uint8_t modifiers  = 0;
					uint8_t pressedChar[4];
					gameplay::Input::Key key = handleKeyEvent(event, &modifiers, &pressedChar[0]);

					BX_UNUSED(pressedChar);

					if (key != gameplay::Input::KEY_NONE)
					{
						eventQueue.postKeyEvent(__defaultWindow, key, modifiers, false);
						return false;
					}
				}
				break;

			default:
				break;
			}

			[NSApp sendEvent:event];
			[NSApp updateWindows];

			return true;
		}

		return false;
	}

	void windowDidResize()
	{
        gameplay::Platform::WindowHandle handle = { 0 };
		NSWindow* window = this->window[handle.idx];
		NSRect originalFrame = [window frame];
		NSRect rect = [window contentRectForFrameRect: originalFrame];
		uint32_t width  = uint32_t(rect.size.width);
		uint32_t height = uint32_t(rect.size.height);
		eventQueue.postSizeEvent(handle, width, height);

		// Make sure mouse button state is 'up' after resize.
		eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_LEFT,  false);
		eventQueue.postMouseEvent(__defaultWindow, mx, my, scroll, gameplay::Input::MOUSE_BUTTON_RIGHT, false);
	}

	void windowDidBecomeKey()
	{
        eventQueue.postSuspendEvent(__defaultWindow, gameplay::Platform::SUSPEND_MODE_WILL_RESUME);
        eventQueue.postSuspendEvent(__defaultWindow, gameplay::Platform::SUSPEND_MODE_DID_RESUME);
	}

	void windowDidResignKey()
	{
        eventQueue.postSuspendEvent(__defaultWindow, gameplay::Platform::SUSPEND_MODE_WILL_RESUME);
		eventQueue.postSuspendEvent(__defaultWindow, gameplay::Platform::SUSPEND_MODE_DID_RESUME);
	}

	int32_t run(int argc, const char* const* argv)
	{
		[NSApplication sharedApplication];

		id dg = [AppDelegate sharedDelegate];
		[NSApp setDelegate:dg];
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		[NSApp activateIgnoringOtherApps:YES];
		[NSApp finishLaunching];

		[[NSNotificationCenter defaultCenter]
			postNotificationName:NSApplicationWillFinishLaunchingNotification
			object:NSApp];

		[[NSNotificationCenter defaultCenter]
			postNotificationName:NSApplicationDidFinishLaunchingNotification
			object:NSApp];

		id quitMenuItem = [NSMenuItem new];
		[quitMenuItem
			initWithTitle:@"Quit"
			action:@selector(terminate:)
			keyEquivalent:@"q"];

		id appMenu = [NSMenu new];
		[appMenu addItem:quitMenuItem];

		id appMenuItem = [NSMenuItem new];
		[appMenuItem setSubmenu:appMenu];

		id menubar = [[NSMenu new] autorelease];
		[menubar addItem:appMenuItem];
		[NSApp setMainMenu:menubar];

		style = 0
				| NSWindowStyleMaskTitled
				| NSWindowStyleMaskClosable
				| NSWindowStyleMaskMiniaturizable
				| NSWindowStyleMaskResizable;

		NSRect screenRect = [[NSScreen mainScreen] frame];
		const float centerX = (screenRect.size.width  - (float)GP_GRAPHICS_WIDTH )*0.5f;
		const float centerY = (screenRect.size.height - (float)GP_GRAPHICS_HEIGHT)*0.5f;

		windowAlloc.alloc();
		NSRect rect = NSMakeRect(centerX, centerY, GP_GRAPHICS_WIDTH, GP_GRAPHICS_HEIGHT);
		NSWindow* window = [[NSWindow alloc]
			initWithContentRect:rect
			styleMask:style
			backing:NSBackingStoreBuffered defer:NO
		];
		NSString* appName = [[NSProcessInfo processInfo] processName];
		[window setTitle:appName];
		[window makeKeyAndOrderFront:window];
		[window setAcceptsMouseMovedEvents:YES];
		[window setBackgroundColor:[NSColor blackColor]];
		[[Window sharedDelegate] windowCreated:window];

		this->window[0] = window;
		windowFrame = [window frame];

		setNSWindow(window);

		MainThreadEntry mte;
		mte.argc = argc;
		mte.argv = argv;

		bx::Thread thread;
		thread.init(mte.threadFunc, &mte);

		gameplay::Platform::WindowHandle handle = { 0 };
		NSRect contentRect = [window contentRectForFrameRect: windowFrame];
		uint32_t width = uint32_t(contentRect.size.width);
		uint32_t height = uint32_t(contentRect.size.height);
		eventQueue.postSizeEvent(handle, width, height);

		while (!(exit = [dg applicationHasTerminated]))
		{
			@autoreleasepool
			{
				bgfx::renderFrame();
			}

			while (dispatchEvent(peekEvent() ) )
			{
			}
		}

		eventQueue.postExitEvent();

		while (bgfx::RenderFrame::NoContext != bgfx::renderFrame() ) {};
		thread.shutdown();

		return 0;
	}

	bool isWindowValid(gameplay::Platform::WindowHandle handle)
	{
		return windowAlloc.isValid(handle.idx);
	}

    gameplay::Platform::EventQueue eventQueue;
	bx::HandleAllocT<GP_WINDOWS_MAX> windowAlloc;
	NSWindow* window[GP_WINDOWS_MAX];
	NSRect windowFrame;
	int32_t mx;
	int32_t my;
	int32_t scroll;
	float scrollf;
	int32_t style;
	bool fullscreen;
	bool exit;
};

static Context __ctx;


extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

const gameplay::Platform::Event* poll()
{
	return __ctx.eventQueue.poll();
}

const gameplay::Platform::Event* poll(gameplay::Platform::WindowHandle handle)
{
	return __ctx.eventQueue.poll(handle);
}

void release(const gameplay::Platform::Event* evt)
{
	__ctx.eventQueue.release(evt);
}

gameplay::Platform::WindowHandle createWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags, const char* title)
{
	BX_UNUSED(x, y, width, height, flags, title);
	gameplay::Platform::WindowHandle handle = { UINT16_MAX };
	return handle;
}

void destroyWindow(gameplay::Platform::WindowHandle handle)
{
	if (__ctx.isWindowValid(handle))
	{
		dispatch_async(dispatch_get_main_queue()
		, ^{
			[__ctx.window[handle.idx] performClose: nil];
		});
	}
}

void setWindowPos(gameplay::Platform::WindowHandle handle, int32_t x, int32_t y)
{
	if (__ctx.isWindowValid(handle))
	{
		NSWindow* window = __ctx.window[handle.idx];
		NSScreen* screen = [window screen];
		NSRect screenRect = [screen frame];
		CGFloat menuBarHeight = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];

		NSPoint position = { float(x), screenRect.size.height - menuBarHeight - float(y) };

		dispatch_async(dispatch_get_main_queue()
		, ^{
			[window setFrameTopLeftPoint: position];
		});
	}
}

void setWindowSize(gameplay::Platform::WindowHandle handle, uint32_t width, uint32_t height)
{
	if (__ctx.isWindowValid(handle))
	{
		NSSize size = { float(width), float(height) };
		dispatch_async(dispatch_get_main_queue()
		, ^{
			[__ctx.window[handle.idx] setContentSize: size];
		});
	}
}

void setWindowTitle(gameplay::Platform::WindowHandle handle, const char* title)
{
	if (__ctx.isWindowValid(handle))
	{
		NSString* titleStr = [[NSString alloc] initWithCString:title encoding:1];
		dispatch_async(dispatch_get_main_queue()
		, ^{
			[__ctx.window[handle.idx] setTitle: titleStr];
		});
		[titleStr release];
	}
}

void setWindowFlags(gameplay::Platform::WindowHandle handle, uint32_t flags, bool enabled)
{
	BX_UNUSED(handle, flags, enabled);
}

void toggleFullscreen(gameplay::Platform::WindowHandle handle)
{
	if (__ctx.isWindowValid(handle))
	{
		NSWindow* window = __ctx.window[handle.idx];
		NSScreen* screen = [window screen];
		NSRect screenRect = [screen frame];

		if (!__ctx.fullscreen)
		{
			__ctx.style &= ~NSWindowStyleMaskTitled;
			dispatch_async(dispatch_get_main_queue()
			, ^{
				[NSMenu setMenuBarVisible: false];
				[window setStyleMask: __ctx.style];
				[window setFrame:screenRect display:YES];
			});

			__ctx.fullscreen = true;
		}
		else
		{
			__ctx.style |= NSWindowStyleMaskTitled;
			dispatch_async(dispatch_get_main_queue()
			, ^{
				[NSMenu setMenuBarVisible: true];
				[window setStyleMask: __ctx.style];
				[window setFrame:__ctx.windowFrame display:YES];
			});

			__ctx.fullscreen = false;
		}
	}
}

void setMouseLock(gameplay::Platform::WindowHandle handle, bool lock)
{
	BX_UNUSED(handle, lock);
}

}


@implementation AppDelegate

+ (AppDelegate*)sharedDelegate
{
	static id delegate = [AppDelegate new];
	return delegate;
}

- (id)init
{
	self = [super init];

	if (self == nil)
	{
		return nil;
	}

	self->terminated = false;
	return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
	BX_UNUSED(sender);
	self->terminated = true;
	return NSTerminateCancel;
}

- (bool)applicationHasTerminated
{
	return self->terminated;
}

@end


@implementation Window

+ (Window*)sharedDelegate
{
	static id windowDelegate = [Window new];
	return windowDelegate;
}

- (id)init
{
	self = [super init];
	if (self == nil)
	{
		return nil;
	}

	self->windowCount = 0;
	return self;
}

- (void)windowCreated:(NSWindow*)window
{
	GP_ASSERT(window);

	[window setDelegate:self];

	assert(self->windowCount < ~0u);
	self->windowCount += 1;
}

- (void)windowWillClose:(NSNotification*)notification
{
	BX_UNUSED(notification);
}

- (BOOL)windowShouldClose:(NSWindow*)window
{
	GP_ASSERT(window);

	[window setDelegate:nil];

	GP_ASSERT(self->windowCount);
	self->windowCount -= 1;

	if (self->windowCount == 0)
	{
		[NSApp terminate:self];
		return false;
	}

	return true;
}

- (void)windowDidResize:(NSNotification*)notification
{
	BX_UNUSED(notification);
	using namespace gameplay;
	__ctx.windowDidResize();
}

- (void)windowDidBecomeKey:(NSNotification*)notification
{
    BX_UNUSED(notification);
    using namespace gameplay;
    __ctx.windowDidBecomeKey();
}

- (void)windowDidResignKey:(NSNotification*)notification
{
    BX_UNUSED(notification);
    using namespace gameplay;
    __ctx.windowDidResignKey();
}

@end


int main(int argc, const char* const* argv)
{
	using namespace gameplay;
	return __ctx.run(argc, argv);
}

#endif
