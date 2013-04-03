#include "SpaceshipGame.h"

// Declare our game instance
SpaceshipGame game;

// Collision constants
#define ROOF_HEIGHT 11.6f
#define FLOOR_HEIGHT 0.6f
#define MAP_LENGTH 1000.0f
#define CAMERA_RANGE_FRONT -1
#define CAMERA_RANGE_BACK 8

// Mass of the space ship
#define MASS 50.0f

// Gravity constant (earth's gravity is ~9.8m/s/s)
#define GRAVITY 9.8f

// Force applied due to gravity (F = mg)
#define GRAVITATIONAL_FORCE (MASS * GRAVITY)

// Normal force used in friction computations (Fn = -mg)
#define FORCE_NORMAL (-GRAVITATIONAL_FORCE)

// Friction between floor and spaceship (Ff = u * Fn)
#define FLOOR_FRICTION (0.4f * FORCE_NORMAL)

// Friction between roof and spaceship (Ff = u * Fn)
#define ROOF_FRICTION (0.035f * FORCE_NORMAL)

// Distance from space ship (screen-space) after which force is no longer applied to the ship
#define TOUCH_DISTANCE_MAX 400.0f

// Amount to scale screen-space push/force vector by
#define FORCE_SCALE 4.0f

// Constant for the maximum force that can be applied to the ship
#define FORCE_MAX (TOUCH_DISTANCE_MAX * FORCE_SCALE)

// Maximum velocity (+/-)
#define VELOCITY_MAX 75.0f

// Maximum ship tilt (degrees)
#define SHIP_TILT_MAX 45.0f

// Maximum ship rotation (degrees) at full throttle
#define SHIP_ROTATE_SPEED_MAX 500.0f

// Multiplier for engine glow effect (based on throttle)
#define ENGINE_POWER 2.5f

// Multiplier for sound pitch (based on throttle)
#define SOUND_PITCH_SCALE 2.0f

// Lighting constants
#define AMBIENT_LIGHT_COLOR Vector3(0.2f, 0.2f, 0.2f)
#define SPECULAR 20.0f

// Clamp function
#define CLAMP(x, min, max) (x < min ? min : (x > max ? max : x))

SpaceshipGame::SpaceshipGame() 
    : _scene(NULL), _cameraNode(NULL), _shipGroupNode(NULL), _shipNode(NULL), _propulsionNode(NULL), _glowNode(NULL),
      _stateBlock(NULL), _font(NULL), _throttle(0), _shipTilt(0), _finished(false), _finishedTime(0), _pushing(false), _time(0), 
       _glowDiffuseParameter(NULL), _shipSpecularParameter(NULL), _spaceshipSound(NULL)
{
}

SpaceshipGame::~SpaceshipGame()
{
}

void SpaceshipGame::initialize()
{
    // TODO: Not working on iOS
    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &SpaceshipGame::drawSplash, NULL, 1000L);

    // Create our render state block that will be reused across all materials
    _stateBlock = RenderState::StateBlock::create();
    _stateBlock->setDepthTest(true);
    _stateBlock->setCullFace(true);
    _stateBlock->setBlend(true);
    _stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    _stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    // Load our scene from file
    _scene = Scene::load("res/spaceship.gpb");

    // Update the aspect ratio for our scene's camera to match the current device resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    // Initialize scene data
    initializeSpaceship();
    initializeEnvironment();

    // Create a background audio track
    _backgroundSound = AudioSource::create("res/background.ogg");
    if (_backgroundSound)
        _backgroundSound->setLooped(true);
    
    // Create font
    _font = Font::create("res/airstrip28.gpb");

    // Store camera node
    _cameraNode = _scene->findNode("camera1");

    // Store initial ship and camera positions
    _initialShipPos = _shipGroupNode->getTranslation();
    _initialShipRot = _shipGroupNode->getRotation();
    _initialCameraPos = _cameraNode->getTranslation();
}

