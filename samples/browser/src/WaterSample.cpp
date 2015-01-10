#include "WaterSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
ADD_SAMPLE("Graphics", "Water", WaterSample, 13);
#endif

// Camera movement consts
static const float MOUSE_SPEED = 0.125f;
static const float MASS = 1.8f;
static const float FRICTION = 0.9f;
static const float SPEED = 5.5f;
static const unsigned int BUFFER_SIZE = 512;
static const float WATER_OFFSET = 0.55f;

WaterSample::WaterSample()
    : _font(NULL), _scene(NULL), _cameraNode(NULL),
    _reflectCameraNode(NULL), _inputMask(0u), _prevX(0),
     _prevY(0), _waterHeight(0.f), 
     _refractBuffer(NULL), _refractBatch(NULL), _reflectBuffer(NULL), _reflectBatch(NULL), 
     _showBuffers(false), _gamepad(NULL)
{
}

void WaterSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Load the scene file
    _scene = Scene::load("res/common/water/watersample.scene");
    _waterHeight = _scene->findNode("Water")->getTranslationY();

    // Set up a first person style camera
    const Vector3 camStartPosition(0.f, 10.f, -30.f);
    _cameraNode = Node::create("cameraNode");
    _cameraNode->setTranslation(camStartPosition);

    Node* camPitchNode = Node::create();
    Matrix m;
    Matrix::createLookAt(_cameraNode->getTranslation(), Vector3::zero(), Vector3::unitY(), &m);
    camPitchNode->rotate(m);
    _cameraNode->addChild(camPitchNode);
    _scene->addNode(_cameraNode);
    Camera* camera = Camera::createPerspective(45.f, Game::getInstance()->getAspectRatio(), 0.1f, 150.f);
    camPitchNode->setCamera(camera);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);
    SAFE_RELEASE(camPitchNode);

    // Add a second camera do draw the reflections
    _reflectCameraNode = Node::create("reflectCamNode");
    _reflectCameraNode->setTranslation(camStartPosition.x, -camStartPosition.y, camStartPosition.z);
    camPitchNode = Node::create();
    Matrix::createLookAt(_reflectCameraNode->getTranslation(), Vector3::zero(), Vector3::unitY(), &m);
    camPitchNode->rotate(m);
    _reflectCameraNode->addChild(camPitchNode);
    camera = Camera::createPerspective(45.f, Game::getInstance()->getAspectRatio(), 0.1f, 150.f);
    camPitchNode->setCamera(camera);
    SAFE_RELEASE(camera);
    SAFE_RELEASE(camPitchNode);

    // Render buffer and preview for refraction
    _refractBuffer = FrameBuffer::create("refractBuffer", BUFFER_SIZE, BUFFER_SIZE);
    DepthStencilTarget* refractDepthTarget = DepthStencilTarget::create("refractDepth", DepthStencilTarget::DEPTH, BUFFER_SIZE, BUFFER_SIZE);
    _refractBuffer->setDepthStencilTarget(refractDepthTarget);
    SAFE_RELEASE(refractDepthTarget);
    _refractBatch = SpriteBatch::create(_refractBuffer->getRenderTarget()->getTexture());

    // Render buffer and preview for reflection
    _reflectBuffer = FrameBuffer::create("reflectBuffer", BUFFER_SIZE, BUFFER_SIZE);
    DepthStencilTarget* reflectDepthTarget = DepthStencilTarget::create("reflectDepth", DepthStencilTarget::DEPTH, BUFFER_SIZE, BUFFER_SIZE);
    _reflectBuffer->setDepthStencilTarget(reflectDepthTarget);
    SAFE_RELEASE(reflectDepthTarget);
    _reflectBatch = SpriteBatch::create(_reflectBuffer->getRenderTarget()->getTexture());

    // Add a node to provide light direction
    Node* lightNode = Node::create("lightNode");
    lightNode->rotateX(-MATH_DEG_TO_RAD(90));
    _scene->addNode(lightNode);

    // Bind material properties
    Material* groundMaterial = dynamic_cast<Model*>(_scene->findNode("Ground")->getDrawable())->getMaterial();
    groundMaterial->getParameter("u_clipPlane")->bindValue(this, &WaterSample::getClipPlane);
    groundMaterial->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorView);
    auto waterMaterial = dynamic_cast<Model*>(_scene->findNode("Water")->getDrawable())->getMaterial();
    auto refractSampler = Texture::Sampler::create(_refractBuffer->getRenderTarget()->getTexture());
    waterMaterial->getParameter("u_refractionTexture")->setSampler(refractSampler);
    SAFE_RELEASE(refractSampler);
    auto reflectSampler = Texture::Sampler::create(_reflectBuffer->getRenderTarget()->getTexture());
    waterMaterial->getParameter("u_reflectionTexture")->setSampler(reflectSampler);
    SAFE_RELEASE(reflectSampler);
    waterMaterial->getParameter("u_worldViewProjectionReflectionMatrix")->bindValue(this, &WaterSample::getReflectionMatrix);
    waterMaterial->getParameter("u_time")->bindValue(this, &WaterSample::getTime);
    SAFE_RELEASE(lightNode);

    setMouseCaptured(true);

    setMultiTouch(true);
    _gamepad = getGamepad(0);
    if (_gamepad && _gamepad->isVirtual())
        _gamepad->getForm()->setEnabled(true);

}

