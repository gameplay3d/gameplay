/*
 * LongboardGame.cpp
 */

#include "LongboardGame.h"

// Declare our game instance
LongboardGame game;

// Pick some arbitrarily large world size for our playground
#define WORLD_SIZE            20.0f

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
#define TURN_RATE_MAX        75.0f
#define TURN_RATE_MAX_MS        (TURN_RATE_MAX / 1000.0f)
#define ROLL_MAX            40.0f

LongboardGame::LongboardGame() :
    _ground(NULL), _board(NULL), _wheels(NULL), _gradient(NULL), _velocity(VELOCITY_MIN_MS)
{
}

LongboardGame::~LongboardGame()
{
}

/**
 * Initializes the game.
 */
void LongboardGame::initialize()
{
    // Initialize the render state
    glClearColor(1, 1, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Calculate initial matrices
    Matrix::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 0.25f, 100.0f, &_projectionMatrix);
    Matrix::createLookAt(0, 1.75f, 1.35f, 0, 0, -0.15f, 0, 0.20f, -0.80f, &_viewMatrix);
    Matrix::multiply(_projectionMatrix, _viewMatrix, &_viewProjectionMatrix);

    // Build game entities
    buildGround();
    buildBoard();
    buildWheels();
    buildGradient();

    // Set initial board physics
    _direction.set(0, 0, -1);
    _velocity = VELOCITY_MIN_MS;
}

/**
 * Called when the game is shutting down to cleanup.
 */
void LongboardGame::finalize()
{
    SAFE_RELEASE(_ground);
    SAFE_RELEASE(_board);
    SAFE_RELEASE(_wheels);
    SAFE_RELEASE(_gradient);
    SAFE_RELEASE(_wheelsSound);
}

/**
 * Builds the ground model, which is simply a texture quad.
 */
void LongboardGame::buildGround()
{
    // Create a large quad for the ground
    Mesh* groundMesh = Mesh::createQuad(Vector3(-WORLD_SIZE, 0, -WORLD_SIZE),
                                        Vector3(-WORLD_SIZE, 0, WORLD_SIZE),
                                        Vector3(WORLD_SIZE, 0, -WORLD_SIZE),
                                        Vector3(WORLD_SIZE, 0, WORLD_SIZE));

    // Create the ground model
    _ground = Model::create(groundMesh);

    // Create the ground material
    Material* groundMaterial = _ground->setMaterial("res/shaders/textured.vsh", "res/shaders/textured.fsh");

    // Bind ground material parameters
    Texture::Sampler* groundSampler = groundMaterial->getParameter("texture")->setValue("res/textures/tileable_asphalt.png", true);
    groundSampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
    groundMaterial->getParameter("worldViewProjection")->setValue(&_groundWorldViewProjectionMatrix);
    groundMaterial->getParameter("textureRepeat")->setValue(Vector2(WORLD_SIZE/2, WORLD_SIZE/2));
    groundMaterial->getParameter("textureTransform")->setValue(&_groundUVTransform);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(groundMesh);
}

/**
 * Builds the longboard model.
 */
void LongboardGame::buildBoard()
{
    // Create longboard mesh
    Mesh* boardMesh = Mesh::createQuad(Vector3(-0.5f, 0.1f, -1.0f),
                                        Vector3(-0.5f, 0.1f, 1.0f),
                                        Vector3(0.5f, 0.1f, -1.0f),
                                        Vector3(0.5f, 0.1f, 1.0f) );

    // Create the board model
    _board = Model::create(boardMesh);

    // Create the board material
    Material* boardMaterial = _board->setMaterial("res/shaders/textured.vsh", "res/shaders/textured.fsh");

    // Bind board material parameters
    Texture::Sampler* boardSampler = boardMaterial->getParameter("texture")->setValue("res/textures/longboard.png", true);
    boardSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    boardMaterial->getParameter("worldViewProjection")->setValue(&_boardWorldViewProjectionMatrix);
    boardMaterial->getParameter("textureRepeat")->setValue(Vector2::one());
    boardMaterial->getParameter("textureTransform")->setValue(Vector2::zero());

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(boardMesh);
}

/**
 * Builds the longboard wheels model.
 */
void LongboardGame::buildWheels()
{
    // Create wheels mesh
    Mesh* wheelsMesh = Mesh::createQuad(Vector3(-0.5f, 0.025f, -0.25f),
                                        Vector3(-0.5f, 0.025f, 0.25f),
                                        Vector3(0.5f, 0.025f, -0.25f),
                                        Vector3(0.5f, 0.025f, 0.25f));
    
    // Create the wheels model
    _wheels = Model::create(wheelsMesh);

    // Create the wheels material
    Material* wheelsMaterial = _wheels->setMaterial("res/shaders/textured.vsh", "res/shaders/textured.fsh");

    // Bind wheels material parameters
    Texture::Sampler* wheelsSampler = wheelsMaterial->getParameter("texture")->setValue("res/textures/wheels.png", true);
    wheelsSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    wheelsMaterial->getParameter("worldViewProjection")->setValue(&_wheelsWorldViewProjectionMatrix);
    wheelsMaterial->getParameter("textureRepeat")->setValue(Vector2::one());
    wheelsMaterial->getParameter("textureTransform")->setValue(Vector2::zero());

    // Load audio sound
    _wheelsSound = AudioSource::create("res/sounds/longboard2.wav");
    if (_wheelsSound)
        _wheelsSound->setLooped(true);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(wheelsMesh);
}

