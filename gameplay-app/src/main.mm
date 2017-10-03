#import "AppDelegate.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <gameplay.h>

int main(int argc, char* argv[])
{
    gameplay::Platform* platform = gameplay::getPlatform();

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSApplication* application = [NSApplication sharedApplication];
    AppDelegate* applicationDelegate = [[[AppDelegate alloc] init] autorelease];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    [application setDelegate:applicationDelegate];
    [pool drain];
    
    return 0;
}
