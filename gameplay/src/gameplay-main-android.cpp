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
    // Make sure glue isn't stripped.
    app_dummy();
    
    __state = state;
    Game* game = Game::getInstance();
    assert(game != NULL);
    Platform* platform = Platform::create(game);
    assert(platform != NULL);
    platform->enterMessagePump();
    Game::getInstance()->exit();
    delete platform;
    
    // We need to exit the process to cleanup global resources.
    exit(0);
}

#endif
