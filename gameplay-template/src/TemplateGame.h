#ifndef TEMPLATEGAME_H_
#define TEMPLATEGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class TemplateGame: public Game
{
public:

    /**
     * Constructror.
     */
    TemplateGame();

    /**
     * Touch event handler.
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

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
    void update(long elapsedTime);

    /**
     * @see Game::render
     */
    void render(long elapsedTime);

private:

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node, void* cookie);

    Scene* _scene;
};

#endif
