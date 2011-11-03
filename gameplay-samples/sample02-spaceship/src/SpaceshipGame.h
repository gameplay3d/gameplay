/*
 * SpaceshipGame.h
 */

#ifndef SPACESHIPGAME_H_
#define SPACESHIPGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

/**
 * Spaceship game.
 */
class SpaceshipGame : public Game
{
public:

    /**
     * Constructor.
     */
    SpaceshipGame();

    /**
     * Destructor.
     */
    virtual ~SpaceshipGame();

    /**
     * @see Game::touch
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
     * Initializes the spaceship.
     */
    void initializeSpaceship();

    /**
     * Initializes the environment including ground, roof and background.
     */
    void initializeEnvironment();

    /**
     * Initializes the material with common parameter bindings.
     *
     * @param lighting true if the material should receive lighting parameters.
     * @param specular true if the material should receive specular lighting parameters.
     */
    void initializeMaterial(Material* material, bool lighting, bool specular);

    /**
     * Handles foe collisions with the ground and roof.
     */
    void handleCollisions(float t);

    /**
     * Updates the camera based on the ships position.
     */
    void updateCamera();

    /**
     * Resets the games
     */
    void resetGame();

    /**
     * Visits nodes in a scene.
     */
    void visitNode(Node* node, long cookie);

    // Scene variables
    Scene* _scene;
    Node* _cameraNode;
    Node* _shipGroupNode;
    Node* _shipNode;
    Node* _propulsionNode;
    Node* _glowNode;
    RenderState::StateBlock* _stateBlock;
    Vector3 _initialShipPos;
    Quaternion _initialShipRot;
    Vector3 _initialCameraPos;

    // Font for text rendering
    Font* _font;

    // Ship physics variables
    Vector2 _velocity;
    Vector2 _acceleration;
    Vector2 _force;
    float _throttle;
    float _shipTilt;

    // State variables
    bool _finished;
    long _finishedTime;
    bool _pushing;
    Vector2 _pushPoint;
    
    // Game time in seconds
    float _time;

    // Frequently updated material parameters
    MaterialParameter* _glowDiffuseParameter;
    MaterialParameter* _shipSpecularParameter;

    // Space ship sound
    AudioSource* _spaceshipSound;

};

#endif
