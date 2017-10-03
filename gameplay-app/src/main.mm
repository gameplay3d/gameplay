#import <Cocoa/Cocoa.h>

#include "gameplay.h"

int main(int argc, char* argv[])
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    gameplay::Platform* platform = gameplay::getPlatform();
    platform->startup(argc, argv);
    int result = platform->enterMessagePump();
    [pool release];
    return result;
}
