#import <Cocoa/Cocoa.h>
#include "gameplay.h"

int main(int argc, char* argv[])
{
    gameplay::Platform* platform = gameplay::getPlatform();
    platform->startup(argc, argv);
    return platform->enterMessagePump();
}