void WaterSample::finalize()
{
    setMouseCaptured(false);
    SAFE_DELETE(_reflectBatch);
    SAFE_RELEASE(_reflectBuffer);
    SAFE_DELETE(_refractBatch);
    SAFE_RELEASE(_refractBuffer);
    SAFE_RELEASE(_reflectCameraNode);
    SAFE_RELEASE(_cameraNode);
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
}

void WaterSample::update(float elapsedTime)
{
    Vector2 axis;
    const float minVal = 0.1f;
    if (_gamepad && _gamepad->getJoystickCount())
        _gamepad->getJoystickValues(0, &axis);
    
    // Move the camera by applying a force
    Vector3 force;
    if ((_inputMask & MOVE_FORWARD) || axis.y > minVal)
        force += _cameraNode->getFirstChild()->getForwardVectorWorld();
    if (_inputMask & MOVE_BACKWARD || axis.y < -minVal)
        force -= _cameraNode->getFirstChild()->getForwardVectorWorld();
    if (_inputMask & MOVE_LEFT || axis.x < -minVal)
        force += _cameraNode->getRightVectorWorld();
    if (_inputMask & MOVE_RIGHT || axis.y > minVal)
        force -= _cameraNode->getRightVectorWorld();
    if (force.lengthSquared() > 1.f) force.normalize();

    _cameraAcceleration += force / MASS;
    _cameraAcceleration *= FRICTION;
    if (_cameraAcceleration.lengthSquared() < 0.01f)
        _cameraAcceleration = Vector3::zero();
    _cameraNode->translate(_cameraAcceleration * SPEED * (elapsedTime / 1000.f));
    
    // Make sure the reflection camera follows
    Vector3 position = _cameraNode->getTranslation();
    position.y = -position.y + _waterHeight * 2.f;
    _reflectCameraNode->setTranslation(position);
}

