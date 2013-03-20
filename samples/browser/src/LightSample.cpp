#include "LightSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Light", LightSample, 11);
#endif

LightSample::LightSample()
    : _font(NULL), 
	  _scene(NULL), 
      _modelNode(NULL),
	  _directionalLightNode(NULL), 
	  _pointLightNode(NULL), 
	  _spotLightNode(NULL),
	  _usedForMoving(NULL),
	  _model(NULL),
	  _directionalLightQuadModel(NULL),
	  _spotLightQuadModel(NULL),
	  _pointLightQuadModel(NULL),
	  _unlitMaterial(NULL),
	  _texturedMaterial(NULL),
	  _bumpedMaterial(NULL),
	  _bumpedSpecularMaterial(NULL),
	  _lighting(NULL),
	  _noLight(NULL),
	  _directional(NULL),
	  _spot(NULL),
	  _point(NULL),
      _properties(NULL),
      _redSlider(NULL),
      _greenSlider(NULL),
      _blueSlider(NULL),
      _specularSlider(NULL),
      _addSpecular(NULL),
      _addBumped(NULL),
      _form(NULL),
      _touched(false),
      _touchX(0),
      _touchY(0)
{
}

void LightSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/common/arial18.gpb");

	// Load the scene
	_scene = Scene::load("res/common/lightBrickWall.gpb");

	// Get the wall model node
	_modelNode = _scene->findNode("wall"); 
	_model = _modelNode->getModel();

	// Create a directional light and a reference icon for the light
	Light* directionalLight = Light::createDirectional(Vector3::one());
	_directionalLightNode = Node::create("directionalLight");
	_directionalLightNode->setLight(directionalLight);
    SAFE_RELEASE(directionalLight);
	Mesh* directionalLightQuadMesh = Mesh::createQuad(-0.3f, -0.3f, 0.6f, 0.6f);
	_directionalLightQuadModel = Model::create(directionalLightQuadMesh);
    SAFE_RELEASE(directionalLightQuadMesh);
	setUnlitMaterialTexture(_directionalLightQuadModel, "res/png/light-directional.png"); 
	_directionalLightNode->setModel(_directionalLightQuadModel);
    _directionalLightNode->setTranslation(0.0f, 0.0f, 7.0f);
	_scene->addNode(_directionalLightNode);

	// Create a spotlight and create a reference icon for the light
	Light* spotLight = Light::createSpot(Vector3::one(), 100.0f, MATH_DEG_TO_RAD(30.0f), MATH_DEG_TO_RAD(40.0f));
	_spotLightNode = Node::create("spotLight");
	_spotLightNode->setLight(spotLight);
    SAFE_RELEASE(spotLight);
	Mesh* spotLightQuadMesh = Mesh::createQuad(-0.3f, -0.3f, 0.6f, 0.6f);
	_spotLightQuadModel = Model::create(spotLightQuadMesh);
    SAFE_RELEASE(spotLightQuadMesh);
	setUnlitMaterialTexture(_spotLightQuadModel, "res/png/light-spot.png");
	_spotLightNode->setModel(_spotLightQuadModel);
	_spotLightNode->setTranslation(0.0f, 0.0f, 8.0f);
	_scene->addNode(_spotLightNode);

	// Create a point light and create a reference icon for the light
	Light* pointLight = Light::createPoint(Vector3::one(), 16.0f);
	_pointLightNode = Node::create("pointLight");
	_pointLightNode->setLight(pointLight);
    SAFE_RELEASE(pointLight);
	Mesh* pointLightQuadMesh = Mesh::createQuad(-0.3f, -0.3f, 0.6f, 0.6f);
	_pointLightQuadModel = Model::create(pointLightQuadMesh);
    SAFE_RELEASE(pointLightQuadMesh);
	setUnlitMaterialTexture(_pointLightQuadModel, "res/png/light-point.png");
	_pointLightNode->setModel(_pointLightQuadModel);
	_pointLightNode->setTranslation(0.0f, 0.0f, 8.0f);
	_scene->addNode(_pointLightNode);

    // Create and initialize ui form
	_form = Form::create("res/common/light.form");
    _properties = static_cast<Container*>(_form->getControl("lightProperties"));
	_redSlider = static_cast<Slider*>(_form->getControl("redSlider"));
	_redSlider->addListener(this, Control::Listener::VALUE_CHANGED);
    _greenSlider = static_cast<Slider*>(_form->getControl("greenSlider"));
	_greenSlider->addListener(this, Control::Listener::VALUE_CHANGED);
    _blueSlider = static_cast<Slider*>(_form->getControl("blueSlider"));
	_blueSlider->addListener(this, Control::Listener::VALUE_CHANGED);
	_specularSlider = static_cast<Slider*>(_form->getControl("specularSlider"));
	_specularSlider->addListener(this, Control::Listener::VALUE_CHANGED);
	_noLight = static_cast<RadioButton*>(_form->getControl("noLighting"));
	_noLight->addListener(this, Control::Listener::VALUE_CHANGED);
	_directional = static_cast<RadioButton*>(_form->getControl("directional"));
	_directional->addListener(this, Control::Listener::VALUE_CHANGED);
	_spot = static_cast<RadioButton*>(_form->getControl("spot"));
	_spot->addListener(this, Control::Listener::VALUE_CHANGED);
	_point = static_cast<RadioButton*>(_form->getControl("point"));
	_point->addListener(this, Control::Listener::VALUE_CHANGED);
	_addSpecular = static_cast<CheckBox*>(_form->getControl("specularCheckBox"));
	_addSpecular->addListener(this, Control::Listener::VALUE_CHANGED);
	_addBumped = static_cast<CheckBox*>(_form->getControl("bumpedCheckBox"));
	_addBumped->addListener(this, Control::Listener::VALUE_CHANGED);

    _properties->setEnabled(false);
    _noLight->setSelected(true);
	_form->setConsumeInputEvents(false);
	
    // Create and initialize lights and materials for lights
	_lighting = Material::create("res/common/light.material");
	_model->setMaterial(_lighting);
	initializeDirectionalTechnique("directional");
	initializeDirectionalTechnique("directionalSpecular");
	initializeDirectionalTechnique("directionalBumped");
	initializeDirectionalTechnique("directionalSpecularBumped");
	initializeSpotTechnique("spot");
	initializeSpotTechnique("spotSpecular");
	initializeSpotTechnique("spotBumped");
	initializeSpotTechnique("spotSpecularBumped");
	initializePointTechnique("point");
	initializePointTechnique("pointSpecular");
	initializePointTechnique("pointBumped");
	initializePointTechnique("pointSpecularBumped");

	setSpecularValue(_specularSlider->getValue());
}

