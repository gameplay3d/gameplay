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
bool __menuFlag = false;

// Declare our game instance
RacerGame game;

// Input bit-flags (powers of 2)
#define ACCELERATOR (1 << 0)
#define BRAKE (1 << 1)
#define REVERSE (1 << 2)
#define UPRIGHT (1 << 3)
#define STEER_LEFT (1 << 4)
#define STEER_RIGHT (1 << 5)
#define ACCELERATOR_MOUSE (1 << 6)
#define BRAKE_MOUSE (1 << 7)

#define STEERING_RESPONSE (7.0f)

RacerGame::RacerGame()
    : _scene(NULL), _keyFlags(0), _mouseFlags(0), _steering(0), _gamepad(NULL), _carVehicle(NULL), _upsetTimer(0),
      _backgroundSound(NULL), _engineSound(NULL), _brakingSound(NULL)
{
}

void RacerGame::initialize()
{
    setMultiTouch(true);

    _font = Font::create("res/common/arial40.gpb");

    // Display the gameplay splash screen during loading, for at least 1 second.
    displayScreen(this, &RacerGame::drawSplash, NULL, 1000L);

    // Create the menu and start listening to its controls.
    _menu = Form::create("res/common/menu.form");
    _menu->setEnabled(false);
    static_cast<Button*>(_menu->getControl("newGameButton"))->addListener(this, Listener::CLICK);
    static_cast<Button*>(_menu->getControl("quitGameButton"))->addListener(this, Listener::CLICK);
    static_cast<RadioButton*>(_menu->getControl("useGamepad"))->addListener(this, Listener::VALUE_CHANGED);
    static_cast<RadioButton*>(_menu->getControl("useTilt"))->addListener(this, Listener::VALUE_CHANGED);
    if (!canExit())
    {
        // Prevent a programmatic exit on platforms that don't allow it.
        _menu->removeControl("quitGameButton");
    }

    // Create a pause button to display the menu
    _overlay = Form::create("res/common/overlay.form");
    static_cast<Button*>(_overlay->getControl("menuButton"))->addListener(this, Listener::CLICK);

    // Load the scene
    _scene = Scene::load("res/common/game.scene");

    // Set the aspect ratio for the scene's camera to match the current resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    // Initialize scene
    _scene->visit(this, &RacerGame::initializeScene);

    // Load and initialize game script
    getScriptController()->loadScript("res/common/game.lua");
    getScriptController()->executeFunction<void>("setScene", "<Scene>", _scene);

    Node* carNode = _scene->findNode("carbody");
    if (carNode && carNode->getCollisionObject()->getType() == PhysicsCollisionObject::VEHICLE)
    {
        _carVehicle = static_cast<PhysicsVehicle*>(carNode->getCollisionObject());
        resetToStart();
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
        _engineSound->setGain(0.7f);
    }

    _brakingSound = AudioSource::create("res/common/braking.wav");
    _brakingSound->setLooped(false);
    _brakingSound->setGain(0.5f);

    _gamepad = getGamepad(0);
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
    SAFE_RELEASE(_menu);
    SAFE_RELEASE(_overlay);
}