void SpaceshipGame::initializeSpaceship()
{
    Material* material;

    _shipGroupNode = _scene->findNode("gSpaceShip");

    // Setup spaceship model
    // Part 0
    _shipNode = _scene->findNode("pSpaceShip");
    material = _shipNode->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR", 0);
    material->getParameter("u_diffuseColor")->setValue(Vector4(0.53544f, 0.53544f, 0.53544f, 1.0f));
    initializeMaterial(material, true, true);
    // Part 1
    material = _shipNode->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR", 1);
    material->getParameter("u_diffuseColor")->setValue(Vector4(0.8f, 0.8f, 0.8f, 1.0f));
    _shipSpecularParameter = material->getParameter("u_specularExponent");
    initializeMaterial(material, true, true);
    // Part 2
    material = _shipNode->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR", 2);
    material->getParameter("u_diffuseColor")->setValue(Vector4(0.280584f, 0.5584863f, 0.6928f, 1.0f));
    initializeMaterial(material, true, true);

    // Setup spaceship propulsion model
    _propulsionNode = _scene->findNode("pPropulsion");
    material = _propulsionNode->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR");
    material->getParameter("u_diffuseColor")->setValue(Vector4(0.8f, 0.8f, 0.8f, 1.0f));
    initializeMaterial(material, true, true);

    // Glow effect node
    _glowNode = _scene->findNode("pGlow");
    material = _glowNode->getModel()->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "MODULATE_COLOR");
    material->getParameter("u_diffuseTexture")->setValue("res/propulsion_glow.png", true);
    _glowDiffuseParameter = material->getParameter("u_modulateColor");
    initializeMaterial(material, false, false);

    // Setup the sound
    _spaceshipSound = AudioSource::create("res/spaceship.wav");
    if (_spaceshipSound)
    {
        _spaceshipSound->setLooped(true);
    }
}

void SpaceshipGame::initializeEnvironment()
{
    Material* material;
    std::vector<Node*> nodes;

    // Setup ground model
    _scene->findNodes("pGround", nodes, true, false);
    for (size_t i = 0, count = nodes.size(); i < count; ++i)
    {
        Node* pGround = nodes[i];
        material = pGround->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR");
        material->getParameter("u_diffuseColor")->setValue(Vector4(0.280584f, 0.5584863f, 0.6928f, 1.0f));
        initializeMaterial(material, true, true);
    }

    // Setup roof model
    nodes.clear();
    _scene->findNodes("pRoof", nodes, true, false);
    for (size_t i = 0, count = nodes.size(); i < count; ++i)
    {
        Node* pRoof = nodes[i];
        material = pRoof->getModel()->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "SPECULAR");
        material->getParameter("u_diffuseColor")->setValue(Vector4(0.280584f, 0.5584863f, 0.6928f, 1.0f));
        initializeMaterial(material, true, true);
    }

    // Setup background model
    nodes.clear();
    Node* pBackground = _scene->findNode("pBackground");
    material = pBackground->getModel()->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag");
    material->getParameter("u_diffuseTexture")->setValue("res/background.png", true);
    initializeMaterial(material, true, false);
}

void SpaceshipGame::initializeMaterial(Material* material, bool lighting, bool specular)
{
    // Set the common render state block for the material
    material->setStateBlock(_stateBlock);

    // Bind the WorldViewProjection matrix
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);

    if (lighting)
    {
        // Apply lighting material parameters
        material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);

        Node* lightNode = _scene->findNode("directionalLight1");
        Vector3 lightDirection = lightNode->getForwardVector();
        lightDirection.normalize();

        material->getParameter("u_lightDirection")->setValue(lightDirection);
        material->getParameter("u_lightColor")->setValue(lightNode->getLight()->getColor());
        material->getParameter("u_ambientColor")->setValue(AMBIENT_LIGHT_COLOR);
       

        if (specular)
        {
            // Apply specular lighting parameters
            material->getParameter("u_specularExponent")->setValue(SPECULAR);
            material->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);
            material->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_WORLD_POSITION);
        }
    }
}

void SpaceshipGame::finalize()
{
    SAFE_RELEASE(_backgroundSound);
    SAFE_RELEASE(_spaceshipSound);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_stateBlock);
    SAFE_RELEASE(_scene);
}

