#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
#include "Gamepad.h"
using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class CharacterGame: public Game, public PhysicsCollisionObject::CollisionListener, public AnimationClip::Listener
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

    /**
     * @see PhysicsCollisionObject::CollisionListener::collisionEvent
     */
    void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
                        const PhysicsCollisionObject::CollisionPair& collisionPair,
                        const Vector3& contactPointA, const Vector3& contactPointB);

    void animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type);

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
    
    static const unsigned int JOYSTICK = 0;
    static const unsigned int BUTTON_1 = 0;

    /**
     * Draws the default "gameplay powered" splash screen.
     */
    void drawSplash(void* param);
    
    void initMaterial(Scene* scene, Node* node, Material* material);
    bool initScene(Node* node, void* cookie);
    bool drawScene(Node* node, void* cookie);
    void loadAnimationClips(Node* node);
    void adjustCamera(long elapsedTime);

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Node* _characterMeshNode;
    Animation* _animation;
    unsigned int _animationState;
    int _rotateX;
    Gamepad* _gamepad;
    MaterialParameter* _materialParameterAlpha;
};

#endif
