#include "LongboardGame.h"

// Declare our game instance
LongboardGame game;

// Pick some arbitrarily large world size for our playground
#define WORLD_SIZE          20.0f

// Maximum velocity, in meters/sec
#define VELOCITY_MAX        2.0f
#define VELOCITY_MAX_MS     (VELOCITY_MAX / 1000.0f)

// Minimum velocity, in meters/sec
#define VELOCITY_MIN        0.2f
#define VELOCITY_MIN_MS     (VELOCITY_MIN / 1000.0f)

// Keyboard input bit-flags (powers of 2)
#define ACCELERATE (1 << 0)
#define BRAKE (1 << 1)
#define STEER_LEFT (1 << 2)
#define STEER_RIGHT (1 << 3)

// Accelerometer pitch control
#define PITCH_MIN            20.0f
#define PITCH_MAX            70.0f
#define PITCH_RANGE          (PITCH_MAX - PITCH_MIN)

// Maximum turn rate (rate of change in direction at full velocity), in degrees/sec
#define TURN_RATE_MAX        75.0f
#define TURN_RATE_MAX_MS     (TURN_RATE_MAX / 1000.0f)
#define ROLL_MAX             40.0f

LongboardGame::LongboardGame() 
    : _ground(NULL), _board(NULL), _wheels(NULL), _gradient(NULL), _stateBlock(NULL),
    _keyFlags(0), _simulatedPitch(0), _simulatedRoll(0), _velocity(VELOCITY_MIN_MS)
{
}

LongboardGame::~LongboardGame()
{
}

void LongboardGame::initialize()
{
    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &LongboardGame::drawSplash, NULL, 1000L);

    // Create our render state block that will be reused across all materials
    _stateBlock = RenderState::StateBlock::create();
    _stateBlock->setCullFace(true);
    _stateBlock->setBlend(true);
    _stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    _stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

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

void LongboardGame::finalize()
{
    SAFE_RELEASE(_stateBlock);
    SAFE_RELEASE(_ground);
    SAFE_RELEASE(_board);
    SAFE_RELEASE(_wheels);
    SAFE_RELEASE(_gradient);
    SAFE_RELEASE(_wheelsSound);
}

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
    Material* groundMaterial = _ground->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT;TEXTURE_OFFSET");

    // Set render state block
    groundMaterial->setStateBlock(_stateBlock);

    // Bind ground material parameters
    Texture::Sampler* groundSampler = groundMaterial->getParameter("u_diffuseTexture")->setValue("res/asphalt.png", true);
    groundSampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
    groundMaterial->getParameter("u_worldViewProjectionMatrix")->setValue(&_groundWorldViewProjectionMatrix);
    groundMaterial->getParameter("u_textureRepeat")->setValue(Vector2(WORLD_SIZE / 2, WORLD_SIZE / 2));
    groundMaterial->getParameter("u_textureOffset")->setValue(&_groundUVTransform);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(groundMesh);
}

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
    Material* boardMaterial = _board->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT;TEXTURE_OFFSET");

    // Set render state block
    boardMaterial->setStateBlock(_stateBlock);

    // Bind board material parameters
    Texture::Sampler* boardSampler = boardMaterial->getParameter("u_diffuseTexture")->setValue("res/longboard.png", true);
    boardSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    boardMaterial->getParameter("u_worldViewProjectionMatrix")->setValue(&_boardWorldViewProjectionMatrix);
    boardMaterial->getParameter("u_textureRepeat")->setValue(Vector2::one());
    boardMaterial->getParameter("u_textureOffset")->setValue(Vector2::zero());

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(boardMesh);
}

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
    Material* wheelsMaterial = _wheels->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT;TEXTURE_OFFSET");

    // Set render state block
    wheelsMaterial->setStateBlock(_stateBlock);

    // Bind wheels material parameters
    Texture::Sampler* wheelsSampler = wheelsMaterial->getParameter("u_diffuseTexture")->setValue("res/longboard_wheels.png", true);
    wheelsSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    wheelsMaterial->getParameter("u_worldViewProjectionMatrix")->setValue(&_wheelsWorldViewProjectionMatrix);
    wheelsMaterial->getParameter("u_textureRepeat")->setValue(Vector2::one());
    wheelsMaterial->getParameter("u_textureOffset")->setValue(Vector2::zero());

    // Load audio sound
    _wheelsSound = AudioSource::create("res/longboard.wav");
    if (_wheelsSound)
        _wheelsSound->setLooped(true);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(wheelsMesh);
}

