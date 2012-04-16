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
    : _font(NULL), _scene(NULL), _character(NULL), _animation(NULL), _currentClip(NULL), _rotateX(0), _materialParameterAlpha(NULL),
      _inputFlags(0), _drawDebug(0)
{
}

void CharacterGame::initialize()
{
    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &CharacterGame::drawSplash, NULL, 1000L);

    // Load the font.
    _font = Font::create("res/arial40.gpb");

    // Load scene.
    _scene = Scene::load("res/scene.scene");

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

    // Store character mesh node.
    _characterMeshNode = node->findNode("boymesh");

    // Store the alpha material parameter from the character's model.
    _materialParameterAlpha = _characterMeshNode->getModel()->getMaterial()->getTechnique((unsigned int)0)->getPass((unsigned int)0)->getParameter("u_globalAlpha");

    // Load character animations.
    _animation = node->getAnimation("movements");
    _animation->createClips("res/boy.animation");

    // Start playing the idle animation when we load.
    play("idle", true);
}

void CharacterGame::initializeGamepad()
{
    _gamepad = new Gamepad("res/gamepad.png", 1, 1);

    float scale = getHeight() / 720.0f;

    // Add a single gamepad joystick
    _gamepad->setJoystick(0,
        Rectangle(120.0f * scale, getHeight() - 130.0f * scale, 47.0f * scale, 47.0f * scale), 
        Rectangle(10, 188, 47, 47),
        Rectangle(48.0f * scale, getHeight() - 191.0f * scale, 170.0f * scale, 170.0f * scale),
        Rectangle(0, 0, 170, 170),
        45.0f);

    // Add a single gamepad button
	_gamepad->setButton(0,
        Rectangle(getWidth() - scale * (120.0f + 47.0f), getHeight() - 130.0f * scale, 47.0f * scale, 47.0f * scale),
        Rectangle(10, 188, 47, 47),
        Rectangle(69, 188, 47, 47));
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

void CharacterGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    SAFE_DELETE(_gamepad);
}

bool CharacterGame::isJumping() const
{
    if (!_currentClip)
        return false;

    if (strcmp(_currentClip->getID(), "jump") == 0)
        return true;

    if (strcmp(_currentClip->getID(), "runningJump") == 0)
        return true;

    return false;
}

void CharacterGame::play(const char* id, bool repeat)
{
    AnimationClip* clip = _animation->getClip(id);

    // Is the clip already playing?
    if (clip->isPlaying())
        return;

    // Set clip properties
    clip->setRepeatCount(repeat ? AnimationClip::REPEAT_INDEFINITE : 1);

    // If a current clip is playing, crossfade into the new one
    if (_currentClip && _currentClip->isPlaying())
    {
        _currentClip->crossFade(clip, 300.0f);
    }
    else
    {
        clip->play();
    }

    _currentClip = clip;
}

