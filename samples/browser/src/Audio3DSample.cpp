#include "Audio3DSample.h"
#include "Grid.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Audio", "3D Audio", Audio3DSample, 1);
#endif

static const unsigned int MOVE_FORWARD = 1;
static const unsigned int MOVE_BACKWARD = 2;
static const unsigned int MOVE_LEFT = 4;
static const unsigned int MOVE_RIGHT = 8;
static const unsigned int MOVE_UP = 16;
static const unsigned int MOVE_DOWN = 32;

static const float MOVE_SPEED = 15.0f;
static const float UP_DOWN_SPEED = 10.0f;

Audio3DSample::Audio3DSample()
    : _font(NULL), _scene(NULL), _cubeNode(NULL), _gamepad(NULL), _moveFlags(0), _prevX(0), _prevY(0), _buttonPressed(false)
{
}

void Audio3DSample::initialize()
{
    setMultiTouch(true);
    _font = Font::create("res/common/arial18.gpb");
    // Load game scene from file
    _scene = Scene::load("res/common/box.gpb");

    // Get light node
    Node* lightNode = _scene->findNode("directionalLight1");
    Light* light = lightNode->getLight();

    // Initialize box model
    Node* boxNode = _scene->findNode("box");
    Model* boxModel = boxNode->getModel();
    Material* boxMaterial = boxModel->setMaterial("res/common/box.material");
    boxMaterial->getParameter("u_lightColor")->setValue(light->getColor());
    boxMaterial->getParameter("u_lightDirection")->setValue(lightNode->getForwardVectorView());

    // Remove the cube from the scene but keep a reference to it.
    _cubeNode = boxNode;
    _cubeNode->addRef();
    _scene->removeNode(_cubeNode);

    loadGrid(_scene);

    // Initialize cameraa
    Vector3 cameraPosition(5, 5, 1);
    if (Camera* camera = _scene->getActiveCamera())
    {
        camera->getNode()->getTranslation(&cameraPosition);
    }

    _fpCamera.initialize();
    _fpCamera.setPosition(cameraPosition);
    _scene->addNode(_fpCamera.getRootNode());
    _scene->setActiveCamera(_fpCamera.getCamera());

    _gamepad = getGamepad(0);
    // This is needed because the virtual gamepad is shared between all samples.
    // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
    if (_gamepad && _gamepad->isVirtual())
        _gamepad->getForm()->setEnabled(true);
}

void Audio3DSample::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_cubeNode);
    for (std::map<std::string, Node*>::iterator it = _audioNodes.begin(); it != _audioNodes.end(); ++it)
    {
        it->second->release();
    }
    _audioNodes.clear();
}

void Audio3DSample::update(float elapsedTime)
{
    float time = (float)elapsedTime / 1000.0f;

    Vector2 move;

    if (_moveFlags != 0)
    {
        // Forward motion
        if (_moveFlags & MOVE_FORWARD)
        {
            move.y = 1;
        }
        else if (_moveFlags & MOVE_BACKWARD)
        {
            move.y = -1;
        }
        // Strafing
        if (_moveFlags & MOVE_LEFT)
        {
            move.x = 1;
        }
        else if (_moveFlags & MOVE_RIGHT)
        {
            move.x = -1;
        }
        move.normalize();

        // Up and down
        if (_moveFlags & MOVE_UP)
        {
            _fpCamera.moveUp(time * UP_DOWN_SPEED);
        }
        else if (_moveFlags & MOVE_DOWN)
        {
            _fpCamera.moveDown(time * UP_DOWN_SPEED);
        }
    }
    else if (_gamepad->getJoystickCount() > 0)
    {
        _gamepad->getJoystickValues(0, &move);
        move.x = -move.x;
    }
    if (_gamepad->getJoystickCount() > 1)
    {
        Vector2 joy2;
        _gamepad->getJoystickValues(1, &joy2);
        _fpCamera.rotate(MATH_DEG_TO_RAD(joy2.x * 2.0f), MATH_DEG_TO_RAD(joy2.y * 2.0f));
    }

    if (!move.isZero())
    {
        move.scale(time * MOVE_SPEED);
        _fpCamera.moveForward(move.y);
        _fpCamera.moveLeft(move.x);
    }

    if (!_buttonPressed && _gamepad->isButtonDown(Gamepad::BUTTON_A))
    {
        addSound("footsteps.wav");
    }
    _buttonPressed = _gamepad->isButtonDown(Gamepad::BUTTON_A);
}

