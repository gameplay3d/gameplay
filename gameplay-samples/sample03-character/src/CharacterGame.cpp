#include "CharacterGame.h"

// Declare our game instance
CharacterGame game;

// Input flags
#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define RUNNING 16

// Character defines
#define WALK_SPEED  5.0f
#define STRAFE_SPEED 1.5f
#define RUN_SPEED 15.0f
#define CAMERA_FOCUS_DISTANCE 16.0f

CharacterGame::CharacterGame()
    : _font(NULL), _scene(NULL), _character(NULL), _characterMeshNode(NULL), _characterShadowNode(NULL),
      _animation(NULL), _currentClip(NULL), _rotateX(0), _materialParameterAlpha(NULL),
      _keyFlags(0), _drawDebug(0), _buttonReleased(true), _gamepad(NULL)
{
}

void CharacterGame::initialize()
{
    // Enable multi-touch (only affects devices that support multi-touch).
    setMultiTouch(true);

    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &CharacterGame::drawSplash, NULL, 1000L);

    // Load the font.
    _font = Font::create("res/common/arial40.gpb");

    // Load scene.
    _scene = Scene::load("res/common/scene.scene");

    // Update the aspect ratio for our scene's camera to match the current device resolution.
    _scene->getActiveCamera()->setAspectRatio((float)getWidth() / (float)getHeight());

    // Initialize the physics character.
    initializeCharacter();

    // Initialize the gamepad.
    initializeGamepad();

    // Initialize scene.
    _scene->visit(this, &CharacterGame::initializeScene);
}

bool CharacterGame::initializeScene(Node* node)
{
    Model* model = node->getModel();
    if (model && model->getMaterial())
    {
        initializeMaterial(_scene, node, model->getMaterial());
    }

    return true;
}

void CharacterGame::initializeMaterial(Scene* scene, Node* node, Material* material)
{
    // Bind light shader parameters to dynamic objects only
    if (node->isDynamic())
    {
        Node* lightNode = scene->findNode("sun");
        material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
        material->getParameter("u_lightColor")->bindValue(lightNode->getLight(), &Light::getColor);
        material->getParameter("u_ambientColor")->bindValue(scene, &Scene::getAmbientColor);
    }
}

void CharacterGame::initializeCharacter()
{
    Node* node = _scene->findNode("boycharacter");

    // Store the physics character object.
    _character = static_cast<PhysicsCharacter*>(node->getCollisionObject());

    // Store character nodes.
    _characterMeshNode = node->findNode("boymesh");
    _characterShadowNode = _scene->findNode("boyshadow");

    // Store the alpha material parameter from the character's model.
    _materialParameterAlpha = _characterMeshNode->getModel()->getMaterial()->getTechnique((unsigned int)0)->getPass((unsigned int)0)->getParameter("u_modulateAlpha");

    // Load character animations.
    _animation = node->getAnimation("movements");
    _animation->createClips("res/common/boy.animation");
    _jumpClip = _animation->getClip("jump");
    _jumpClip->addListener(this, _jumpClip->getDuration() - 250);

    // Start playing the idle animation when we load.
    play("idle", true);
}

void CharacterGame::initializeGamepad()
{
    // Get the gamepad loaded for player1 from game.config.
    _gamepad = Game::getInstance()->getGamepad(0);
    GP_ASSERT(_gamepad);
}

void CharacterGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void CharacterGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->begin();
    batch->draw(getWidth() * 0.5f, getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->end();
    SAFE_DELETE(batch);
}

bool CharacterGame::drawScene(Node* node, bool transparent)
{
    if (node->getModel() && (transparent == node->isTransparent()))
        node->getModel()->draw();

    return true;
}

void CharacterGame::play(const char* id, bool repeat, float speed)
{
    AnimationClip* clip = _animation->getClip(id);

    // Set clip properties
    clip->setSpeed(speed);
    clip->setRepeatCount(repeat ? AnimationClip::REPEAT_INDEFINITE : 1);

    // Is the clip already playing?
    if (clip == _currentClip && clip->isPlaying())
        return;

    if (_jumpClip->isPlaying())
    {
        _currentClip = clip;
        return;
    }

    // If a current clip is playing, crossfade into the new one
    if (_currentClip && _currentClip->isPlaying())
    {
        _currentClip->crossFade(clip, 150);
    }
    else
    {
        clip->play();
    }

    _currentClip = clip;
}

