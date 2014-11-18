#ifdef __APPLE__

#import <Foundation/Foundation.h>
#include "gameplay.h"

using namespace gameplay;

extern int __argc;
extern char** __argv;

/**
 * Main entry point.
 */
int main(int argc, char *argv[])
{
    __argc = argc;
    __argv = argv;

    int result;

    @autoreleasepool {
        Game* game = Game::getInstance();
        Platform* platform = Platform::create(game);
        GP_ASSERT(platform);
        result = platform->enterMessagePump();
        delete platform;
    }

    return result;
}

#endif
