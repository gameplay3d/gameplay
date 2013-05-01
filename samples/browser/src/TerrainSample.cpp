#include "TerrainSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Scene", "Terrain", TerrainSample, 3);
#endif

struct TerrainHitFilter : public PhysicsController::HitFilter
{
    TerrainHitFilter(Terrain* terrain)
    {
        terrainObject = terrain->getNode()->getCollisionObject();
    }

    bool filter(PhysicsCollisionObject* object)
    {
        // Filter out all objects but the terrain
        return (object != terrainObject);
    }

    PhysicsCollisionObject* terrainObject;
};

TerrainSample::TerrainSample()
	: _font(NULL), _scene(NULL), _terrain(NULL), _sky(NULL), _form(NULL), _formVisible(true),
	  _wireframe(false), _debugPhysics(false), _snapToGround(true), _vsync(true),
	  _mode(MODE_LOOK), _sphere(NULL), _box(NULL)
{
}

TerrainSample::~TerrainSample()
{
    SAFE_RELEASE(_box);
    SAFE_RELEASE(_sphere);
    SAFE_RELEASE(_form);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}

void TerrainSample::initialize()
{
    // Load scene
	_scene = Scene::load("res/common/terrain/sample.scene");
	_terrain = _scene->findNode("terrain")->getTerrain();
    _sky = _scene->findNode("sky");

    // Load shapes
    Bundle* bundle;
    bundle = Bundle::create("res/common/sphere.gpb");
    _sphere = bundle->loadNode("sphere");
    SAFE_RELEASE(bundle);

    bundle = Bundle::create("res/common/box.gpb");
    _box = bundle->loadNode("box");
    SAFE_RELEASE(bundle);

    // Load font
	_font = Font::create("res/common/arial18.gpb");

    // Setup form
    _form = Form::create("res/common/terrain/terrain.form");
    _form->getControl("plusButton")->addListener(this, Control::Listener::CLICK);
    _form->getControl("minusButton")->addListener(this, Control::Listener::CLICK);
    _form->getControl("wireframe")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("patches")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("physics")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("lod")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("culling")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("snapToGround")->addListener(this, Control::Listener::VALUE_CHANGED);
    _form->getControl("dropSphere")->addListener(this, Control::Listener::CLICK);
    _form->getControl("dropBox")->addListener(this, Control::Listener::CLICK);
    _form->getControl("clearAll")->addListener(this, Control::Listener::CLICK);
    Control* main = _form->getControl("main");
    _formSize.set(main->getWidth(), main->getHeight());

    // Use script camera for navigation
	enableScriptCamera(true);
    setScriptCameraSpeed(20, 80);
}

void TerrainSample::finalize()
{
	SAFE_RELEASE(_font);
	SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
    SAFE_RELEASE(_sphere);
    SAFE_RELEASE(_box);
}

void TerrainSample::update(float elapsedTime)
{
    Node* camera = _scene->getActiveCamera()->getNode();

	if (_snapToGround)
	{
		// Get current camera location in world coords
		Vector3 pos = camera->getTranslationWorld();

		// Query the height of our terrain at this location
		float height = _terrain->getHeight(pos.x, pos.z);

		// Snap our camera to the ground
		camera->setTranslationY(height + 10);
	}

    // Keep the sky centered around the viewer
    _sky->setTranslationX(camera->getTranslationX());
    _sky->setTranslationZ(camera->getTranslationZ());

    // Prune dropped physics shapes that fall off the terrain
    for (std::list<Node*>::iterator itr = _shapes.begin(); itr != _shapes.end(); )
    {
        Node* shape = *itr;
        if (shape->getTranslation().y < 0)
        {
            _scene->removeNode(shape);
            std::list<Node*>::iterator oldItr = itr;
            ++itr;
            _shapes.erase(oldItr);
        }
        else
        {
            ++itr;
        }
    }
}

void TerrainSample::render(float elapsedTime)
{
	clear(Game::CLEAR_COLOR_DEPTH, 0, 0, 0, 1, 1, 0);

    // Draw scene
	_scene->visit(this, &TerrainSample::drawScene);

    // Debug draw
    if (_debugPhysics)
        Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());

    // Draw form
    _form->draw();

    // Draw text
    char buffer[1024];
    sprintf(buffer, "FPS: %d", getFrameRate());
    _font->start();
    _font->drawText(buffer, 65, 18, Vector4::one(), 30);
    if (_formVisible)
    {
        // Draw stats
        sprintf(buffer,
            "Total Patches: %d\n" \
            "Visible Patches: %d\n" \
            "Total Triangles: %d\n" \
            "Visible Triangles: %d\n",
            _terrain->getPatchCount(),
            _terrain->getVisiblePatchCount(),
            _terrain->getTriangleCount(),
            _terrain->getVisibleTriangleCount());
        _font->drawText(buffer, 25, 300, Vector4::one(), 20);
    }
    _font->finish();
}

bool TerrainSample::drawScene(Node* node)
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

void TerrainSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    // Process keyboard shortcuts for options
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ONE:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("wireframe"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        case Keyboard::KEY_TWO:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("patches"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        case Keyboard::KEY_THREE:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("physics"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        case Keyboard::KEY_FOUR:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("lod"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        case Keyboard::KEY_FIVE:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("culling"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        case Keyboard::KEY_SIX:
            {
                CheckBox* cb = static_cast<CheckBox*>(_form->getControl("snapToGround"));
                cb->setChecked(!cb->isChecked());
            }
            break;
        }
    }
}

void TerrainSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (evt == Touch::TOUCH_PRESS)
    {
        // If the FPS region is touched, toggle vsync (if platform supports it)
        if (x >= 65 && x < 300 && y >= 18 && y <= 48)
        {
            _vsync = !_vsync;
            setVsync(_vsync);
        }
        else if (_mode != MODE_LOOK)
        {
            // Ray sample
            Ray pickRay;
            _scene->getActiveCamera()->pickRay(Rectangle (0, 0, getWidth(), getHeight()), x, y, &pickRay);

            PhysicsController::HitResult hitResult;
            TerrainHitFilter hitFilter(_terrain);
            if (Game::getInstance()->getPhysicsController()->rayTest(pickRay, 1000000, &hitResult, &hitFilter) && hitResult.object == _terrain->getNode()->getCollisionObject())
            {
                Node* clone = NULL;
                PhysicsCollisionShape::Definition rbShape;
                const char* material = NULL;

                switch (_mode)
                {
                case MODE_DROP_SPHERE:
                    {
                        clone = _sphere->clone();
                        rbShape = PhysicsCollisionShape::sphere();
                        material = "res/common/terrain/shapes.material#sphere";
                    }
                    break;

                case MODE_DROP_BOX:
                    {
                        clone = _box->clone();
                        rbShape = PhysicsCollisionShape::box();
                        material = "res/common/terrain/shapes.material#box";
                    }
                    break;
                }

                if (clone)
                {
                    clone->setScale(10,10,10);
                    clone->setTranslation(hitResult.point.x, hitResult.point.y + 50, hitResult.point.z);
                    PhysicsRigidBody::Parameters rbParams(1);
                    clone->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, rbShape, &rbParams);
                    _scene->addNode(clone);
                    clone->getModel()->setMaterial(material);
                    clone->release();

                    _shapes.push_back(clone);

                    _mode = MODE_LOOK;
                    setMessage(NULL);
                }
            }
        }
    }
}

bool TerrainSample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

void TerrainSample::controlEvent(Control* control, EventType evt)
{
    if (strcmp(control->getId(), "plusButton") == 0)
    {
        control->setVisible(false);
        _form->getControl("minusButton")->setVisible(true);
        _form->getControl("settings")->setVisible(true);
        _form->getControl("physicsSettings")->setVisible(true);
        _form->getControl("main")->setSize(_formSize.x, _formSize.y);
        _formVisible = true;
    }
    else if (strcmp(control->getId(), "minusButton") == 0)
    {
        control->setVisible(false);
        _form->getControl("plusButton")->setVisible(true);
        _form->getControl("settings")->setVisible(false);
        _form->getControl("physicsSettings")->setVisible(false);
        _form->getControl("main")->setSize(50, 50);
        _formVisible = false;
    }
    else if (strcmp(control->getId(), "wireframe") == 0)
    {
        _wireframe = static_cast<CheckBox*>(control)->isChecked();
    }
    else if (strcmp(control->getId(), "patches") == 0)
    {
        _terrain->setFlag(Terrain::DEBUG_PATCHES, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "physics") == 0)
    {
        _debugPhysics = static_cast<CheckBox*>(control)->isChecked();
    }
    else if (strcmp(control->getId(), "lod") == 0)
    {
        _terrain->setFlag(Terrain::LEVEL_OF_DETAIL, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "culling") == 0)
    {
        _terrain->setFlag(Terrain::FRUSTUM_CULLING, static_cast<CheckBox*>(control)->isChecked());
    }
    else if (strcmp(control->getId(), "snapToGround") == 0)
    {
        _snapToGround = static_cast<CheckBox*>(control)->isChecked();
        if (_snapToGround)
            setScriptCameraSpeed(20, 80);
        else
            setScriptCameraSpeed(200, 800);
    }
    else if (strcmp(control->getId(), "dropSphere") == 0)
    {
        if (_mode == MODE_DROP_SPHERE)
        {
            _mode = MODE_LOOK;
            setMessage(NULL);
        }
        else
        {
            _mode = MODE_DROP_SPHERE;
            setMessage("Click on the terrain to drop a sphere.");
        }
    }
    else if (strcmp(control->getId(), "dropBox") == 0)
    {
        if (_mode == MODE_DROP_BOX)
        {
            _mode = MODE_LOOK;
            setMessage(NULL);
        }
        else
        {
            _mode = MODE_DROP_BOX;
            setMessage("Click on the terrain to drop a box.");
        }
    }
    else if (strcmp(control->getId(), "clearAll") == 0)
    {
        for (std::list<Node*>::iterator itr = _shapes.begin(); itr != _shapes.end(); ++itr)
            _scene->removeNode(*itr);
        _shapes.clear();
    }
}

void TerrainSample::setMessage(const char* message)
{
    Label* label = static_cast<Label*>(_form->getControl("message"));
    label->setText(message ? message : "");
    _form->getControl("messageBox")->setVisible(message ? true : false);
}