void WaterSample::render(float elapsedTime)
{
    const Vector4 clearColour(0.84f, 0.89f, 1.f, 1.f);
    
    // Update the refract buffer
    FrameBuffer* defaultBuffer = _refractBuffer->bind();
    Rectangle defaultViewport = getViewport();
    setViewport(Rectangle(BUFFER_SIZE, BUFFER_SIZE));
    _clipPlane.y = -1.f;
    _clipPlane.w = _waterHeight + WATER_OFFSET;
    clear(CLEAR_COLOR_DEPTH, clearColour, 1.0f, 0);
    _scene->visit(this, &WaterSample::drawScene, false);

    // Switch plane direction and camera, and update reflection buffer
    _reflectBuffer->bind();
    _clipPlane.y = 1.f;
    _clipPlane.w = -_waterHeight + WATER_OFFSET;
    Camera* defaultCamera = _scene->getActiveCamera();
    _scene->setActiveCamera(_reflectCameraNode->getFirstChild()->getCamera());

    // Store the mvp matrix here as it is only valid when reflect camera is active
    m_worldViewProjectionReflection = _scene->findNode("Water")->getWorldViewProjectionMatrix();
    clear(CLEAR_COLOR_DEPTH, clearColour, 1.0f, 0);
    _scene->visit(this, &WaterSample::drawScene, false);

    // Draw the final scene
    defaultBuffer->bind();
    setViewport(defaultViewport);
    _clipPlane = Vector4::zero();
    _scene->setActiveCamera(defaultCamera);
    clear(CLEAR_COLOR_DEPTH, clearColour, 1.0f, 0);
    _scene->visit(this, &WaterSample::drawScene, true);

    // Draw preview if enabled
    if (_showBuffers)
    {
        _refractBatch->start();
        _refractBatch->draw(Vector3(0.f, 4.f, 0.f), Rectangle(BUFFER_SIZE, BUFFER_SIZE), Vector2(426.f, 240.f));
        _refractBatch->finish();
        _reflectBatch->start();
        _reflectBatch->draw(Vector3(430.f, 4.f, 0.f), Rectangle(BUFFER_SIZE, BUFFER_SIZE), Vector2(426.f, 240.f));
        _reflectBatch->finish();
    }

    // Draw the gamepad
    _gamepad->draw();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void WaterSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle buffer preview if the user touches the top left corner
            _showBuffers = !_showBuffers;
        }
        _prevX = x;
        _prevY = y;
        break;
    case Touch::TOUCH_RELEASE:
        _prevX = x;
        _prevY = y;
        break;
    case Touch::TOUCH_MOVE:
    {
        int deltaX = x - _prevX;
        int deltaY = y - _prevY;
        _prevX = x;
        _prevY = y;
        
        float xMovement = MATH_DEG_TO_RAD(deltaX * 0.25f);
        float yMovement = MATH_DEG_TO_RAD(-deltaY * 0.25f);
        _cameraNode->rotateY(-xMovement);
        _cameraNode->getFirstChild()->rotateX(yMovement);
        _reflectCameraNode->rotateY(-xMovement);
        _reflectCameraNode->getFirstChild()->rotateX(-yMovement);
    }
        break;
    };
}

void WaterSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_SPACE:
            _showBuffers = !_showBuffers;
            break;
        case Keyboard::KEY_W:
        case Keyboard::KEY_UP_ARROW:
            _inputMask |= MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_DOWN_ARROW:
            _inputMask |= MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_LEFT_ARROW:
            _inputMask |= MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_RIGHT_ARROW:
            _inputMask |= MOVE_RIGHT;
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_UP_ARROW:
            _inputMask &= ~MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
        case Keyboard::KEY_DOWN_ARROW:
            _inputMask &= ~MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
        case Keyboard::KEY_LEFT_ARROW:
            _inputMask &= ~MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
        case Keyboard::KEY_RIGHT_ARROW:
            _inputMask &= ~MOVE_RIGHT;
            break;
        }
    }
}

bool WaterSample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_MOVE:
    {
        float xMovement = MATH_DEG_TO_RAD(-x * MOUSE_SPEED);
        float yMovement = MATH_DEG_TO_RAD(-y * MOUSE_SPEED);
        _cameraNode->rotateY(xMovement);
        _cameraNode->getFirstChild()->rotateX(yMovement);
        _reflectCameraNode->rotateY(xMovement);
        _reflectCameraNode->getFirstChild()->rotateX(-yMovement);
    }
        return true;
    case Mouse::MOUSE_PRESS_LEFT_BUTTON:
        _inputMask |= MOVE_FORWARD;
        return true;
    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        _inputMask &= ~MOVE_FORWARD;
        return true;
    case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
        _inputMask |= MOVE_BACKWARD;
        return true;
    case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
        _inputMask &= ~MOVE_BACKWARD;
        return true;
    default: return false;
    }
    return false;
}

void WaterSample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch (evt)
    {
    case Gamepad::CONNECTED_EVENT:
    case Gamepad::DISCONNECTED_EVENT:
        _gamepad = getGamepad(0);
        if (_gamepad && _gamepad->isVirtual())
            _gamepad->getForm()->setEnabled(true);
        break;
    }
}

bool WaterSample::drawScene(Node* node, bool drawWater)
{
    std::string id = node->getId();
    if (!drawWater && id == "Water") return true;
    
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    return true;
}