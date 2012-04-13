#include "CharacterGame.h"

// Declare our game instance
CharacterGame game;

#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define RUNNING 16

#define WALK_SPEED  5.0f
#define STRAFE_SPEED 1.5f
#define RUN_SPEED 15.0f
#define OPAQUE_OBJECTS      0
#define TRANSPARENT_OBJECTS 1
#define CAMERA_FOCUS_DISTANCE 16.0f

#define SCREEN_WIDTH getWidth()
#define SCREEN_HEIGHT getHeight()
#define SCALE_FACTOR (SCREEN_HEIGHT / 720.0f)
#define THUMB_WIDTH 47.0f
#define THUMB_HEIGHT 47.0f
#define THUMB_X 10.0f
#define THUMB_Y 188.0f
#define THUMB_SCREEN_X 120.0f 
#define THUMB_SCREEN_Y 130.0f
#define DOCK_WIDTH 170.0f
#define DOCK_HEIGHT 170.0f
#define DOCK_X 0.0f
#define DOCK_Y 0.0f
#define DOCK_SCREEN_X 48.0f
#define DOCK_SCREEN_Y 191.0f
#define BUTTON_WIDTH 47.0f
#define BUTTON_HEIGHT 47.0f
#define BUTTON_PRESSED_X 69.0f
#define BUTTON_PRESSED_Y 188.0f
#define BUTTON_RELEASED_X 10.0f
#define BUTTON_RELEASED_Y 188.0f
#define BUTTON_SCREEN_X 120.0f
#define BUTTON_SCREEN_Y 130.0f
#define JOYSTICK_RADIUS 45.0f

unsigned int inputFlags = 0;
float velocityNS = 0.0f;
float velocityEW = 0.0f;
int drawDebug = 0;

CharacterGame::CharacterGame()
    : _font(NULL), _scene(NULL), _character(NULL), _animation(NULL), _currentClip(NULL), _rotateX(0), _materialParameterAlpha(NULL)
{
}

CharacterGame::~CharacterGame()
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

    // Update the aspect ratio for our scene's camera to match the current device resolution
    _scene->getActiveCamera()->setAspectRatio((float)SCREEN_WIDTH / (float) SCREEN_HEIGHT);

    // Initialize the physics character
    initializeCharacter();

    // Set a ghost object on our camera node to assist in camera occlusion adjustments
    _scene->findNode("Camera")->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT, PhysicsCollisionShape::sphere(0.5f));

    // Initialize scene.
    _scene->visit(this, &CharacterGame::initScene);

    // Initialize the gamepad.
	_gamepad = new Gamepad("res/gamepad.png", 1, 1);

    Gamepad::Rect thumbScreenRegion = {THUMB_SCREEN_X * SCALE_FACTOR, SCREEN_HEIGHT - THUMB_SCREEN_Y * SCALE_FACTOR, THUMB_WIDTH * SCALE_FACTOR, THUMB_HEIGHT * SCALE_FACTOR};
    Gamepad::Rect thumbTexRegion =    {THUMB_X, THUMB_Y, THUMB_WIDTH, THUMB_HEIGHT};
    Gamepad::Rect dockScreenRegion =  {DOCK_SCREEN_X * SCALE_FACTOR, SCREEN_HEIGHT - DOCK_SCREEN_Y * SCALE_FACTOR, DOCK_WIDTH * SCALE_FACTOR, DOCK_HEIGHT * SCALE_FACTOR};
    Gamepad::Rect dockTexRegion =     {DOCK_X, DOCK_Y, DOCK_WIDTH, DOCK_HEIGHT};

    _gamepad->setJoystick(JOYSTICK, &thumbScreenRegion, &thumbTexRegion, &dockScreenRegion, &dockTexRegion, JOYSTICK_RADIUS);

	Gamepad::Rect regionOnScreen = {SCREEN_WIDTH - SCALE_FACTOR * (BUTTON_SCREEN_X + BUTTON_WIDTH), SCREEN_HEIGHT - BUTTON_SCREEN_Y * SCALE_FACTOR, BUTTON_WIDTH * SCALE_FACTOR, BUTTON_HEIGHT * SCALE_FACTOR};
	Gamepad::Rect releasedRegion = {BUTTON_RELEASED_X, BUTTON_RELEASED_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
	Gamepad::Rect pressedRegion =  {BUTTON_PRESSED_X, BUTTON_PRESSED_Y, BUTTON_WIDTH, BUTTON_HEIGHT};

	_gamepad->setButton(BUTTON_1, &regionOnScreen, &releasedRegion, &pressedRegion);
}

void CharacterGame::initializeMaterial(Scene* scene, Node* node, Material* material)
{
    // Bind light shader parameters to dynamic objects only
    if (material)
    {
        Node* lightNode = scene->findNode("sun");
        material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
        material->getParameter("u_lightColor")->bindValue(lightNode->getLight(), &Light::getColor);
        material->getParameter("u_ambientColor")->bindValue(scene, &Scene::getAmbientColor);
    }
}