void CharacterGame::update(long elapsedTime)
{
    Vector2 joystickVec = _gamepad->getJoystickState(0);
    if (!joystickVec.isZero())
    {
	    _inputFlags = 0;

        // Calculate rotation
        float angle = joystickVec.x * MATH_PI * -0.015;
        _character->rotate(Vector3::unitY(), angle);
    }

    /*Gamepad::ButtonState buttonOneState = _gamepad->getButtonState(BUTTON_1);
    if (buttonOneState)
    {
        _inputFlags = 16;
    }

    if (_inputFlags == 16)
    {
        play("jump", false);
    }*/

    /*if (_inputFlags == 0)
    {
        play("idle", true);
        _character->setForwardVelocity(0);
        _character->setRightVelocity(0);
    }
    else*/
    {
        if (_inputFlags & NORTH)
        {
            if (_inputFlags & RUNNING)
            {
                play("running", true);
                _character->setForwardVelocity(RUN_SPEED);
            }
            else
            {
                play("walking", true);
                _character->setForwardVelocity(WALK_SPEED);
            }

            if (_inputFlags & EAST)
                _character->setRightVelocity(STRAFE_SPEED);
            else if (_inputFlags & WEST)
                _character->setRightVelocity(-STRAFE_SPEED);
            else
                _character->setRightVelocity(0);
        }
        else if (_inputFlags & SOUTH)
        {
            play("walking", true);
            _character->setForwardVelocity(-WALK_SPEED);

            if (_inputFlags & EAST)
                _character->setRightVelocity(STRAFE_SPEED);
            else if (_inputFlags & WEST)
                _character->setRightVelocity(-STRAFE_SPEED);
            else
                _character->setRightVelocity(0);
        }
        else if (_inputFlags & EAST)
        {
            play("strafeRight", true);
            _character->setForwardVelocity(0);
            _character->setRightVelocity(STRAFE_SPEED);
        }
        else if (_inputFlags & WEST)
        {
            play("strafeLeft", true);
            _character->setForwardVelocity(0);
            _character->setRightVelocity(-STRAFE_SPEED);
        }
        else
        {
            play("idle", true);
            _character->setRightVelocity(0);
            _character->setForwardVelocity(0);
        }
    }

    adjustCamera(elapsedTime);

    PhysicsController::HitResult hitResult;
    Vector3 v = _character->getNode()->getTranslationWorld();
    if (getPhysicsController()->rayTest(Ray(Vector3(v.x, v.y + 1.0f, v.z), Vector3(0, -1, 0)), 100.0f, &hitResult))
        _scene->findNode("boyshadow")->setTranslation(Vector3(hitResult.point.x, hitResult.point.y + 0.1f, hitResult.point.z));
}

void CharacterGame::render(long elapsedTime)
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

    // Draw gamepad for touch devices.
    //if (isMultiTouch())
        _gamepad->draw();

    // Draw FPS
    _font->begin();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->end();
}

bool CharacterGame::drawScene(Node* node, bool transparent)
{
    if (node->getModel() && (transparent == node->isTransparent()))
        node->getModel()->draw();

    return true;
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
            _inputFlags |= NORTH;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            _inputFlags |= SOUTH;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            _inputFlags |= WEST;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            _inputFlags |= EAST;
            break;
        case Keyboard::KEY_B:
            _drawDebug++;
            if (_drawDebug > 3)
                _drawDebug = 0;
            break;
        case Keyboard::KEY_SPACE:
            play("jump", false);
            _character->jump(3.0f);
            break;
        case Keyboard::KEY_SHIFT:
            _inputFlags |= RUNNING;
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            _inputFlags &= ~NORTH;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            _inputFlags &= ~SOUTH;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            _inputFlags &= ~WEST;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            _inputFlags &= ~EAST;
            break;
        case Keyboard::KEY_SHIFT:
            _inputFlags &= ~RUNNING;
            break;
        }
    }
}

void CharacterGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // Get the joystick's current state.
    bool wasActive = _gamepad->isJoystickActive(0);

    _gamepad->touchEvent(evt, x, y, contactIndex);

    // See if the joystick is still active.
    bool isActive = _gamepad->isJoystickActive(0);
    if (!isActive)
    {
        // If it was active before, reset the joystick's influence on the _inputFlags.
        if (wasActive)
            _inputFlags = 0;

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
}

void CharacterGame::adjustCamera(long elapsedTime)
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

    PhysicsController::HitResult result;
    PhysicsCollisionObject* occlusion = NULL;
    do
    {
        // Perform a ray test to check for camera collisions
        if (!physics->sweepTest(cameraNode->getCollisionObject(), focalPoint, &result) || result.object == _character)
            break;

        occlusion = result.object;

        // Step the camera closer to the focal point to resolve the occlusion
        do
        {
            // Prevent the camera from getting too close to the character.
            // Without this check, it's possible for the camera to fly past the character
            // and essentially end up in an infinite loop here.
            if (cameraNode->getTranslationWorld().distanceSquared(focalPoint) <= 2.0f)
                return;

            cameraNode->translateForward(0.1f);
            cameraOffset += 0.1f;

        } while (physics->sweepTest(cameraNode->getCollisionObject(), focalPoint, &result) && result.object == occlusion);

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
}
