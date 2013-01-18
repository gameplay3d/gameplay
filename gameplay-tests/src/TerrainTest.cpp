#include "TerrainTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Scene", "Terrain", TerrainTest, 3);
#endif

TerrainTest::TerrainTest()
	: _font(NULL), _scene(NULL), _terrain(NULL), _sky(NULL), _form(NULL), _formVisible(true), _wireframe(false), _snapToGround(false)
{
}

TerrainTest::~TerrainTest()
{
}

void TerrainTest::initialize()
{
    setVsync(false);

    // Load scene
	_scene = Scene::load("res/common/terrain/test.scene");
	_terrain = _scene->findNode("terrain")->getTerrain();
    _sky = _scene->findNode("sky");

    // Load font
	_font = Font::create("res/common/arial18.gpb");

    // Setup form
    _form = Form::create("res/common/terrain/terrain.form");
    _formSize.set(_form->getWidth(), _form->getHeight());
    _form->getControl("plusButton")->addListener(this, Control::Listener::CLICK);
    _form->getControl("minusButton")->addListener(this, Control::Listener::CLICK);
    _form->getControl("lod")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("culling")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("wireframe")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("patches")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("snapToGround")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("plusButton")->setVisible(false);
    _form->getControl("plusButton")->setEnabled(false);

    // Use script camera for navigation
	enableScriptCamera(true);

    // HACK - remove
    Gamepad::getGamepads()->at(0)->getForm()->setEnabled(false);
}

void TerrainTest::finalize()
{
	SAFE_RELEASE(_font);
	SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
}

void TerrainTest::update(float elapsedTime)
{
    _form->update(elapsedTime);

    Node* camera = _scene->getActiveCamera()->getNode();

	if (_snapToGround)
	{
		// Get current camera location in world coords
		Vector3 pos = camera->getTranslationWorld();

		// Query the height of our terrain at this location
		float height = _terrain->getHeight(pos.x, pos.z);

		// Snap our camera to the ground
		camera->setTranslationY(height + 5);
	}

    // Keep the sky centered around the viewer
    _sky->setTranslationX(camera->getTranslationX());
    _sky->setTranslationZ(camera->getTranslationZ());
}

void TerrainTest::render(float elapsedTime)
{
	clear(Game::CLEAR_COLOR_DEPTH, 0, 0, 0, 1, 1, 0);

    // Draw scene
	_scene->visit(this, &TerrainTest::drawScene);

    // Draw form
    _form->draw();

    // Draw FPS
    char buffer[1024];
    sprintf(buffer, "FPS: %d", getFrameRate());
    _font->start();
    _font->drawText(buffer, 65, 18, Vector4::one(), 30);
    if (_formVisible)
    {
        // Draw stats
        sprintf(buffer,
            "Total Pathces: %d\n" \
            "Visible Pathces: %d\n" \
            "Total Triangles: %d\n" \
            "Visible Triangles: %d\n",
            _terrain->getPatchCount(),
            _terrain->getVisiblePatchCount(),
            _terrain->getTriangleCount(),
            _terrain->getVisibleTriangleCount());
        _font->drawText(buffer, 25, 225, Vector4::one(), 20);
    }
    _font->finish();
}

bool TerrainTest::drawScene(Node* node)
{
	if (node->getModel())
	{
		node->getModel()->draw();
	}
	else if (node->getTerrain())
	{
		node->getTerrain()->draw(_wireframe);
	}

	return true;
}

void TerrainTest::keyEvent(Keyboard::KeyEvent evt, int key)
{
}

void TerrainTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool TerrainTest::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

void TerrainTest::controlEvent(Control* control, EventType evt)
{
    if (strcmp(control->getId(), "plusButton") == 0)
    {
        control->setVisible(false);
        control->setEnabled(false);
        _form->getControl("minusButton")->setVisible(true);
        _form->getControl("minusButton")->setEnabled(true);
        _form->setSize(_formSize.x, _formSize.y);
        _formVisible = true;
    }
    else if (strcmp(control->getId(), "minusButton") == 0)
    {
        control->setVisible(false);
        control->setEnabled(false);
        _form->getControl("plusButton")->setVisible(true);
        _form->getControl("plusButton")->setEnabled(true);
        _form->setSize(50, 50);
        _formVisible = false;
    }
    else if (strcmp(control->getId(), "lod") == 0)
    {
        _terrain->setFlag(Terrain::LEVEL_OF_DETAIL, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "culling") == 0)
    {
        _terrain->setFlag(Terrain::FRUSTUM_CULLING, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "wireframe") == 0)
    {
        _wireframe = static_cast<CheckBox*>(control)->isChecked();
    }
    else if (strcmp(control->getId(), "patches") == 0)
    {
        _terrain->setFlag(Terrain::DEBUG_PATCHES, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "snapToGround") == 0)
    {
        _snapToGround = static_cast<CheckBox*>(control)->isChecked();
    }
}