/**
 * Builds a textured gradient model that we use as a cheap/fake
 * static screen-space lighting effect to give a bit more realism.
 */
void LongboardGame::buildGradient()
{
    // Create a full-screen quad for rendering a screen-space gradient effect to the scene
    Mesh* gradientMesh = Mesh::createQuadFullscreen();

    // Create the wheels model
    _gradient = Model::create(gradientMesh);

    // Create the gradient material
    Material* gradientMaterial = _gradient->setMaterial("res/shaders/quad.vsh", "res/shaders/quad.fsh");

    // Bind material parameters
    Texture::Sampler* gradientSampler = gradientMaterial->getParameter("texture")->setValue("res/textures/nice_gradient.png", false);
    gradientSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(gradientMesh);
}

void LongboardGame::update(long elapsedTime)
{
    // Query the accelerometer
    float pitch, roll;
    Input::getAccelerometerPitchAndRoll(&pitch, &roll);

    // Clamp angles
    pitch = fmax(fmin(pitch, PITCH_MAX), PITCH_MIN);
    roll = fmax(fmin(roll, ROLL_MAX), -ROLL_MAX);

    // Calculate the 'throttle' (which is the % controlling change in acceleration, similar to a car's gas pedal)
    float throttle = 1.0f - ((pitch - PITCH_MIN) / PITCH_RANGE);

    if (throttle > 0.0f)
    {
        if (_wheelsSound->getState() != AudioSource::PLAYING)
            _wheelsSound->play();

        _wheelsSound->setPitch(throttle);
    }
    else
    {
        _wheelsSound->stop();
    }

    // Update velocity based on current throttle.
    // Note that this is a very simplified calculation that ignores acceleration (assumes it's constant)
    _velocity = VELOCITY_MIN_MS + ((VELOCITY_MAX_MS - VELOCITY_MIN_MS) * throttle);

    // Update direction based on accelerometer roll and max turn rate
    static Matrix rotMat;
    Matrix::createRotationY(-MATH_DEG_TO_RAD((TURN_RATE_MAX_MS * elapsedTime) * (roll / ROLL_MAX) * throttle), &rotMat);
    rotMat.transformNormal(&_direction);
    _direction.normalize();

    // Transform the ground. We rotate the ground instead of the board since we don't actually
    // move the board along the ground (we just simulate moving it so it appears infinite).
    Matrix::multiply(rotMat, _groundWorldMatrix, &_groundWorldMatrix);
    Matrix::multiply(_viewProjectionMatrix, _groundWorldMatrix, &_groundWorldViewProjectionMatrix);

    // Transform the wheels
    Matrix::createScale(1.2f, 1.2f, 1.2f, &_wheelsWorldMatrix);
    _wheelsWorldMatrix.translate(-roll / ROLL_MAX * 0.05f/*HACK*/, 0, 0.05f);
    _wheelsWorldMatrix.rotateY(MATH_DEG_TO_RAD(roll * 0.45f));
    Matrix::multiply(_viewProjectionMatrix, _wheelsWorldMatrix, &_wheelsWorldViewProjectionMatrix);

    // Transform and tilt the board
    Matrix::createScale(1.25f, 1.25f, 1.25f, &_boardWorldMatrix);
    _boardWorldMatrix.translate(0, 0, 0.65f);//0.15f
    _boardWorldMatrix.rotateZ(MATH_DEG_TO_RAD(roll * 0.5f));
    _boardWorldMatrix.rotateY(MATH_DEG_TO_RAD(roll * 0.1f));//HACK
    Matrix::multiply(_viewProjectionMatrix, _boardWorldMatrix, &_boardWorldViewProjectionMatrix);

    // Transform the ground texture coords to give the illusion of the board moving.
    // We'll assume that a 'patch' of ground is equal to 1 meter.
    _groundUVTransform.x += -_direction.x * (_velocity * elapsedTime);
    _groundUVTransform.y += -_direction.z * (_velocity * elapsedTime);
    if (_groundUVTransform.x >= 1.0f)
    {
        _groundUVTransform.x = 1.0f - _groundUVTransform.x;
    }
    if (_groundUVTransform.y >= 1.0f)
    {
        _groundUVTransform.y = 1.0f - _groundUVTransform.y;
    }
}

void LongboardGame::render(long elapsedTime)
{
    // Clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the scene
    _ground->draw();
    _wheels->draw();
    _board->draw();
    _gradient->draw();
}
