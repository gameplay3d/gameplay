#ifndef GAMEPLAYMAINQNX_H_
#define GAMEPLAYMAINQNX_H_

#ifdef __QNX__

#include "gameplay.h"

using namespace gameplay;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    Game* game = Game::getInstance();
    assert(game != NULL);
    Platform* platform = Platform::create(game);
    return platform->enterMessagePump();
}

#endif

#endif