bool CharacterGame::initScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    std::string id(node->getId());
    if (model)
    {
        if (model->getMaterial() &&
            id.find("wall") == id.npos &&
            id.find("basketballnet") == id.npos &&
            id.find("backboard") == id.npos &&
            id.find("easel") == id.npos &&
            id.find("floor") == id.npos &&
            id.find("storageorganizer") == id.npos &&
            id.find("book") == id.npos &&
            id.find("toybox") == id.npos &&
            id.find("table") == id.npos)
        {
            initializeMaterial(_scene, node, model->getMaterial());
        }
    }

    return true;
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

void CharacterGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->begin();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
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
    Vector2 joystickVec = _gamepad->getJoystickState(JOYSTICK);
    if (!joystickVec.isZero())
    {
	    inputFlags = 0;

        velocityNS = joystickVec.y;

        // Calculate forward/backward movement.
        if (velocityNS > 0)
		    inputFlags |= 1;
	    else if (velocityNS < 0)
		    inputFlags |= 2;
        
        // Calculate rotation
        float angle = joystickVec.x * MATH_PI * -0.015;
        _character->rotate(Vector3::unitY(), angle);
    }

    /*Gamepad::ButtonState buttonOneState = _gamepad->getButtonState(BUTTON_1);
    if (buttonOneState)
    {
        inputFlags = 16;
    }

    if (inputFlags == 16)
    {
        play("jump", false);
    }*/

    /*if (inputFlags == 0)
    {
        play("idle", true);
        _character->setForwardVelocity(0);
        _character->setRightVelocity(0);
    }
    else*/
    {
        if (inputFlags & NORTH)
        {
            if (inputFlags & RUNNING)
            {
                play("running", true);
                _character->setForwardVelocity(RUN_SPEED);
            }
            else
            {
                play("walking", true);
                _character->setForwardVelocity(WALK_SPEED);
            }

            if (inputFlags & EAST)
                _character->setRightVelocity(STRAFE_SPEED);
            else if (inputFlags & WEST)
                _character->setRightVelocity(-STRAFE_SPEED);
            else
                _character->setRightVelocity(0);
        }
        else if (inputFlags & SOUTH)
        {
            play("walking", true);
            _character->setForwardVelocity(-WALK_SPEED);

            if (inputFlags & EAST)
                _character->setRightVelocity(STRAFE_SPEED);
            else if (inputFlags & WEST)
                _character->setRightVelocity(-STRAFE_SPEED);
            else
                _character->setRightVelocity(0);
        }
        else if (inputFlags & EAST)
        {
            play("strafeRight", true);
            _character->setForwardVelocity(0);
            _character->setRightVelocity(STRAFE_SPEED);
        }
        else if (inputFlags & WEST)
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

    // Draw our scene
    _scene->visit(this, &CharacterGame::drawScene, (void*)0);
    _scene->visit(this, &CharacterGame::drawScene, (void*)1);

    switch (drawDebug)
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

    _gamepad->draw();

    _font->begin();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->end();
}

bool CharacterGame::drawScene(Node* node, void* cookie)
{
    Model* model = node->getModel();
    if (model)
    {
        switch ((long)cookie)
        {
        case OPAQUE_OBJECTS:
            if (!node->isTransparent())
                model->draw();
            break;

        case TRANSPARENT_OBJECTS:
            if (node->isTransparent())
                model->draw();
            break;
        }
    }

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
            inputFlags |= NORTH;
            velocityNS = 1.0f;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            inputFlags |= SOUTH;
            velocityNS = -1.0f;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            inputFlags |= WEST;
            velocityEW = 1.0f;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            inputFlags |= EAST;
            velocityEW = -1.0f;
            break;
        case Keyboard::KEY_B:
            drawDebug++;
            if (drawDebug > 3)
                drawDebug = 0;
            break;
        case Keyboard::KEY_SPACE:
            play("jump", false);
            _character->jump(3.0f);
            break;
        case Keyboard::KEY_SHIFT:
            inputFlags |= RUNNING;
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            inputFlags &= ~NORTH;
            velocityNS = 0.0f;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
            inputFlags &= ~SOUTH;
            velocityNS = 0.0f;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_CAPITAL_A:
            inputFlags &= ~WEST;
            velocityEW = 0.0f;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_CAPITAL_D:
            inputFlags &= ~EAST;
            velocityEW = 0.0f;
            break;
        case Keyboard::KEY_SHIFT:
            inputFlags &= ~RUNNING;
            break;
        }
    }
}

void CharacterGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // Get the joystick's current state.
    bool wasActive = _gamepad->isJoystickActive(JOYSTICK);

    _gamepad->touchEvent(evt, x, y, contactIndex);

    // See if the joystick is still active.
    bool isActive = _gamepad->isJoystickActive(JOYSTICK);
    if (!isActive)
    {
        // If it was active before, reset the joystick's influence on the inputFlags.
        if (wasActive)
            inputFlags = 0;

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
