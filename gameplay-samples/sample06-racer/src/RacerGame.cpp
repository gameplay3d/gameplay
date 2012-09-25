#include "RacerGame.h"

// Render queue indexes (in order of drawing).
enum RenderQueue
{
    QUEUE_OPAQUE = 0,
    QUEUE_TRANSPARENT,
    QUEUE_COUNT
};

bool __viewFrustumCulling = true;
bool __flythruCamera = false;
bool __drawDebug = false;
bool __useAccelerometer = false;
bool __showMenu = false;

// Declare our game instance
RacerGame game;

// Input bit-flags (powers of 2)
#define ACCELERATOR (1 << 0)
#define BRAKE (1 << 1)
#define REVERSE (1 << 2)
#define UPRIGHT (1 << 3)
#define STEER_LEFT (1 << 4)
#define STEER_RIGHT (1 << 5)

#define STEERING_RESPONSE (7.0f)

#define BUTTON_A (_gamepad->isVirtual() ? 0 : 10)
#define BUTTON_B (_gamepad->isVirtual() ? 1 : 11)
#define BUTTON_X (12)
#define BUTTON_Y (13)

// Aerodynamic downforce effect
#define AIR_DENSITY (1.2f)
#define KPH_TO_MPS (1.0f / 3.6f)
#define REF_AREA (7.5f)
#define LIFT_COEFF (0.6f)
#define DOWNFORCE_LUMPED (0.5f * AIR_DENSITY * KPH_TO_MPS * KPH_TO_MPS * REF_AREA * LIFT_COEFF)

RacerGame::RacerGame()
    : _scene(NULL), _keyFlags(0), _mouseFlags(0), _steering(0), _gamepad(NULL), _carVehicle(NULL),
    _backgroundSound(NULL), _engineSound(NULL), _brakingSound(NULL), _carSpeedLag(0)
{
}

void RacerGame::initialize()
{
    //setVsync(false);

    setMultiTouch(true);

    _font = Font::create("res/common/arial40.gpb");

    // Display the gameplay splash screen during loading, for at least 1 second.
    displayScreen(this, &RacerGame::drawSplash, NULL, 1000L);

    // Create the menu and start listening to its controls.
    _menu = Form::create("res/common/menu.form");
    static_cast<Button*>(_menu->getControl("resetButton"))->addListener(this, Listener::CLICK);
    static_cast<Button*>(_menu->getControl("exitButton"))->addListener(this, Listener::CLICK);
    static_cast<RadioButton*>(_menu->getControl("useGamepad"))->addListener(this, Listener::VALUE_CHANGED);
    static_cast<RadioButton*>(_menu->getControl("useTilt"))->addListener(this, Listener::VALUE_CHANGED);

    // Load the scene
    _scene = Scene::load("res/common/game.scene");

    // Set the aspect ratio for the scene's camera to match the current resolution
    _scene->getActiveCamera()->setAspectRatio((float)getWidth() / (float)getHeight());

    // Initialize scene
    _scene->visit(this, &RacerGame::initializeScene);

    // Initialize the gamepad
    _gamepad = getGamepad(0);

    // Load and initialize game script
    getScriptController()->loadScript("res/common/game.lua");
    getScriptController()->executeFunction<void>("setScene", "<Scene>", _scene);

    _virtualGamepad = getGamepad(0);
    Form* gamepadForm = _virtualGamepad->getForm();

    float from = 0.0f;
    float to = getHeight();
    Animation* virtualGamepadAnimation = gamepadForm->createAnimationFromTo("gamepad_transition", Form::ANIMATE_POSITION_Y, &from, &to, Curve::LINEAR, 600L);
    _virtualGamepadClip = virtualGamepadAnimation->getClip();

    Node* carNode = _scene->findNode("carbody");
    if (carNode && carNode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE)
    {
        _carVehicle = static_cast<PhysicsVehicle*>(carNode->getCollisionObject());
        resetVehicle();
    }

    // Create audio tracks
    _backgroundSound = AudioSource::create("res/common/background_track.ogg");
    if (_backgroundSound)
    {
        _backgroundSound->setLooped(true);
        _backgroundSound->play();
        _backgroundSound->setGain(0.3f);
    }

    _engineSound = AudioSource::create("res/common/engine_loop.ogg");
    if (_engineSound)
    {
        _engineSound->setLooped(true);
        _engineSound->play();
        _engineSound->setGain(0.5f);
    }

    _brakingSound = AudioSource::create("res/common/braking.ogg");
    _brakingSound->setLooped(false);
    _brakingSound->setGain(10.0f);
}

