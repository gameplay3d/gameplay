#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
#include "Gamepad.h"
using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class CharacterGame: public Game, public AnimationClip::Listener
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

    /**
     * @see AnimationClip::Listener::animationEvent
     */
    void animationEvent(AnimationClip* clip, EventType type);

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

private:

    bool initializeScene(Node* node);
    void initializeMaterial(Scene* scene, Node* node, Material* material);
    void initializeCharacter();
    void initializeGamepad();
    void drawSplash(void* param);
    bool drawScene(Node* node, bool transparent);
    void play(const char* id, bool repeat, float speed = 1.0f);
    void jump();
    void adjustCamera(float elapsedTime);
    bool isOnFloor() const;

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Node* _characterMeshNode;
    Node* _characterShadowNode;
    Animation* _animation;
    AnimationClip* _currentClip;
    AnimationClip* _jumpClip;
    int _rotateX;
    Gamepad* _gamepad;
    MaterialParameter* _materialParameterAlpha;
    unsigned int _keyFlags;
    int _drawDebug;
    bool _buttonReleased;
};

#endif