void CharacterGame::jump()
{
    if (isOnFloor())
    {
        play("jump", false, 0.55f);
        _character->jump(3.0f);
    }
}

bool CharacterGame::isOnFloor() const
{
    return (std::fabs(_character->getCurrentVelocity().y) < MATH_EPSILON);
}

void CharacterGame::update(float elapsedTime)
{
    if (!_gamepad->isJoystickActive(0))
    {
        // Construct direction vector from keyboard input
        if (_keyFlags & NORTH)
            _currentDirection.y = 1;
        else if (_keyFlags & SOUTH)
            _currentDirection.y = -1;
        else
            _currentDirection.y = 0;

        if (_keyFlags & EAST)
            _currentDirection.x = 1;
        else if (_keyFlags & WEST)
            _currentDirection.x = -1;
        else 
            _currentDirection.x = 0;


        _currentDirection.normalize();
        if ((_keyFlags & RUNNING) == 0)
            _currentDirection *= 0.5f;
    }

    // Update character animation and velocity
    if (_currentDirection.isZero())
    {
        play("idle", true);
        _character->setVelocity(Vector3::zero());
    }
    else
    {
        bool running = (_currentDirection.lengthSquared() > 0.75f);
        float speed = running ? RUN_SPEED : WALK_SPEED;

        play(running ? "running" : "walking", true, 1.0f);

        // Orient the character relative to the camera so he faces the direction we want to move.
        const Matrix& cameraMatrix = _scene->getActiveCamera()->getNode()->getWorldMatrix();
        Vector3 cameraRight, cameraForward;
        cameraMatrix.getRightVector(&cameraRight);
        cameraMatrix.getForwardVector(&cameraForward);

        // Get the current forward vector for the mesh node (negate it since the character was modelled facing +z)
        Vector3 currentHeading(-_characterMeshNode->getForwardVectorWorld());

        // Construct a new forward vector for the mesh node
        Vector3 newHeading(cameraForward * _currentDirection.y + cameraRight * _currentDirection.x);

        // Compute the rotation amount based on the difference between the current and new vectors
        float angle = atan2f(newHeading.x, newHeading.z) - atan2f(currentHeading.x, currentHeading.z);
        if (angle > MATH_PI)
            angle -= MATH_PIX2;
        else if (angle < -MATH_PI)
            angle += MATH_PIX2;
        angle *= (float)elapsedTime * 0.001f * MATH_PIX2;
        _characterMeshNode->rotate(Vector3::unitY(), angle);

        // Update the character's velocity
        Vector3 velocity = -_characterMeshNode->getForwardVectorWorld();
        velocity.normalize();
        velocity *= speed;
        _character->setVelocity(velocity);
    }

    // Adjust camera to avoid it from being obstructed by walls and objects in the scene.
    adjustCamera(elapsedTime);

    // Project the character's shadow node onto the surface directly below him.
    PhysicsController::HitResult hitResult;
    Vector3 v = _character->getNode()->getTranslationWorld();
    if (getPhysicsController()->rayTest(Ray(Vector3(v.x, v.y + 1.0f, v.z), Vector3(0, -1, 0)), 100.0f, &hitResult))
        _characterShadowNode->setTranslation(Vector3(hitResult.point.x, hitResult.point.y + 0.1f, hitResult.point.z));
}

void CharacterGame::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene, with separate passes for opaque and transparent objects.
    _scene->visit(this, &CharacterGame::drawScene, false);
    _scene->visit(this, &CharacterGame::drawScene, true);

    // Draw debug info (physics bodies, bounds, etc).
    switch (_drawDebug)
    {
    case 1:
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
        break;
    case 2:
        _scene->drawDebug(Scene::DEBUG_BOXES);
        break;
    case 3:
        _scene->drawDebug(Scene::DEBUG_SPHERES);
        break;
    }

    // Draw FPS
    _font->begin();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->end();
}

void CharacterGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            _keyFlags |= NORTH;
            _keyFlags &= ~SOUTH;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            _keyFlags |= SOUTH;
            _keyFlags &= ~NORTH;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            _keyFlags |= WEST;
            _keyFlags &= ~EAST;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            _keyFlags |= EAST;
            _keyFlags &= ~WEST;
            break;
        case Keyboard::KEY_B:
            _drawDebug++;
            if (_drawDebug > 3)
                _drawDebug = 0;
            break;
        case Keyboard::KEY_SPACE:
            jump();
            break;
        case Keyboard::KEY_SHIFT:
            _keyFlags |= RUNNING;
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            _keyFlags &= ~NORTH;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            _keyFlags &= ~SOUTH;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            _keyFlags &= ~WEST;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            _keyFlags &= ~EAST;
            break;
        case Keyboard::KEY_SHIFT:
            _keyFlags &= ~RUNNING;
            break;
        }
    }
}

void CharacterGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // This should only be called if the gamepad did not handle the touch event.
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        {
            _rotateX = x;
        }
        break;
    case Touch::TOUCH_RELEASE:
        {
            _rotateX = 0;
        }
        break;
    case Touch::TOUCH_MOVE:
        {
            int deltaX = x - _rotateX;
            _rotateX = x;
            _character->getNode()->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
        }
        break;
    default:
        break;
    }
}

void CharacterGame::adjustCamera(float elapsedTime)
{
    static float cameraOffset = 0.0f;

    PhysicsController* physics = Game::getInstance()->getPhysicsController();
    Node* cameraNode = _scene->getActiveCamera()->getNode();

    // Reset camera
    if (cameraOffset != 0.0f)
    {
        cameraNode->translateForward(-cameraOffset);
        cameraOffset = 0.0f;
    }

    Vector3 cameraPosition = cameraNode->getTranslationWorld();
    Vector3 cameraDirection = cameraNode->getForwardVectorWorld();
    cameraDirection.normalize();

    // Get focal point of camera (use the resolved world location of the head joint as a focal point)
    Vector3 focalPoint(cameraPosition + (cameraDirection * CAMERA_FOCUS_DISTANCE));

    Vector3 oldPosition = cameraNode->getTranslationWorld();

    PhysicsController::HitResult result;
    PhysicsCollisionObject* occlusion = NULL;
    do
    {
        // Perform a ray test to check for camera collisions
        if (!physics->sweepTest(cameraNode->getCollisionObject(), focalPoint, &result) || result.object == _character)
            break;

        occlusion = result.object;

        // Step the camera closer to the focal point to resolve the occlusion
        float d = cameraNode->getTranslationWorld().distance(result.point);
        cameraNode->translateForward(d);
        cameraOffset += d;
        while (physics->sweepTest(cameraNode->getCollisionObject(), focalPoint, &result) && result.object == occlusion)
        {
            // Prevent the camera from getting too close to the character.
            // Without this check, it's possible for the camera to fly past the character
            // and essentially end up in an infinite loop here.
            if (cameraNode->getTranslationWorld().distanceSquared(focalPoint) <= 2.0f)
                return;

            cameraNode->translateForward(0.1f);
            cameraOffset += 0.1f;
        }
    } while (true);

    // If the character is closer than 10 world units to the camera, apply transparency to the character
    // so he does not obstruct the view.
    if (occlusion)
    {
        float d = _scene->getActiveCamera()->getNode()->getTranslationWorld().distance(_characterMeshNode->getTranslationWorld());
        float alpha = d < 10 ? (d * 0.1f) : 1.0f;
        _characterMeshNode->setTransparent(alpha < 1.0f);
        _materialParameterAlpha->setValue(alpha);
    }
    else
    {
        _characterMeshNode->setTransparent(false);
        _materialParameterAlpha->setValue(1.0f);
    }

    Vector3 newPosition = cameraNode->getTranslationWorld();
    float distance = oldPosition.distance(newPosition);
    distance = 0;
}

void CharacterGame::animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type)
{
    clip->crossFade(_currentClip, 150);
}

void CharacterGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int index)
{
    switch(evt)
    {
        case Gamepad::BUTTON_EVENT:
            if (gamepad->getButtonState(index) == Gamepad::BUTTON_PRESSED)
                jump();
            break;
        case Gamepad::JOYSTICK_EVENT:
            _currentDirection = gamepad->getJoystickValue(index);
            break;
    }
    return;
}