void LongboardGame::buildGradient()
{
    // Create a full-screen quad for rendering a screen-space gradient effect to the scene
    Mesh* gradientMesh = Mesh::createQuadFullscreen();

    // Create the wheels model
    _gradient = Model::create(gradientMesh);

    // Create the gradient material.
    Material* gradientMaterial = _gradient->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag");

    // Set render state block
    gradientMaterial->setStateBlock(_stateBlock);

    // Bind material parameters
    Texture::Sampler* gradientSampler = gradientMaterial->getParameter("u_diffuseTexture")->setValue("res/overlay_gradient.png", false);
    gradientSampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(gradientMesh);
}

void LongboardGame::update(float elapsedTime)
{
    // Query the accelerometer values.
    float pitch, roll;
    getAccelerometerValues(&pitch, &roll);

    // On desktop platforms without accelerometers, use key inputs instead.
    if (!Game::hasAccelerometer())
    {
        float pitchTarget = 0;
        float rollTarget = 0;

        pitch = _simulatedPitch;
        roll = _simulatedRoll;

        pitchTarget = -0.5f * (PITCH_MAX + PITCH_MIN);
        if (_keyFlags & BRAKE)
        {
            pitchTarget = -PITCH_MAX;
        }
        else if (_keyFlags & ACCELERATE)
        {
            pitchTarget = -PITCH_MIN;
        }

        if (_keyFlags & STEER_RIGHT)
        {
            rollTarget += ROLL_MAX;
        }

        if (_keyFlags & STEER_LEFT)
        {
            rollTarget -= ROLL_MAX;
        }

        MathUtil::smooth(&_simulatedPitch, pitchTarget, elapsedTime, 600);
        MathUtil::smooth(&_simulatedRoll, rollTarget, elapsedTime, 100);
    }

    // Clamp angles
    pitch = max(min(-pitch, PITCH_MAX), PITCH_MIN);
    roll = max(min(roll, ROLL_MAX), -ROLL_MAX);

    
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
    Matrix::createRotationY(MATH_DEG_TO_RAD((TURN_RATE_MAX_MS * elapsedTime) * (roll / ROLL_MAX) * throttle), &rotMat);
    rotMat.transformVector(&_direction);
    _direction.normalize();

    // Transform the ground. We rotate the ground instead of the board since we don't actually
    // move the board along the ground (we just simulate moving it so it appears infinite).
    Matrix::multiply(rotMat, _groundWorldMatrix, &_groundWorldMatrix);
    Matrix::multiply(_viewProjectionMatrix, _groundWorldMatrix, &_groundWorldViewProjectionMatrix);

    // Transform the wheels
    Matrix::createScale(1.2f, 1.2f, 1.2f, &_wheelsWorldMatrix);
    _wheelsWorldMatrix.translate(roll / ROLL_MAX * 0.05f, 0, 0.05f);
    _wheelsWorldMatrix.rotateY(-MATH_DEG_TO_RAD(roll * 0.45f));
    Matrix::multiply(_viewProjectionMatrix, _wheelsWorldMatrix, &_wheelsWorldViewProjectionMatrix);

    // Transform and tilt the board
    Matrix::createScale(1.25f, 1.25f, 1.25f, &_boardWorldMatrix);
    _boardWorldMatrix.translate(0, 0, 0.65f);
    _boardWorldMatrix.rotateZ(-MATH_DEG_TO_RAD(roll * 0.5f));
    _boardWorldMatrix.rotateY(-MATH_DEG_TO_RAD(roll * 0.1f));
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

void LongboardGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
        case Keyboard::KEY_LEFT_ARROW:
            _keyFlags |= STEER_LEFT;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
        case Keyboard::KEY_RIGHT_ARROW:
            _keyFlags |= STEER_RIGHT;
            break;
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
        case Keyboard::KEY_UP_ARROW:
            _keyFlags |= ACCELERATE;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
        case Keyboard::KEY_DOWN_ARROW:
            _keyFlags |= BRAKE;
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
        case Keyboard::KEY_LEFT_ARROW:
            _keyFlags &= ~STEER_LEFT;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
        case Keyboard::KEY_RIGHT_ARROW:
            _keyFlags &= ~STEER_RIGHT;
            break;
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
        case Keyboard::KEY_UP_ARROW:
            _keyFlags &= ~ACCELERATE;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
        case Keyboard::KEY_DOWN_ARROW:
            _keyFlags &= ~BRAKE;
            break;
        }
    }
}

void LongboardGame::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR, Vector4::one(), 1.0f, 0);

    // Draw the scene
    _ground->draw();
    _wheels->draw();
    _board->draw();
    _gradient->draw();
}

void LongboardGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}
