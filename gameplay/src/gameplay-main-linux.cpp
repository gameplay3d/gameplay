#ifdef __linux__

#include "gameplay.h"

using namespace gameplay;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    Game* game = Game::getInstance();
    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);
    int result = platform->enterMessagePump();
    delete platform;
    return result;
}

#endif
