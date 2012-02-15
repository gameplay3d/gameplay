#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class CharacterGame: public Game
{
public:

    /**
     * Constructor.
     */
    CharacterGame();

    /**
     * Destructor.
     */
    virtual ~CharacterGame();

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Game::keyEvent
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

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

    bool initScene(Node* node, void* cookie);
    bool drawScene(Node* node, void* cookie);

    void loadAnimationClips();

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Animation* _animation;
    unsigned int _animationState;
    int _rotateX;
};

#endif