void SpaceshipGame::update(float elapsedTime)
{
    // Calculate elapsed time in seconds
    float t = (float)elapsedTime / 1000.0;
    
    if (!_finished)
    {
        _time += t;

        // Play the background track
        if (_backgroundSound->getState() != AudioSource::PLAYING)
            _backgroundSound->play();
    }
    else
    {
        // Stop the background track
        if (_backgroundSound->getState() != AudioSource::STOPPED)
            _backgroundSound->stop();

        _throttle = 0.0f;
    }

    // Set initial force due to gravity
    _force.set(0, -GRAVITATIONAL_FORCE);

    // While we are pushing/touching the screen, apply a push force vector based on the distance from
    // the touch point to the center of the space ship.
    if (_pushing)
    {
        // Get the center point of the space ship in screen coordinates
        Vector3 shipCenterScreen;
        _scene->getActiveCamera()->project(getViewport(), _shipGroupNode->getBoundingSphere().center, &shipCenterScreen.x, &shipCenterScreen.y);

        // Compute a screen-space vector between the center point of the ship and the touch point.
        // We will use this vector to apply a "pushing" force to the space ship, similar to what
        // happens when you hold a magnet close to an object with opposite polarity.
        Vector2 pushForce((shipCenterScreen.x - _pushPoint.x), -(shipCenterScreen.y - _pushPoint.y));
        
        // Transform the vector so that a smaller magnitude emits a larger force and applying the
        // maximum touch distance.
        float distance = (std::max)(TOUCH_DISTANCE_MAX - pushForce.length(), 0.0f);
        pushForce.normalize();
        pushForce.scale(distance * FORCE_SCALE);
        _force.add(pushForce);

        // Compute a throttle value based on our force vector, minus gravity
        Vector2 throttleVector(_force.x, _force.y + GRAVITATIONAL_FORCE);
        _throttle += throttleVector.length() / FORCE_MAX * t;
    }
    else
    {
        // Gradually decrease the throttle
        if (_throttle > 0.0f)
        {
            _throttle *= 1.0f - t;
        }
    }

    // Clamp the throttle
    _throttle = CLAMP(_throttle, 0.0f, 1.0f);

    // Update acceleration (a = F/m)
    _acceleration.set(_force.x / MASS, _force.y / MASS);

    // Update velocity (v1 = v0 + at)
    _velocity.x += _acceleration.x * t;
    _velocity.y += _acceleration.y * t;

    // Clamp velocity to its maximum range
    _velocity.x = CLAMP(_velocity.x, -VELOCITY_MAX, VELOCITY_MAX);
    _velocity.y = CLAMP(_velocity.y, -VELOCITY_MAX, VELOCITY_MAX);

    // Move the spaceship based on its current velocity (x1 = x0 + vt)
    _shipGroupNode->translate(_velocity.x * t, _velocity.y * t, 0);

    // Check for collisions
    handleCollisions(t);

    // Update camera
    updateCamera();

    // Reset ship rotation
    _shipGroupNode->setRotation(_initialShipRot);

    // Apply ship tilt
    if (_force.x != 0 && fabs(_velocity.x) > 0.1f)
    {
        // Compute an angle based on the dot product between the force vector and the Y axis
        Vector2 fn;
        _force.normalize(&fn);
        float angle = MATH_RAD_TO_DEG(acos(Vector2::dot(Vector2(0, 1), fn)));
        if (_force.x > 0)
            angle = -angle;
        angle *= _throttle * t;
        _shipTilt += angle;
        _shipTilt = _shipTilt < -SHIP_TILT_MAX ? -SHIP_TILT_MAX : (_shipTilt > SHIP_TILT_MAX ? SHIP_TILT_MAX : _shipTilt);
    }
    else
    {
        // Interpolate tilt back towards zero when no force is applied
        _shipTilt = (_shipTilt + ((0 - _shipTilt) * t * 2.0f));
    }
    _shipGroupNode->rotateZ(MATH_DEG_TO_RAD(_shipTilt));

    if (_throttle > MATH_EPSILON)
    {
        // Apply ship spin
        _shipNode->rotateY(MATH_DEG_TO_RAD(SHIP_ROTATE_SPEED_MAX * t * _throttle));

        // Play sound effect
        if (_spaceshipSound->getState() != AudioSource::PLAYING)
            _spaceshipSound->play();
        
        // Set the pitch based on the throttle
        _spaceshipSound->setPitch(_throttle * SOUND_PITCH_SCALE);
    }
    else
    {
        // Stop sound effect
        _spaceshipSound->stop();
    }

    // Modify ship glow effect based on the throttle
    _glowDiffuseParameter->setValue(Vector4(1, 1, 1, _throttle * ENGINE_POWER));
    _shipSpecularParameter->setValue(SPECULAR - ((SPECULAR-2.0f) * _throttle));
}

