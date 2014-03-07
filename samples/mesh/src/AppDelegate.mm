//
// Example NSApplicationDelegate implementation.
//
// Andy Duplain <andy@trojanfoe.com>, March 2014.
//

#import <sys/types.h>
#import <sys/sysctl.h>
#import "AppDelegate.h"
#import "Base.h"
#import "Game.h"

using namespace gameplay;

@implementation AppDelegate

#pragma mark - NSApplicationDelegate methods

- (void)applicationWillFinishLaunching:(NSNotification*)notification
{
    //
    // From: http://www.cocoawithlove.com/2010/09/minimalist-cocoa-programming.html
    //
    NSMenu* menubar = [[NSMenu new] autorelease];
    NSMenuItem* appMenuItem = [[NSMenuItem new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    NSMenu* appMenu = [[NSMenu new] autorelease];
    NSString* appName = [[NSProcessInfo processInfo] processName];
    NSString* aboutTitle = [@"About " stringByAppendingString:appName];
    NSMenuItem* aboutMenuItem = [[[NSMenuItem alloc] initWithTitle:aboutTitle
                                                  action:@selector(orderFrontStandardAboutPanel:)
                                                     keyEquivalent:@""] autorelease];
    [appMenu addItem:aboutMenuItem];

    NSString* quitTitle = [@"Quit " stringByAppendingString:appName];
    NSMenuItem* quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                                           action:@selector(terminate:)
                                                    keyEquivalent:@"q"] autorelease];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    Game* game = Game::getInstance();
    game->exit();
}

@end

