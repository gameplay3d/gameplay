/*
 * SandboxGame.h
 */

#ifndef SANDBOXGAME_H_
#define SANDBOXGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

/**
 * This is a simple sample that demonstrates basic usage of the physics API.
 */
class SandboxGame: public Game
{
public:
    /**
     * Destructor.
     */
    virtual ~SandboxGame();

    /**
     * Touch event handler.
     */
    void touch(int x, int y, int touchEvent);

    /**
     * Key press event handler.
     */
    void keyPress(int key, int keyEvent);

protected:
    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::update
     */
    void update(long elapsedTime);

    /**
     * @see Game::render
     */
    void render(long elapsedTime);

    /**
     * @see Game::finalize
     */
    void finalize();

private:
    void visitNode(Node* node, long cookie);

    Scene* _scene;
    Font* _font;
    Node* _volleyball;
};

#endif
