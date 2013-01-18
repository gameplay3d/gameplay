#include "TerrainTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Scene", "Terrain", TerrainTest, 3);
#endif

TerrainTest::TerrainTest()
	: _font(NULL), _scene(NULL), _terrain(NULL), _sky(NULL), _wireframe(false), _snapToGround(true), _normalMap(false)
{
}

TerrainTest::~TerrainTest()
{
}

void TerrainTest::initialize()
{
    setVsync(false);
	_font = Font::create("res/common/arial18.gpb");
	_scene = Scene::load("res/common/terrain/test.scene");
	_terrain = _scene->findNode("terrain")->getTerrain();
    _sky = _scene->findNode("sky");
	enableScriptCamera(true);
}

void TerrainTest::finalize()
{
	SAFE_RELEASE(_font);
	SAFE_RELEASE(_scene);
}

void TerrainTest::update(float elapsedTime)
{
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

	_scene->visit(this, &TerrainTest::drawScene);

    // Draw stats
    Node* camera = _scene->getActiveCamera()->getNode();
    std::ostringstream str;
    str << "FPS: " << getFrameRate() << "\n"
        << "Position: (" << camera->getTranslationX() << ", " << camera->getTranslationY() << ", " << camera->getTranslationZ() << ")\n";
    _font->start();
    _font->drawText(str.str().c_str(), 5, 5, Vector4(1,1,0,1));
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
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_G:
        case Keyboard::KEY_CAPITAL_G:
            _snapToGround = !_snapToGround;
            break;

        case Keyboard::KEY_T:
        case Keyboard::KEY_CAPITAL_T:
            _wireframe = !_wireframe;
            break;
        }
    }
}

void TerrainTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool TerrainTest::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}
