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
     * Constructror.
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

    
    void gestureSwipeEvent(int x, int y, int direction);
    void gesturePinchEvent(int x, int y, float scale);
    void gestureRotateEvent(int x, int y, float angle);
    void gestureTapEvent(int x, int y);
    void gestureTapDoubleEvent(int x, int y);
};


#endif
