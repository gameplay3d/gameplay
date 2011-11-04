/*
 * LongboardGame.h
 */

#ifndef LONGBOARDGAME_H_
#define LONGBOARDGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

/**
 * Longboard game class.
 *
 * This class extends gameplay::Game, which hosts the game's message pump,
 * frame loop, window and other neccessary code.
 */
class LongboardGame : public Game
{
public:

    /**
     * Constructror.
     */
    LongboardGame();

    /**
     * Destructor.
     */
    virtual ~LongboardGame();

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

    void buildGround();

    void buildBoard();

    void buildWheels();

    void buildGradient();

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

    // Camera matrices
    Matrix _viewMatrix;
    Matrix _projectionMatrix;
    Matrix _viewProjectionMatrix;

    // Physics
    float _velocity;
    Vector3 _direction;
    Vector2 _groundUVTransform;
};

#endif
