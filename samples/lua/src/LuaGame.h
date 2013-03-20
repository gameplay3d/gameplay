#ifndef LUAGAME_H_
#define LUAGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 *
 * This game has all the implementation code done in lua.
 */
class LuaGame: public Game
{
public:

    /**
     * Constructor.
     */
    LuaGame();

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);
};


#endif
