#ifndef GP_NO_PLATFORM
#ifdef __ANDROID__

#include <android_native_app_glue.h>
#include "gameplay.h"

using namespace gameplay;

extern struct android_app* __state;

/**
 * Main entry point.
 */
void android_main(struct android_app* state)
{
    // Android specific : Dummy function that needs to be called to 
    // ensure that the native activity works properly behind the scenes.
    app_dummy();
    
    __state = state;
    Game* game = Game::getInstance();
    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);
    platform->enterMessagePump();
    delete platform;
    
    // Android specific : the process needs to exit to trigger
    // cleanup of global and static resources (such as the game).
    exit(0);
}

#endif
#endif