bool RacerGame::initializeScene(Node* node)
{
    static Node* lightNode = _scene->findNode("directionalLight1");

    Model* model = node->getModel();
    if (model)
    {
        Material* material = model->getMaterial();
        if (material && material->getTechnique()->getPassByIndex(0)->getEffect()->getUniform("u_lightDirection"))
        {
            material->getParameter("u_ambientColor")->setValue(_scene->getAmbientColor());
            material->getParameter("u_lightColor")->setValue(lightNode->getLight()->getColor());
            material->getParameter("u_lightDirection")->setValue(lightNode->getForwardVectorView());
        }
    }

    return true;
}

void RacerGame::finalize()
{
    SAFE_RELEASE(_backgroundSound);
    SAFE_RELEASE(_engineSound);
    SAFE_RELEASE(_brakingSound);
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void RacerGame::update(float elapsedTime)
{
    // Check if we have any physical gamepad connections.
    getGamepadsConnected();
    
    _gamepad->update(elapsedTime);

	_menu->update(Game::getAbsoluteTime());

    Node* cameraNode;
    if (_scene->getActiveCamera() && (cameraNode = _scene->getActiveCamera()->getNode()))
    {
        float dt = elapsedTime / 1000.0f;
        float braking = 0;
        float driving = 0;

        if (!__flythruCamera && _carVehicle)
        {
            // Vehicle Control (Normal Mode)
            Vector2 direction;
            if (_gamepad->isJoystickActive(0))
            {
                _gamepad->getJoystickAxisValues(0, &direction);
            }

            // Allow keys to control steering
            if (_keyFlags & STEER_LEFT)
            {
                _steering += STEERING_RESPONSE * dt;
            }
            else if (_keyFlags & STEER_RIGHT)
            {
                _steering -= STEERING_RESPONSE * dt;
            }
            else if (__useAccelerometer)
            {
            	float pitch, roll;
            	Game::getAccelerometerValues(&pitch, &roll);

            	_steering = -0.16 * roll;
            }
            else
            {
                _steering = -direction.x;
            }
            _steering = max(-1.0f, min(_steering, 1.0f));

            if ( (_keyFlags & ACCELERATOR) || (_gamepad->getButtonState(BUTTON_A) == Gamepad::BUTTON_PRESSED) )
            {
                driving = 1;
                _engineSound->setGain(1.0f);
            }
            else
            {
                _engineSound->setGain(0.8f);
            }
            float s = _carSpeedLag / 100.0f;
            _engineSound->setPitch(max(0.2f, min(s, 2.0f)));

            // Reverse only below a reasonable speed
            bool isReverseCommanded = (_keyFlags & REVERSE) ||
                                      (!_gamepad->isVirtual() && _gamepad->getButtonState(BUTTON_X) == Gamepad::BUTTON_PRESSED) ||
                                      (direction.y < -0.1 && _gamepad->getButtonState(BUTTON_A) == Gamepad::BUTTON_PRESSED);
            if (isReverseCommanded && _carVehicle->getSpeedKph() < 30.0f)
            {
                driving = -0.6f;
            }

            if ( (_keyFlags & BRAKE) || _gamepad->getButtonState(BUTTON_B) == Gamepad::BUTTON_PRESSED)
            {
                braking = 1;
                if (_brakingSound && _brakingSound->getState() != AudioSource::PLAYING && _carVehicle->getSpeedKph() > 30.0f)
                    _brakingSound->play();
            }
            else
            {
                _brakingSound->stop();
            }

            // Make the camera follow the car
            Node* carNode = _carVehicle->getNode();
            Vector3 carPosition(carNode->getTranslation());
            Vector3 fixedArm(Vector3::unitY()*4.0f - carNode->getBackVector()*10.0f);
            Vector3 swingArm(carPosition, _carPositionPrevious);
            swingArm.y = max(0.0f, swingArm.y);
            swingArm += fixedArm*0.0001f;
            swingArm.normalize();
            Vector3 commandedPosition(carPosition + fixedArm + swingArm*5.0f);
            cameraNode->translate((commandedPosition - cameraNode->getTranslation()) * (5.0f * dt));
            Matrix m;
            Matrix::createLookAt(cameraNode->getTranslation(), carPosition, Vector3::unitY(), &m);
            m.transpose();
            Quaternion q;
            m.getRotation(&q);
            cameraNode->setRotation(q);
            _carPositionPrevious.set(carPosition);
        }

        if (_carVehicle)
        {
            float v = _carVehicle->getSpeedKph();

            // Aerodynamic downforce
            float delta = v - _carSpeedLag;
            _carSpeedLag += delta * dt / (dt + (delta > 0 ? 0 : 1.2));
            _carVehicle->getRigidBody()->applyForce(Vector3(0, -DOWNFORCE_LUMPED * _carSpeedLag * _carSpeedLag, 0));

            // Reduce control authority with speed
            float blowdown = max(_gamepad->isVirtual() ? 0.15f : 0.22f, 1 - 0.009f*fabs(v));
            float throttleBack = (braking > 0) ? 0 : max(0.0f, v - 105.0f) / 75.0f;
            throttleBack *= throttleBack;
            float brakeBack = max(0.0f, v - 100.0f) / 70.0f;
            brakeBack *= brakeBack;

            // Increase engine bottom-end
            float boost = max(1.0f, 2.6f * (1.0f - fabs(v)/120.0f));

            _carVehicle->update(elapsedTime, blowdown*_steering, max(0.0f, braking - brakeBack), boost*driving - throttleBack);

            if ( (_keyFlags & UPRIGHT) ||
                 (!_gamepad->isVirtual() && _gamepad->getButtonState(BUTTON_Y) == Gamepad::BUTTON_PRESSED) ||
                 (_carVehicle->getNode()->getTranslationY() < -300.0f) )
            {
                resetVehicle();
            }
        }
    }
}

void RacerGame::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene to build our render queues
    for (unsigned int i = 0; i < QUEUE_COUNT; ++i)
        _renderQueues[i].clear();
    _scene->visit(this, &RacerGame::buildRenderQueues);

    // Draw the scene from our render queues
    drawScene();

    if (__drawDebug)
    {
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
    }

    // Draw the gamepad
    _virtualGamepad->draw();

    // Draw the menu
    if (__showMenu)
    {
        _menu->draw();
    }
        
    // Draw FPS and speed
    int carSpeed = _carVehicle ? (int)_carVehicle->getSpeedKph() : 0;
    _font->start();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(0,0.5f,1,1), 20);
    char kph[32];
    sprintf(kph, "%d [km/h]", carSpeed);
    _font->drawText(kph, getWidth() / 2 - 50, getHeight() - 60, Vector4(1,1,1,1), 40);
    _font->finish();
}

