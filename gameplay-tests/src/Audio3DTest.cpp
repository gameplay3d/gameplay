#include "Audio3DTest.h"
#include "Grid.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Audio", "3D Audio", Audio3DTest, 1);
#endif

static const unsigned int MOVE_FORWARD = 1;
static const unsigned int MOVE_BACKWARD = 2;
static const unsigned int MOVE_LEFT = 4;
static const unsigned int MOVE_RIGHT = 8;
static const unsigned int MOVE_UP = 16;
static const unsigned int MOVE_DOWN = 32;

static const float MOVE_SPEED = 15.0f;
static const float UP_DOWN_SPEED = 10.0f;

// Functions
static void loadGrid(Scene* scene);

/**
 * Returns true if the string ends with the given suffix.
 */
static bool endsWith(const std::string& str, const char* suffix)
{
    size_t suffixLength = strlen(suffix);
    if (suffixLength == 0)
        return true;
    if (str.length() < suffixLength)
        return false;
    return std::equal(str.end() - suffixLength, str.end(), suffix);
}

Audio3DTest::Audio3DTest()
    : _font(NULL), _scene(NULL), _cubeNode(NULL), _moveFlags(0), _prevX(0), _prevY(0)
{
}

void Audio3DTest::initialize()
{
    _font = Font::create("res/common/arial18.gpb");
    // Load game scene from file
    Bundle* bundle = Bundle::create("res/common/box.gpb");
    _scene = bundle->loadScene();
    SAFE_RELEASE(bundle);

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

    std::vector<std::string> files;
    FileSystem::listFiles("res/common", files);
    for (std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i)
    {
        if (endsWith(*i, ".wav") || endsWith(*i, ".ogg"))
        {
            _soundFiles.push_back(*i);
        }
    }
}

void Audio3DTest::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_cubeNode);
    for (NodeMap::iterator it = _audioNodes.begin(); it != _audioNodes.end(); ++it)
    {
        it->second->release();
    }
    _audioNodes.clear();
}

void Audio3DTest::update(float elapsedTime)
{
    float time = (float)elapsedTime / 1000.0f;

    if (_moveFlags != 0)
    {
        Vector2 move;
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

        move.normalize().scale(time * MOVE_SPEED);
        _fpCamera.moveForward(move.y);
        _fpCamera.moveLeft(move.x);

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
}

void Audio3DTest::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Audio3DTest::drawScene);

    drawDebugText();
}

bool Audio3DTest::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw();
    }
    return true;
}

void Audio3DTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
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

void Audio3DTest::keyEvent(Keyboard::KeyEvent evt, int key)
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
            addSound(_soundFiles[0]);
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

bool Audio3DTest::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_WHEEL:
        _fpCamera.moveForward(wheelDelta * MOVE_SPEED / 2.0f );
        return true;
    }
    return false;
}

void Audio3DTest::addSound(const std::string& file)
{
    std::string path("res/common/");
    path.append(file);

    Node* node = NULL;
    NodeMap::iterator it = _audioNodes.find(path);
    if (it != _audioNodes.end())
    {
        node = it->second->clone();
    }
    else
    {
        AudioSource* audioSource = AudioSource::create(path.c_str());
        assert(audioSource);

        node = _cubeNode->clone();
        node->setId(file.c_str());
        node->setAudioSource(audioSource);
        audioSource->release();
        
        _audioNodes[path] = node;
        node->addRef();
    }
    assert(node);
    node->setTranslation(_scene->getActiveCamera()->getNode()->getTranslationWorld());
    _scene->addNode(node);
    node->getAudioSource()->play();
    node->release();
}

void Audio3DTest::drawDebugText()
{
    _font->start();
    static const int V_SPACE = 16;
    int y = 0;
    AudioListener* audioListener = AudioListener::getInstance();
    drawVector3("Listener Position", audioListener->getPosition(), 0, 0);
    drawVector3("Listener Forward", audioListener->getOrientationForward(), 0, y+=V_SPACE);
    drawVector3("Listener Up", audioListener->getOrientationUp(), 0, y+=V_SPACE);
    drawVector3("Listener Velocity", audioListener->getVelocity(), 0, y+=V_SPACE);
    _font->finish();
}

void Audio3DTest::drawVector3(const char* str, const Vector3 vector, int x, int y)
{
    char buffer[255];
    sprintf(buffer, "%s: (%f, %f, %f)", str, vector.x, vector.y, vector.z);
    _font->drawText(buffer, x, y, Vector4::one(), _font->getSize());
}

// Functions

void loadGrid(Scene* scene)
{
    assert(scene);
    Model* gridModel = createGridModel();
    assert(gridModel);
    gridModel->setMaterial("res/common/grid.material");
    Node* node = scene->addNode("grid");
    node->setModel(gridModel);
    gridModel->release();
}
