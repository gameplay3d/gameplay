#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class CharacterGame: public Game, public AnimationClip::Listener, public PhysicsCollisionObject::CollisionListener
{
public:
    
    /**
     * Constructor.
     */
    CharacterGame();

    /**
     * @see Game::keyEvent
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Game::mouseEvent
     */
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * @see Game::gamepadEvent
     */
    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    /**
     * @see AnimationClip::Listener::animationEvent
     */
    void animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type);

    /**
     * @see PhysicsCollisionObject::CollisionListener::collisionEvent
     */
    void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
                                    const PhysicsCollisionObject::CollisionPair& collisionPair,
                                    const Vector3& contactPointA = Vector3::zero(),
                                    const Vector3& contactPointB = Vector3::zero());
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
    void drawSplash(void* param);
    bool drawScene(Node* node, bool transparent);
    void play(const char* id, bool repeat, float speed = 1.0f);
    void jump();
    void kick();
    void adjustCamera(float elapsedTime);
    bool isOnFloor() const;
    void clone();
    void grabBall();
    void releaseBall();

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Node* _characterNode;
    Node* _characterMeshNode;
    Node* _characterShadowNode;
    Node* _basketballNode;
    float _floorLevel;
    Animation* _animation;
    AnimationClip* _currentClip;
    AnimationClip* _jumpClip;
    AnimationClip* _kickClip;
    int _rotateX;
    MaterialParameter* _materialParameterAlpha;
    unsigned int _keyFlags;
    int _drawDebug;
    bool _wireframe;
    Vector3 _oldBallPosition;
    bool _hasBall;
    bool _applyKick;
    bool _kicking;
    float _kickDelay;
    bool* _buttonPressed;
    Vector2 _currentDirection;
    Gamepad* _gamepad;

};

#endif
