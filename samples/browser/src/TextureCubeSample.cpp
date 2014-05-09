#include "TextureCubeSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Texture Cube", TextureCubeSample, 7);
#endif

TextureCubeSample::TextureCubeSample()
    : _font(NULL), _scene(NULL), _cubeCamera(NULL), _fBuffer(NULL), _rTargets(NULL)
{
}

void TextureCubeSample::initialize()
{
	// Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

	// Load game scene from file
    _scene = Scene::load("res/common/cubeenv.gpb");

	_scene->setAmbientColor(0.5f, 0.5f, 0.5f);

	// Modify aspect ratio
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio());

	// Add light
	Node* lightNode = _scene->findNode("light_node");
	if(!lightNode->getLight())
	{
		Light* light = Light::createPoint(1, 1, 1, 100000);
		lightNode->setLight(light);
		SAFE_RELEASE(light);
	}

	// Initialize materials
	Properties* matProp = Properties::create("res/common/cubeenv.material");
	_scene->visit(this, &TextureCubeSample::initMaterials, matProp);
	SAFE_DELETE(matProp);

	// Setup cube-map
	createCubeMap();

	// Use script camera for navigation
	enableScriptCamera(true);
    setScriptCameraSpeed(10, 40);
}

void TextureCubeSample::createCubeMap()
{
	// Setup camera
	Camera* cam = Camera::createPerspective(90, 1, 5, 1000);
	_cubeCamera = Node::create("cube_camera_node");
	_cubeCamera->setCamera(cam);
	_scene->addNode(_cubeCamera);

	// Setup sphere materials
	Material* mat = _scene->findNode("almighty_sphere")->getModel()->getMaterial();
	mat->getParameter("u_cameraPosition")->bindValue(_scene->getActiveCamera()->getNode(), &Node::getTranslationView);

	Texture* tex = Texture::create(Texture::RGB, 1024, 1024, NULL, false, Texture::TEX_CUBE);
	Texture::Sampler* texSamp = Texture::Sampler::create(tex);

	mat->getParameter("u_cubeTexture")->setValue(texSamp);

	// Setup rendertargets
	_fBuffer = FrameBuffer::create("cube_camera_framebuffer", tex->getWidth(), tex->getHeight());
	_rTargets = new RenderTarget*[6];
	char buffer[256];
	for(unsigned int i = 0; i < 6; i++)
	{
		std::string rendTargetName = "cube_camera_rendertarget_";
		rendTargetName += itoa(i, buffer, 10);
		Texture* face = tex->getFaceTexture((Texture::CubeFace)i);
		_rTargets[i] = RenderTarget::create(rendTargetName.c_str(), face);
		SAFE_RELEASE(face);
	}

	// Cleanup
	SAFE_RELEASE(texSamp);
	SAFE_RELEASE(tex);
	SAFE_RELEASE(cam);
}

Material* setupLights(Properties* prop, Node* lightNode)
{
	Light* light = lightNode->getLight();

	Material* def = Material::create(prop);
	def->getParameter("u_pointLightPosition[0]")->bindValue(lightNode, &Node::getTranslationView);
	def->getParameter("u_pointLightRangeInverse[0]")->bindValue(light, &Light::getRangeInverse);
	def->getParameter("u_pointLightColor[0]")->bindValue(light, &Light::getColor);

	return def;
}

bool TextureCubeSample::initMaterials(Node* node, Properties* prop)
{
    // If the node visited contains a model, setup materials for it
    Model* model = node->getModel();
    if (model)
    {
		Node* lightNode = node->getScene()->findNode("light_node");
		Properties* p = prop->getNamespace(node->getId());
		Material* m;
		if(p)
		{
			m = setupLights(p, lightNode);
		}
		else
		{
			m = setupLights(prop->getNamespace("default"), lightNode);
		}
		model->setMaterial(m);
		SAFE_RELEASE(m);
    }
    return true;
}

void TextureCubeSample::finalize()
{
	if(_rTargets)
	{
		for(unsigned int i = 0; i < 6; i++)
		{
			SAFE_RELEASE(_rTargets[i]);
		}
		SAFE_DELETE_ARRAY(_rTargets);
	}
	SAFE_RELEASE(_fBuffer);
	SAFE_RELEASE(_cubeCamera);
    SAFE_RELEASE(_scene);
	SAFE_RELEASE(_font);
}

void TextureCubeSample::update(float elapsedTime)
{
    // Rotate model
	_scene->findNode("cube_camera_node")->setTranslation(_scene->findNode("almighty_sphere")->getTranslationWorld());
    _scene->findNode("sphere_handle")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 4000.0f * 180.0f));
}

void TextureCubeSample::render(float elapsedTime)
{
	//Draw cube-map
	Node* ignore = _scene->findNode("almighty_sphere");
	Node* cubeCameraNode = _scene->findNode("cube_camera_node");
	Camera* prevCamera = _scene->getActiveCamera();
	_scene->setActiveCamera(cubeCameraNode->getCamera());

	Rectangle defaultViewport = getViewport();
	setViewport(Rectangle(_fBuffer->getWidth(), _fBuffer->getHeight()));
	FrameBuffer* prevFrameBuffer = _fBuffer->bind();
	for(unsigned int i = 0; i < 6; i++)
	{
		//Set render target
		_fBuffer->setRenderTarget(_rTargets[i]);

		//Setup camera
		cubeCameraNode->setRotation(Quaternion::identity());
		switch((Texture::CubeFace)i)
		{
			case Texture::POS_X:
				cubeCameraNode->rotateY(MATH_DEG_TO_RAD(-90));
				break;
			case Texture::NEG_X:
				cubeCameraNode->rotateY(MATH_DEG_TO_RAD(90));
				break;
			case Texture::POS_Y:
				cubeCameraNode->rotateX(MATH_DEG_TO_RAD(-90));
				break;
			case Texture::NEG_Y:
				cubeCameraNode->rotateX(MATH_DEG_TO_RAD(90));
				break;
			case Texture::POS_Z:
				break;
			case Texture::NEG_Z:
				cubeCameraNode->rotateY(MATH_DEG_TO_RAD(180));
				break;
		}

		//Actual draw
		clear(CLEAR_COLOR, Vector4::zero(), 1.0f, 0);
		_scene->visit(this, &TextureCubeSample::drawScene, ignore);
	}

    // Visit all the nodes in the scene for drawing
	ignore = NULL;
	setViewport(defaultViewport);
	_scene->setActiveCamera(prevCamera);
	prevFrameBuffer->bind();
	clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    _scene->visit(this, &TextureCubeSample::drawScene, ignore);

	// Draw framerate
	drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

bool TextureCubeSample::drawScene(Node* node, Node* ignore)
{
	if(node != ignore)
	{
		// If the node visited contains a model, draw it
		Model* model = node->getModel();
		if (model)
		{
			model->draw();
		}
	}
    return true;
}

void TextureCubeSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	switch(evt)
	{
		case Touch::TOUCH_PRESS:
			Game::getInstance()->setCursorVisible(false);
			if (x < 75 && y < 50)
			{
				// Toggle Vsync if the user touches the top left corner
				setVsync(!isVsync());
			}
			break;
		case Touch::TOUCH_RELEASE:
			Game::getInstance()->setCursorVisible(true);
			break;
	}
}