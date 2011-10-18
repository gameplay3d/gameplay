/*
 * LongboardGame.h
 */

#ifndef LONGBOARDGAME_H_
#define LONGBOARDGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

// Maximum velocity, in meters/sec
#define VELOCITY_MAX        2.0f
#define VELOCITY_MAX_MS        (VELOCITY_MAX / 1000.0f)

// Minimum velocity, in meters/sec
#define VELOCITY_MIN        0.2f
#define VELOCITY_MIN_MS        (VELOCITY_MIN / 1000.0f)

// Accelerometer pitch control
#define PITCH_MIN            20.0f
#define PITCH_MAX            70.0f
#define PITCH_RANGE            (PITCH_MAX - PITCH_MIN)

// Maximum turn rate (rate of change in direction at full velocity), in degrees/sec
#define TURNRATE_MAX        75.0f
#define TURNRATE_MAX_MS        (TURNRATE_MAX / 1000.0f)
#define ROLL_MAX            40.0f

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
