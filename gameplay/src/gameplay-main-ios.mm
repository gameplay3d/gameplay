#ifdef __APPLE__

#include "gameplay.h"

using namespace gameplay;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{    
    Game* game = Game::getInstance();
    GP_ASSERT(game != NULL);
    Platform* platform = Platform::create(game);
    int result = platform->enterMessagePump();
	delete platform;
    return result;
}

#endif
