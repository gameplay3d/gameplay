#ifndef GAMEPLAYMAINQNX__H
#define GAMEPLAYMAINQNX__H

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