bool RacerGame::buildRenderQueues(Node* node)
{
    Model* model = node->getModel(); 
    if (model)
    {
        // Perform view-frustum culling for this node
        if (__viewFrustumCulling && !node->getBoundingSphere().intersects(_scene->getActiveCamera()->getFrustum()))
            return true;

        // Determine which render queue to insert the node into
        std::vector<Node*>* queue;
        if (node->hasTag("transparent"))
            queue = &_renderQueues[QUEUE_TRANSPARENT];
        else
            queue = &_renderQueues[QUEUE_OPAQUE];

        queue->push_back(node);
    }
    return true;
}

void RacerGame::drawScene()
{
    // Iterate through each render queue and draw the nodes in them
    for (unsigned int i = 0; i < QUEUE_COUNT; ++i)
    {
        std::vector<Node*>& queue = _renderQueues[i];

        for (unsigned int j = 0, ncount = queue.size(); j < ncount; ++j)
        {
            queue[j]->getModel()->draw();
        }
    }
}

void RacerGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);

    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}

void RacerGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            menuEvent();
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
            _keyFlags |= ACCELERATOR;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
        case Keyboard::KEY_DOWN_ARROW:
            _keyFlags |= REVERSE;
            break;
        case Keyboard::KEY_SPACE:
            _keyFlags |= BRAKE;
            break;
        case Keyboard::KEY_Y:
        case Keyboard::KEY_CAPITAL_Y:
            _keyFlags |= UPRIGHT;
            break;
        case Keyboard::KEY_V:
            __viewFrustumCulling = !__viewFrustumCulling;
            break;
        case Keyboard::KEY_F:
            __flythruCamera = !__flythruCamera;
            getScriptController()->executeFunction<void>("toggleCamera");
            break;
        case Keyboard::KEY_B:
            __drawDebug = !__drawDebug;
            break;
        case Keyboard::KEY_J:
        	__useAccelerometer = !__useAccelerometer;
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
            _keyFlags &= ~ACCELERATOR;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_CAPITAL_S:
        case Keyboard::KEY_DOWN_ARROW:
            _keyFlags &= ~REVERSE;
            break;
        case Keyboard::KEY_SPACE:
            _keyFlags &= ~BRAKE;
            break;
        case Keyboard::KEY_Y:
        case Keyboard::KEY_CAPITAL_Y:
            _keyFlags &= ~UPRIGHT;
            break;
        }
    }
}

void RacerGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

bool RacerGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    bool consumed = false;

    switch (evt)
    {
    case Mouse::MOUSE_PRESS_LEFT_BUTTON:
        _keyFlags |= ACCELERATOR;
        break;
    case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
        _keyFlags |= BRAKE;
        break;
    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        _keyFlags &= ~ACCELERATOR;
        break;
    case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
        _keyFlags &= ~BRAKE;
        break;
    }

    return consumed;
}

void RacerGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
    case Gamepad::CONNECTED_EVENT:
        if (gamepad->isVirtual())
        {
            _virtualGamepadClip->setSpeed(-1.0f);
        }
        else
        {
            _virtualGamepadClip->setSpeed(1.0f);
        }
        _virtualGamepadClip->play();
        _gamepad = gamepad;

        break;
    case Gamepad::DISCONNECTED_EVENT:
        _virtualGamepadClip->setSpeed(-1.0f);
        _virtualGamepadClip->play();
        _gamepad = getGamepad(0);
        break;
    }
}

void RacerGame::menuEvent()
{
    __showMenu = !__showMenu;

	if (__showMenu)
	{
		pause();
	}
	else
	{
		resume();
	}
}

void RacerGame::resetVehicle()
{
    Node* carNode = _carVehicle->getNode();

    _carVehicle->getRigidBody()->setEnabled(false);
    carNode->setTranslation(-258, 1, 278);
    _carPositionPrevious.set(carNode->getTranslation());
    carNode->setRotation(Vector3::unitY(), MATH_DEG_TO_RAD(143.201f));
    _carVehicle->getRigidBody()->setLinearVelocity(Vector3::zero());
    _carSpeedLag = 0;
    _carVehicle->getRigidBody()->setAngularVelocity(Vector3::zero());
    _carVehicle->getRigidBody()->setEnabled(true);
}

void RacerGame::controlEvent(Control* control, EventType evt)
{
    if (strcmp(control->getId(), "resetButton") == 0)
    {
        resetVehicle();

		// Close the menu and resume the game.
		menuEvent();
    }
    else if (strcmp(control->getId(), "exitButton") == 0)
    {
        exit();
    }
    else if (strcmp(control->getId(), "useGamepad") == 0)
    {
        __useAccelerometer = false;
    }
    else if (strcmp(control->getId(), "useTilt") == 0)
    {
        __useAccelerometer = true;
    }
}
