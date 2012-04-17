#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
#include "Gamepad.h"
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

    /**
     * Draws the default "gameplay powered" splash screen.
     */
    void drawSplash(void* param);

    bool initializeScene(Node* node);
    void initializeMaterial(Scene* scene, Node* node, Material* material);
    void initializeCharacter();
    void initializeGamepad();
    bool drawScene(Node* node, bool transparent);
    void loadAnimationClips(Node* node);
    void adjustCamera(long elapsedTime);
    void play(const char* id, bool repeat);
    bool isJumping() const;

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Node* _characterMeshNode;
    Animation* _animation;
    AnimationClip* _currentClip;
    int _rotateX;
    Gamepad* _gamepad;
    MaterialParameter* _materialParameterAlpha;
    int _drawDebug;
    unsigned int _inputFlags;
};

#endif
