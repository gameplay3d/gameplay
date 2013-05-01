#ifndef LONGBOARDGAME_H_
#define LONGBOARDGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Longboard game class.
 *
 * This class extends gameplay::Game, which hosts the game's message pump,
 * frame loop, window and other necessary code.
 */
class LongboardGame : public Game
{
public:

    /**
     * Constructor.
     */
    LongboardGame();

    /**
     * Destructor.
     */
    virtual ~LongboardGame();

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
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Builds the ground model, which is simply a texture quad.
     */
    void buildGround();

    /**
     * Builds the longboard model.
     */
    void buildBoard();

    /**
     * Builds the longboard wheels model.
     */
    void buildWheels();

    /**
     * Builds a textured gradient model that we use as a cheap/fake
     * static screen-space lighting effect to give a bit more realism.
     */
    void buildGradient();

    /**
     * Draws the default "gameplay powered" splash screen.
     */
    void drawSplash(void* param);

    // Ground
    Model* _ground;
    Matrix _groundWorldMatrix;
    Matrix _groundWorldViewProjectionMatrix;

    // Board
    Model* _board;
    Matrix _boardWorldMatrix;
    Matrix _boardWorldViewProjectionMatrix;

    // Wheels
    Model* _wheels;
    Matrix _wheelsWorldMatrix;
    Matrix _wheelsWorldViewProjectionMatrix;
    AudioSource* _wheelsSound;

    // Gradient
    Model* _gradient;

    // Render state
    RenderState::StateBlock* _stateBlock;

    // Camera matrices
    Matrix _viewMatrix;
    Matrix _projectionMatrix;
    Matrix _viewProjectionMatrix;

    // Physics and keyboard controls
    unsigned int _keyFlags;
    float _simulatedPitch, _simulatedRoll;
    float _velocity;
    Vector3 _direction;
    Vector2 _groundUVTransform;
};

#endif