void LightSample::finalize()
{
    SAFE_RELEASE(_directionalLightQuadModel);
    SAFE_RELEASE(_directionalLightNode);
    SAFE_RELEASE(_spotLightQuadModel);
    SAFE_RELEASE(_spotLightNode);
    SAFE_RELEASE(_pointLightQuadModel);
    SAFE_RELEASE(_pointLightNode);
    SAFE_RELEASE(_lighting);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
}

void LightSample::update(float elapsedTime)
{	
}

void LightSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

	_model->draw();

	if(_directional->isSelected())
	{
		_directionalLightQuadModel->draw();
	}
	else if (_spot->isSelected())
	{
		_spotLightQuadModel->draw();
	}
	else if (_point->isSelected())
	{
		_pointLightQuadModel->draw();
	}

    _form->draw();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void LightSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	switch (evt)
	{
	case Touch::TOUCH_PRESS:
		{
			_touched = true;
			_touchX = x;
			_touchY = y;
			
			Ray pickRay;
			_scene->getActiveCamera()->pickRay(getViewport(), _touchX, _touchY, &pickRay);

			if (_directional->isSelected() && pickRay.intersects(_directionalLightQuadModel->getMesh()->getBoundingBox()))
			{
				_usedForMoving = _directionalLightNode;
			}
			else if (_spot->isSelected() && pickRay.intersects(_spotLightQuadModel->getMesh()->getBoundingBox()))
			{
				_usedForMoving = _spotLightNode;
			}
			else if (_point->isSelected() && pickRay.intersects(_pointLightQuadModel->getMesh()->getBoundingBox()))
			{
				_usedForMoving = _pointLightNode;
			}
			else
			{
				_usedForMoving = NULL;
			}
		}
		break;

	case Touch::TOUCH_RELEASE:
		{
			_touched = false;
			_touchX = 0;
			_touchY = 0;
			_usedForMoving = NULL;
		}
		break;

	case Touch::TOUCH_MOVE:
		{
			int deltaX = x - _touchX;
			_touchX = x;

			int deltaY = y - _touchY;
			_touchY = y;

			Matrix m;
			_modelNode->getWorldMatrix().transpose(&m);

			// Yaw in world frame
			Vector3 yaw;
			m.getUpVector(&yaw);
			_modelNode->rotate(yaw, MATH_DEG_TO_RAD(deltaX * 0.5f));

			// Roll in world frame
			Vector3 pitch;
			m.getRightVector(&pitch);
			_modelNode->rotate(pitch, MATH_DEG_TO_RAD(deltaY * 0.5f));
		}
		break;

	default:
		break;
	};
}

void LightSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
	switch(evt)
    {
    case Keyboard::KEY_PRESS:
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
		}
		break;
    case Keyboard::KEY_RELEASE:
        break;
    }
}

void LightSample::controlEvent(Control* control, EventType evt)
{
    bool changeTechnique = false;
	
	switch(evt)
    {
	case Control::Listener::VALUE_CHANGED:
		if ((control == _redSlider) || (control == _greenSlider) || (control == _blueSlider))
        {
			Vector3 color(_redSlider->getValue(), _greenSlider->getValue(), _blueSlider->getValue());
			setColorValue(color);
        }
		else if (control == _specularSlider)
        {
			setSpecularValue(_specularSlider->getValue());
        }
	    else if (control == _noLight)
        {
			changeTechnique = true;
            if (_noLight->isSelected())
            {
                _properties->setEnabled(false);
            }
        }
	    else if ((control == _directional) ||  (control == _spot) || (control == _point))
        {
			changeTechnique = true;
            if (!_noLight->isSelected())
            {
                _properties->setEnabled(true);
            }
        }
        else if ((control == _addSpecular) || (control == _addBumped))
        {
            changeTechnique = true;
        }
		break;
	}

	if (changeTechnique)
	{
		if (_noLight->isSelected())
		{
			_lighting->setTechnique("unlit");
		}
		else if (_directional->isSelected() && !_addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("directional");
		}
		else if (_directional->isSelected() && _addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("directionalSpecular");
		}
		else if (_directional->isSelected() && !_addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("directionalBumped");
		}
		else if (_directional->isSelected() && _addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("directionalSpecularBumped");
		}
		else if (_spot->isSelected() && !_addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("spot");
		}
	    else if (_spot->isSelected() && _addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("spotSpecular");
		}
		else if (_spot->isSelected() && !_addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("spotBumped");
		}
		else if (_spot->isSelected() && _addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("spotSpecularBumped");
		}
	    else if (_point->isSelected() && !_addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("point");
		}
	    else if (_point->isSelected() && _addSpecular->isChecked() && !_addBumped->isChecked())
		{
			_lighting->setTechnique("pointSpecular");
		}
		else if (_point->isSelected() && !_addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("pointBumped");
		}
		else if (_point->isSelected() && _addSpecular->isChecked() && _addBumped->isChecked())
		{
			_lighting->setTechnique("pointSpecularBumped");
		}
	}
}