void Audio3DSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Audio3DSample::drawScene);

    drawDebugText(0, _font->getSize());

    _gamepad->draw();
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

bool Audio3DSample::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw();
    }
    return true;
}

void Audio3DSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        _prevX = x;
        _prevY = y;
        break;
    case Touch::TOUCH_RELEASE:
        _prevX = 0;
        _prevY = 0;
        break;
    case Touch::TOUCH_MOVE:
    {
        int deltaX = x - _prevX;
        int deltaY = y - _prevY;
        _prevX = x;
        _prevY = y;
        float pitch = -MATH_DEG_TO_RAD(deltaY * 0.5f);
        float yaw = MATH_DEG_TO_RAD(deltaX * 0.5f);
        _fpCamera.rotate(yaw, pitch);
        break;
    }   
    };
}

void Audio3DSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags |= MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags |= MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags |= MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags |= MOVE_RIGHT;
            break;

        case Keyboard::KEY_Q:
            _moveFlags |= MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags |= MOVE_UP;
            break;
        case Keyboard::KEY_PG_UP:
            _fpCamera.rotate(0, MATH_PIOVER4);
            break;
        case Keyboard::KEY_PG_DOWN:
            _fpCamera.rotate(0, -MATH_PIOVER4);
            break;

        case Keyboard::KEY_ONE:
        case Keyboard::KEY_SPACE:
            addSound("footsteps.wav");
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags &= ~MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags &= ~MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags &= ~MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags &= ~MOVE_RIGHT;
            break;
        case Keyboard::KEY_Q:
            _moveFlags &= ~MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags &= ~MOVE_UP;
            break;
        }
    }
}

bool Audio3DSample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_WHEEL:
        _fpCamera.moveForward(wheelDelta * MOVE_SPEED / 2.0f );
        return true;
    }
    return false;
}

void Audio3DSample::addSound(const std::string& file)
{
    std::string path("res/common/");
    path.append(file);

    Node* node = NULL;
    std::map<std::string, Node*>::iterator it = _audioNodes.find(path);
    if (it != _audioNodes.end())
    {
        node = it->second->clone();
    }
    else
    {
        AudioSource* audioSource = AudioSource::create(path.c_str());
        assert(audioSource);
        audioSource->setLooped(true);

        node = _cubeNode->clone();
        node->setId(file.c_str());
        node->setAudioSource(audioSource);
        audioSource->release();
        
        _audioNodes[path] = node;
        node->addRef();
    }
    assert(node);
    Node* cameraNode = _scene->getActiveCamera()->getNode();
    // Position the sound infront of the user
    node->setTranslation(cameraNode->getTranslationWorld());
    Vector3 dir = cameraNode->getForwardVectorWorld().normalize();
    dir.scale(2);
    node->translate(dir);
    _scene->addNode(node);
    node->getAudioSource()->play();
    node->release();
}

void Audio3DSample::drawDebugText(int x, int y)
{
    _font->start();
    static const int V_SPACE = 16;
    AudioListener* audioListener = AudioListener::getInstance();
    drawVector3("Position", audioListener->getPosition(), x, y);
    drawVector3("Forward", audioListener->getOrientationForward(), x, y+=V_SPACE);
    drawVector3("Orientation", audioListener->getOrientationUp(), x, y+=V_SPACE);
    drawVector3("Velocity", audioListener->getVelocity(), x, y+=V_SPACE);
    _font->finish();
}

void Audio3DSample::drawVector3(const char* str, const Vector3 vector, int x, int y)
{
    char buffer[255];
    sprintf(buffer, "%s: (%f, %f, %f)", str, vector.x, vector.y, vector.z);
    _font->drawText(buffer, x, y, Vector4::one(), _font->getSize());
}

void Audio3DSample::loadGrid(Scene* scene)
{
    assert(scene);
    Model* gridModel = createGridModel();
    assert(gridModel);
    gridModel->setMaterial("res/common/grid.material");
    Node* node = scene->addNode("grid");
    node->setModel(gridModel);
    gridModel->release();
}

void Audio3DSample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
    case Gamepad::CONNECTED_EVENT:
    case Gamepad::DISCONNECTED_EVENT:
        _gamepad = getGamepad(0);
        // This is needed because the virtual gamepad is shared between all samples.
        // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
        if (_gamepad && _gamepad->isVirtual())
            _gamepad->getForm()->setEnabled(true);
        break;
    }
}