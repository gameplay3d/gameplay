#include "gameplay.h"

#ifdef __APPLE__

#import "Fouundation/Foundation.h""

int main(int argc, char** argv)
{
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    gameplay::Platform* platform = gameplay::getPlatform();
    platform->startup(argc, argv);
    delete platform;
    [p release];
    return result;
}

#endif