void LightSample::initializeDirectionalTechnique(const char* technique)
{
    _lighting->getTechnique(technique)->getParameter("u_lightDirection")->bindValue(_directionalLightNode, &Node::getForwardVectorWorld); 
	_lighting->getTechnique(technique)->getParameter("u_lightColor")->setValue(Vector3(_redSlider->getValue(), _greenSlider->getValue(), _blueSlider->getValue()));
	_lighting->getTechnique(technique)->getParameter("u_ambientColor")->setValue(Vector3(0.0f, 0.0f, 0.0f));
}

void LightSample::initializeSpotTechnique(const char* technique)
{
	_lighting->getTechnique(technique)->getParameter("u_spotLightInnerAngleCos")->setValue(_spotLightNode->getLight()->getInnerAngleCos());
	_lighting->getTechnique(technique)->getParameter("u_spotLightOuterAngleCos")->setValue(_spotLightNode->getLight()->getOuterAngleCos());
	_lighting->getTechnique(technique)->getParameter("u_spotLightRangeInverse")->setValue(_spotLightNode->getLight()->getRangeInverse());
	_lighting->getTechnique(technique)->getParameter("u_spotLightDirection")->bindValue(_spotLightNode, &Node::getForwardVectorView);
	_lighting->getTechnique(technique)->getParameter("u_spotLightPosition")->bindValue(_spotLightNode, &Node::getTranslationView);
	_lighting->getTechnique(technique)->getParameter("u_ambientColor")->setValue(Vector3(0.0f, 0.0f, 0.0f));
	_lighting->getTechnique(technique)->getParameter("u_lightColor")->setValue(Vector3(_redSlider->getValue(), _greenSlider->getValue(), _blueSlider->getValue()));
}

void LightSample::initializePointTechnique(const char* technique)
{
	_lighting->getTechnique(technique)->getParameter("u_pointLightPosition")->bindValue(_pointLightNode, &Node::getTranslationView);
	_lighting->getTechnique(technique)->getParameter("u_pointLightRangeInverse")->setValue(_pointLightNode->getLight()->getRangeInverse());
	_lighting->getTechnique(technique)->getParameter("u_ambientColor")->setValue(Vector3(0.0f, 0.0f, 0.0f));
	_lighting->getTechnique(technique)->getParameter("u_lightColor")->setValue(Vector3(_redSlider->getValue(), _greenSlider->getValue(), _blueSlider->getValue()));
}

void LightSample::setUnlitMaterialTexture(Model* model, const char* texturePath, bool mipmap)
{
    Material* material = model->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag");
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");

    // Load the texture from file.
    Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue(texturePath, mipmap);
    if (mipmap)
	{
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
	}
	else
	{
        sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
	}
	sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);
	material->getStateBlock()->setBlend(true);
	material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
	material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
}

void LightSample::setColorValue(const Vector3& value)
{
	_lighting->getTechnique(("directional"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("directionalSpecular"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("directionalBumped"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("directionalSpecularBumped"))->getParameter("u_lightColor")->setValue(value);

	_lighting->getTechnique(("spot"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("spotSpecular"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("spotBumped"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("spotSpecularBumped"))->getParameter("u_lightColor")->setValue(value);

	_lighting->getTechnique(("point"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("pointSpecular"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("pointBumped"))->getParameter("u_lightColor")->setValue(value);
	_lighting->getTechnique(("pointSpecularBumped"))->getParameter("u_lightColor")->setValue(value);
}

void LightSample::setSpecularValue(float value)
{
	_lighting->getTechnique("directionalSpecular")->getParameter("u_specularExponent")->setValue(value);
	_lighting->getTechnique("directionalSpecularBumped")->getParameter("u_specularExponent")->setValue(value);
	_lighting->getTechnique("spotSpecular")->getParameter("u_specularExponent")->setValue(value);
	_lighting->getTechnique("spotSpecularBumped")->getParameter("u_specularExponent")->setValue(value);
	_lighting->getTechnique("pointSpecular")->getParameter("u_specularExponent")->setValue(value);
	_lighting->getTechnique("pointSpecularBumped")->getParameter("u_specularExponent")->setValue(value);
}
