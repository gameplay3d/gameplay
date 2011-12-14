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
    void touch(int x, int y, int touchEvent);

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