void RacerGame::update(float elapsedTime)
{   
    // The "Start" button is mapped to MENU2.
    if (!__showMenu && !__menuFlag && _gamepad->isButtonDown(Gamepad::BUTTON_MENU2))
    {
        __menuFlag = true;
        menuEvent();
    }

    if (__menuFlag && !_gamepad->isButtonDown(Gamepad::BUTTON_MENU2))
    {
        __menuFlag = false;
    }

    if (__showMenu && !__menuFlag && _gamepad->isButtonDown(Gamepad::BUTTON_MENU2))
    {
        __menuFlag = true;
        menuEvent();
    }

    Node* cameraNode;
    if (_scene->getActiveCamera() && (cameraNode = _scene->getActiveCamera()->getNode()))
    {
        float dt = elapsedTime / 1000.0f;
        float braking = 0;
        float driving = 0;

        if (_carVehicle)
        {
            float v = _carVehicle->getSpeedKph();
            bool isVirt = _gamepad->isVirtual();

            if (!__flythruCamera)
            {
                // Vehicle Control (Normal Mode)
                Vector2 direction;
                if (_gamepad->getJoystickCount())
                {
                    _gamepad->getJoystickValues(0, &direction);
                }
                
                if (_gamepad->isButtonDown(Gamepad::BUTTON_LEFT))
                {
                    direction.set(-1.0f, 0.0f);
                }
                else if (_gamepad->isButtonDown(Gamepad::BUTTON_RIGHT))
                {
                    direction.set(1.0f, 0.0f);
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

                if (_gamepad->getTriggerCount() > 1)
                {
                    driving = _gamepad->getTriggerValue(1);
                    _engineSound->setGain(0.8f + (driving * 0.2f));
                }
                
                if (!driving && (_keyFlags & ACCELERATOR || _keyFlags & ACCELERATOR_MOUSE || _gamepad->isButtonDown(Gamepad::BUTTON_A)))
                {
                    driving = 1;
                    _engineSound->setGain(1.0f);
                }
                else
                {
                    _engineSound->setGain(0.8f);
                }
                float s = _carVehicle->getSpeedSmoothKph() / 100.0f;
                _engineSound->setPitch(max(0.2f, min(s, 2.0f)));

                // Reverse only below a reasonable speed
                bool isReverseCommanded = (_keyFlags & REVERSE) ||
                                          (!isVirt && _gamepad->isButtonDown(Gamepad::BUTTON_X)) ||
                                          (direction.y < -0.1 && _gamepad->isButtonDown(Gamepad::BUTTON_A));
                if (isReverseCommanded && v < 30.0f)
                {
                    driving = -0.6f;
                }

                if ( (_keyFlags & BRAKE) || (_keyFlags & BRAKE_MOUSE) || _gamepad->isButtonDown(Gamepad::BUTTON_B))
                {
                    braking = 1;
                    if (_brakingSound && (_brakingSound->getState() != AudioSource::PLAYING) && (v > 30.0f))
                        _brakingSound->play();
                }
                else
                {
                    _brakingSound->stop();
                }

                // Make the camera follow the car
                Node* carNode = _carVehicle->getNode();
                Vector3 carPosition(carNode->getTranslation());
                Vector3 commandedPosition(carPosition + Vector3::unitY()*4.0f - carNode->getBackVector()*10.0f);
                cameraNode->translateSmooth(commandedPosition, dt, 0.2f);
                Matrix m;
                Matrix::createLookAt(cameraNode->getTranslation(), carPosition, Vector3::unitY(), &m);
                m.transpose();
                Quaternion q;
                m.getRotation(&q);
                cameraNode->setRotation(q);
            }

            // Slightly different steering gain based on gamepad type.
            _carVehicle->setSteerdown( (isVirt ? 94.0f : 87.0f), (isVirt ? 0.15f : 0.22f) );
            _carVehicle->update(elapsedTime, _steering, braking, driving);

            // Auto-detect an upset car
            if (fabs(v) < 10.0f && isUpset())
            {
                _upsetTimer += dt;
            }
            else
            {
                _upsetTimer = 0;
            }

            if (_upsetTimer > 3.0f)
            {
                _upsetTimer = 0;
                resetInPlace();
            }
            else if ( (_keyFlags & UPRIGHT) ||
                 (!isVirt && _gamepad->isButtonDown(Gamepad::BUTTON_Y)) ||
                 (_carVehicle->getNode()->getTranslationY() < -300.0f) )
            {
                resetToStart();
            }
        }
    }
}

bool RacerGame::isUpset() const
{
    GP_ASSERT(_carVehicle);

    return _carVehicle->getNode()->getUpVector().y < 0.4f;
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
    if (_gamepad && _gamepad->isVirtual())
    	_gamepad->draw();

    // Draw the menu
    if (__showMenu)
    {
        _menu->draw();
    }
    
    _overlay->draw();
        
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

        for (size_t j = 0, ncount = queue.size(); j < ncount; ++j)
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
        _keyFlags |= ACCELERATOR_MOUSE;
        break;
    case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
        _keyFlags |= BRAKE_MOUSE;
        break;
    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        _keyFlags &= ~ACCELERATOR_MOUSE;
        break;
    case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
        _keyFlags &= ~BRAKE_MOUSE;
        break;
    }

    return consumed;
}

void RacerGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    // Prioritise physical gamepads over the virtual one.
    switch(evt)
    {
    case Gamepad::CONNECTED_EVENT:
        if (gamepad->isVirtual())
        {
            float from = 0.0f;
            float to = getHeight();
            Animation* virtualGamepadAnimation = gamepad->getForm()->createAnimationFromTo("gamepad_transition", Form::ANIMATE_POSITION_Y, &from, &to, Curve::LINEAR, 2000L);
            _virtualGamepadClip = virtualGamepadAnimation->getClip();

            _virtualGamepad = gamepad;
        }
        else
        {
            if (!_physicalGamepad)
                _physicalGamepad = gamepad;
        }

        if (_physicalGamepad)
        {
        	if (_virtualGamepadClip && _gamepad == _virtualGamepad)
            {
        		_virtualGamepadClip->setSpeed(1.0f);
                _virtualGamepadClip->play();
            }
            _gamepad = _physicalGamepad;
			if (_virtualGamepad)
			{
				_virtualGamepad->getForm()->setEnabled(false);
			}
        }
        else if (_virtualGamepad)
        {
            if (_gamepad == _physicalGamepad)
            {
        	    _virtualGamepadClip->setSpeed(-1.0f);
                _virtualGamepadClip->play();
            }
            _gamepad = _virtualGamepad;
            _virtualGamepad->getForm()->setEnabled(true);
        }

        break;
    case Gamepad::DISCONNECTED_EVENT:
        if (gamepad == _physicalGamepad)
        {
            _gamepad = _virtualGamepad;
            _physicalGamepad = NULL;

            _virtualGamepadClip->setSpeed(-1.0f);
            _virtualGamepadClip->play();
            _virtualGamepad->getForm()->setEnabled(true);
        }
        break;
    }
}

void RacerGame::menuEvent()
{
    __showMenu = !__showMenu;

    if (__showMenu)
    {
        static_cast<Button*>(_overlay->getControl("menuButton"))->setText("Resume");
        pause();
        _menu->setEnabled(true);
        _menu->setState(Control::FOCUS);
    }
    else
    {
        static_cast<Button*>(_overlay->getControl("menuButton"))->setText("Menu");
        resume();
        _menu->setEnabled(false);
    }
}

void RacerGame::resetToStart()
{
    Vector3 pos(-258, 1, 278);
    Quaternion rot(Vector3::unitY(), MATH_DEG_TO_RAD(143.201f));

    reset(pos, rot);
}

void RacerGame::resetInPlace()
{
    Node* carNode = _carVehicle->getNode();

    Vector3 pos;
    carNode->getTranslation(&pos);
    pos.y += 3.0f;

    float angle = 0;
    Vector3 v;
    carNode->getForwardVector(&v);
    if (v.x*v.x + v.z*v.z > 0)
    {
        angle += atan2(-v.x, -v.z);
    }
    Quaternion rot(Vector3::unitY(), angle);

    reset(pos, rot);
}

void RacerGame::reset(const Vector3& pos, const Quaternion& rot)
{
    Node* carNode = _carVehicle->getNode();

    _carVehicle->setEnabled(false);
    carNode->setTranslation(pos);
    carNode->setRotation(rot);
    _carVehicle->reset();
    _carVehicle->setEnabled(true);
}

void RacerGame::controlEvent(Control* control, EventType evt)
{
    if (strcmp(control->getId(), "newGameButton") == 0)
    {
        resetToStart();
        // Close the menu and resume the game.
        menuEvent();
    }
    else if (strcmp(control->getId(), "quitGameButton") == 0)
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
    else if (strcmp(control->getId(), "menuButton") == 0)
    {
        menuEvent();
    }
}
