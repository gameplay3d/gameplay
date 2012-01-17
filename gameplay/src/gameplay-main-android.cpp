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
    assert(game != NULL);
    Platform* platform = Platform::create(game);
    assert(platform != NULL);
    platform->enterMessagePump();
    Game::getInstance()->exit();
    delete platform;
    
    // Android specific : the process needs to exit to 
    // to trigger cleanup of global resources (such as game).
    exit(0);
}

#endif
