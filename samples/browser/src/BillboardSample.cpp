#include "BillboardSample.h"
#include "Grid.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Billboards", BillboardSample, 12);
#endif

static const unsigned int MOVE_FORWARD = 1;
static const unsigned int MOVE_BACKWARD = 2;
static const unsigned int MOVE_LEFT = 4;
static const unsigned int MOVE_RIGHT = 8;
static const unsigned int MOVE_UP = 16;
static const unsigned int MOVE_DOWN = 32;

static const float MOVE_SPEED = 1.0f;
static const float UP_DOWN_SPEED = 1.0f;

static const float GROUND_WIDTH = 4.0f;
static const float GROUND_HEIGHT = 4.0f;
static const float GROUND_REPEAT_TEXTURE = 30.0f;

static const float BILLBOARD_WIDTH = 0.5f;
static const float BILLBOARD_HEIGHT = 0.5f;
static const unsigned int BILLBOARD_COUNT = 300;

BillboardSample::BillboardSample()
    : _font(NULL), _scene(NULL), _ground(NULL), _gamepad(NULL), _moveFlags(0), _prevX(0), _prevY(0), _buttonPressed(false)
{
}

void BillboardSample::initialize()
{
    setMultiTouch(true);

	// Create the font and scene
    _font = Font::create("res/common/arial18.gpb");
	_scene = Scene::create();

	// Initialize the camera
    _camera.initialize(0.1f, 100, 45);
    _camera.setPosition(Vector3(0, BILLBOARD_HEIGHT * 1.5f, GROUND_WIDTH / 2.0f));
	_camera.rotate(0.0f, -MATH_DEG_TO_RAD(10));
    _scene->addNode(_camera.getRootNode());
    _scene->setActiveCamera(_camera.getCamera());

	// Load the ground
	loadGround();

    // Load billboards
    loadBillboards();

    _gamepad = getGamepad(0);
    // This is needed because the virtual gamepad is shared between all samples.
    // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
    if (_gamepad && _gamepad->isVirtual())
        _gamepad->getForm()->setEnabled(true);
}

void BillboardSample::finalize()
{
	for (std::vector<Node*>::iterator it = _billboards.begin(); it != _billboards.end(); ++it)
    {
        Node* node = *it;
		node->release();
    }
    _billboards.clear();
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
	SAFE_RELEASE(_ground);
}

void BillboardSample::update(float elapsedTime)
{
    float time = (float)elapsedTime / 1000.0f;

    if (_gamepad->isButtonDown(Gamepad::BUTTON_A))
        _moveFlags |= MOVE_DOWN;
    else
        _moveFlags &= ~MOVE_DOWN;

    if (_gamepad->isButtonDown(Gamepad::BUTTON_B))
        _moveFlags |= MOVE_UP;
    else
        _moveFlags &= ~MOVE_UP;
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
            _camera.moveUp(time * UP_DOWN_SPEED);
        }
        else if (_moveFlags & MOVE_DOWN)
        {
            _camera.moveDown(time * UP_DOWN_SPEED);
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
        _camera.rotate(MATH_DEG_TO_RAD(joy2.x * 2.0f), MATH_DEG_TO_RAD(joy2.y * 2.0f));
    }

    if (!move.isZero())
    {
        move.scale(time * MOVE_SPEED);
        _camera.moveForward(move.y);
        _camera.moveLeft(move.x);
    }
}

void BillboardSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::fromColor(0x355D90FF), 1.0f, 0);

	// Draw the ground
	_ground->draw();

	// Get the scene camera
	Camera* camera = _scene->getActiveCamera();

	for (unsigned int i = 0; i < BILLBOARD_COUNT; i++)
	{
		Node* node = _billboards[i];

		// Rotate the node x/z to face the camera
		Matrix m;
        Matrix::createBillboard(node->getTranslationWorld(), camera->getNode()->getTranslationWorld(), camera->getNode()->getUpVectorWorld(), &m);
		Quaternion q;
		m.getRotation(&q);
		node->setRotation(q);

		if (node->getBoundingSphere().intersects(camera->getFrustum()))
			node->getModel()->draw();
	}

    // draw the gamepad
    _gamepad->draw();

    // draw the frame rate
    drawFrameRate(_font, Vector4::one(), 5, 1, getFrameRate());
}

void BillboardSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
        _camera.rotate(yaw, pitch);
        break;
    }   
    };
}

void BillboardSample::keyEvent(Keyboard::KeyEvent evt, int key)
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
            _camera.rotate(0, MATH_PIOVER4);
            break;
        case Keyboard::KEY_PG_DOWN:
            _camera.rotate(0, -MATH_PIOVER4);
            break;

        case Keyboard::KEY_ONE:
        case Keyboard::KEY_SPACE:
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

bool BillboardSample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_WHEEL:
        _camera.moveForward(wheelDelta * MOVE_SPEED / 4.0f );
        return true;
    }
    return false;
}

void BillboardSample::loadGround()
{
	// Just a basic plane for this sample
	Mesh* mesh = Mesh::createQuad(-(GROUND_WIDTH / 2.0f), -(GROUND_HEIGHT / 2.0f), GROUND_WIDTH, GROUND_HEIGHT);
	Node* node = Node::create();
	_ground = Model::create(mesh);
	SAFE_RELEASE(mesh);
	node->setModel(_ground);
    _scene->addNode(node);
	node->rotateX(MATH_DEG_TO_RAD(90));
	Effect* effect = Effect::createFromFile("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT");    
	Material* material = Material::create(effect); 
	material->getStateBlock()->setDepthTest(true);
	material->getStateBlock()->setBlend(false);
	Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue("res/png/dirt.png", true);
	sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
	material->getParameter("u_textureRepeat")->setValue(Vector2(GROUND_REPEAT_TEXTURE, GROUND_REPEAT_TEXTURE));
	material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
	_ground->setMaterial(material);
	SAFE_RELEASE(material);
	SAFE_RELEASE(effect)
    SAFE_RELEASE(node);
}

void BillboardSample::loadBillboards()
{
	Mesh* mesh = Mesh::createQuad(-(BILLBOARD_WIDTH / 2.0f), -(BILLBOARD_HEIGHT / 2.0f), BILLBOARD_WIDTH, BILLBOARD_HEIGHT);
	mesh->setBoundingSphere(BoundingSphere(Vector3::zero(), BILLBOARD_HEIGHT));

    Effect* effect = Effect::createFromFile("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_DISCARD_ALPHA");

	// Create the model and node and bind the material
    for ( unsigned int i = 0; i < BILLBOARD_COUNT; i++ ) 
    {   
		Node* node = Node::create();
		Model* model = Model::create(mesh);
		node->setModel(model);
		_scene->addNode(node);
        
		Material* material = Material::create(effect); 
		material->getStateBlock()->setDepthTest(true);
		material->getStateBlock()->setBlend(false);
		material->getParameter("u_diffuseTexture")->setValue("res/png/grass.png" , true);
		material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
		model->setMaterial(material);
		SAFE_RELEASE(model);
		SAFE_RELEASE(material);

		// Randomly postiion within the domain
        float tx = MATH_RANDOM_0_1() * GROUND_WIDTH - (GROUND_WIDTH / 2.0f);
		float tz = MATH_RANDOM_0_1() * GROUND_HEIGHT - (GROUND_HEIGHT / 2.0f);
        node->translate(tx, (BILLBOARD_HEIGHT / 2.0f), tz);

        _billboards.push_back(node);
    }
	SAFE_RELEASE(effect);
	SAFE_RELEASE(mesh);
}

void BillboardSample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
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