void SpaceshipGame::handleCollisions(float t)
{
    float friction = 0.0f;

    // Use the ship's bounding sphere for roof collisions
    const BoundingSphere& shipBounds = _shipNode->getBoundingSphere();

    // Compute a bounding box for floor collisions
    BoundingBox propulsionBounds = _propulsionNode->getModel()->getMesh()->getBoundingBox();
    propulsionBounds.transform(_propulsionNode->getWorldMatrix());

    if (propulsionBounds.min.y <= FLOOR_HEIGHT)
    {
        // Floor collision
        friction = FLOOR_FRICTION;
        _shipGroupNode->translateY(FLOOR_HEIGHT - propulsionBounds.min.y);
        if (_velocity.y < 0.0f)
        {
            // Cancel vertical velocity
            _velocity.y = 0.0f;
        }
    }
    else if ((shipBounds.center.y + shipBounds.radius) >= ROOF_HEIGHT)
    {
        // Roof collision
        friction = ROOF_FRICTION;
        _shipGroupNode->translateY(ROOF_HEIGHT - (shipBounds.center.y + shipBounds.radius));
        if (_velocity.y >= 0.0f)
        {
            // Cancel vertical velocity
            _velocity.y = 0.0f;
        }
    }

    // Apply friction to velocity
    if (friction != 0.0f)
    {
        if (_velocity.x > 0)
        {
            _velocity.x = (std::max)(_velocity.x + friction * t, 0.0f);
        }
        else if (_velocity.x < 0)
        {
            _velocity.x = (std::min)(_velocity.x - friction  * t, 0.0f);
        }
    }

    // Keep the ship within the playable area of the map
    const Vector3& shipPos = _shipGroupNode->getTranslation();
    if (shipPos.x < _initialShipPos.x)
    {
        _shipGroupNode->translateX(_initialShipPos.x - shipPos.x);
        _velocity.x = 0.0f;
    }
    else if (shipPos.x > (_initialShipPos.x + MAP_LENGTH))
    {
        if (!_finished)
        {
            // Passed the finish line
            _finished = true;
            _finishedTime = getAbsoluteTime();
            _pushing = false;
        }
    }
}

void SpaceshipGame::updateCamera()
{
    if (_finished)
        return;

    // Keep the camera focused on the ship
    const Vector3& cameraPos = _cameraNode->getTranslation();
    const Vector3& shipPos = _shipGroupNode->getTranslation();
    float diff = cameraPos.x - shipPos.x;
    if (diff > CAMERA_RANGE_BACK)
    {
        _cameraNode->translateX(-(diff - CAMERA_RANGE_BACK));
    }
    else if (diff < -CAMERA_RANGE_FRONT)
    {
        _cameraNode->translateX(-(diff + CAMERA_RANGE_FRONT));
    }
}

void SpaceshipGame::resetGame()
{
    _time = 0;
    _finished = false;
    _velocity.set(0, 0);
    _shipGroupNode->setTranslation(_initialShipPos);
    _cameraNode->setTranslation(_initialCameraPos);
}

void SpaceshipGame::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit scene nodes for opaque drawing
    _scene->visit(this, &SpaceshipGame::drawScene, (void*)0);

    // Visit scene nodes for transparent drawing
    _scene->visit(this, &SpaceshipGame::drawScene, (void*)1);

    // Draw game text (yellow)
    drawText();
}

void SpaceshipGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}

bool SpaceshipGame::drawScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        // Transparent nodes must be drawn last (stage 1)
        bool isTransparent = (node == _glowNode);

        // Skip transparent objects for stage 0
        if ((!isTransparent && (int*)cookie == 0) || (isTransparent && (int*)cookie == (int*)1))
            model->draw();
    }

    return true;
}

void SpaceshipGame::drawText()
{
    _font->start();
    char text[1024];
    sprintf(text, "%dsec.", (int)_time);
    _font->drawText(text, getWidth() - 120, 10, Vector4(1, 1, 0, 1), _font->getSize());
    if (_finished)
    {
        _font->drawText("Click to Play Again", getWidth()/2 - 175, getHeight()/2 - 40, Vector4::one(), _font->getSize());
    }
    _font->finish();
}

void SpaceshipGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void SpaceshipGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (_finished && (getAbsoluteTime() - _finishedTime) > 1000L)
        {
            resetGame();
        }
    case Touch::TOUCH_MOVE:
        if (!_finished)
        {
            _pushing = true;
            _pushPoint.set(x, y);
        }
        break;

    case Touch::TOUCH_RELEASE:
        _pushing = false;
        break;
    }
}
