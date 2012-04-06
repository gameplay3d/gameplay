#include "ParticlesGame.h"

// Declare our game instance
ParticlesGame game;

// Emitter presets.
static std::string _particleFiles[] = 
{
    "fire",
    "smoke",
    "explosion",
};
const static unsigned int _particleFilesCount = 3;

// Form settings for presets.
const static float PARTICLE_SIZE_MAX[] = { 5.0f, 30.0f, 30.0f };

const static float EMIT_RATE_MAX[] = { 500, 100, 100 };

const float INPUT_SENSITIVITY = 1.0f;

const Vector4 BACKGROUND_COLOR = Vector4::zero();

ParticlesGame::ParticlesGame()
    : _scene(NULL)
{
}

void ParticlesGame::initialize()
{
    // Set keyboard state.
    _wDown = _aDown = _sDown = _dDown = false;
    _touched = false;
    _prevX = _prevY = 0;

    // Create a scene with a camera node.
    // The camera node is a child of a node at the same location as the particle emitter.
    // The camera node is offset from its parent, looking straight at it.
    // That way, when we rotate the parent node, the camera stays aimed at
    // the particle emitter.
    _scene = Scene::createScene();
    Node* cameraNode = _scene->addNode("Camera");
    _cameraParent = _scene->addNode("CameraParent");
    _cameraParent->addChild(cameraNode);
    Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 0.25f, 100.0f);
    cameraNode->setCamera(camera);
    cameraNode->setTranslation(0.0f, 0.0f, 40.0f);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);

    // Load ParticleEmitter presets.
    loadParticleEmitters();

    // Load the form for editing ParticleEmitters.
    _form = Form::create("res/editor.form");
    _form->setConsumeTouchEvents(false);

    _startMin = (Slider*)_form->getControl("startMin");
    _startMax = (Slider*)_form->getControl("startMax");
    _endMin = (Slider*)_form->getControl("endMin");
    _endMax = (Slider*)_form->getControl("endMax");
    _energyMin = (Slider*)_form->getControl("energyMin");
    _energyMax = (Slider*)_form->getControl("energyMax");
    _emissionRate = (Slider*)_form->getControl("emissionRate");
    _started = (CheckBox*)_form->getControl("started");
    _emit = (Button*)_form->getControl("emit");
    _spiralFlame = (RadioButton*)_form->getControl("spiralFlame");
    _smoke = (RadioButton*)_form->getControl("smoke");
    _explosion = (RadioButton*)_form->getControl("explosion");
    _zoomIn = (Button*)_form->getControl("zoomIn");
    _zoomOut = (Button*)_form->getControl("zoomOut");
    _burstSize = (Slider*)_form->getControl("burstSize");

    _startMin->addListener(this, Listener::VALUE_CHANGED);
    _startMax->addListener(this, Listener::VALUE_CHANGED);
    _endMin->addListener(this, Listener::VALUE_CHANGED);
    _endMax->addListener(this, Listener::VALUE_CHANGED);
    _energyMin->addListener(this, Listener::VALUE_CHANGED);
    _energyMax->addListener(this, Listener::VALUE_CHANGED);
    _emissionRate->addListener(this, Listener::VALUE_CHANGED);

    _started->addListener(this, Listener::VALUE_CHANGED);
    _emit->addListener(this, Listener::CLICK);
    _spiralFlame->addListener(this, Listener::VALUE_CHANGED);
    _smoke->addListener(this, Listener::VALUE_CHANGED);
    _explosion->addListener(this, Listener::VALUE_CHANGED);

    _zoomIn->addListener(this, Listener::PRESS);
    _zoomIn->addListener(this, Listener::RELEASE);
    _zoomOut->addListener(this, Listener::PRESS);
    _zoomOut->addListener(this, Listener::RELEASE);

    _burstSize->addListener(this, Listener::VALUE_CHANGED);
    
    emitterChanged();
}

void ParticlesGame::controlEvent(Control* control, EventType evt)
{
    ParticleEmitter* emitter = _particleEmitterNode->getParticleEmitter();
    switch(evt)
    {
    case Listener::VALUE_CHANGED:
        if (control == _startMin)
        {
            emitter->setSize(_startMin->getValue(), emitter->getSizeStartMax(), emitter->getSizeEndMin(), emitter->getSizeEndMax());
            char txt[25];
            sprintf(txt, "Min. Begin Size\n\n%.1f", _startMin->getValue());
            _startMin->setText(txt);
        }
        else if (control == _startMax)
        {
            emitter->setSize(emitter->getSizeStartMin(), _startMax->getValue(), emitter->getSizeEndMin(), emitter->getSizeEndMax());
            char txt[25];
            sprintf(txt, "Max. Begin Size\n\n%.1f", _startMax->getValue());
            _startMax->setText(txt);
        }
        else if (control == _endMin)
        {
            emitter->setSize(emitter->getSizeStartMin(), emitter->getSizeStartMax(), _endMin->getValue(), emitter->getSizeEndMax());
            char txt[25];
            sprintf(txt, "Min. End Size\n\n%.1f", _endMin->getValue());
            _endMin->setText(txt);
        }
        else if (control == _endMax)
        {
            emitter->setSize(emitter->getSizeStartMin(), emitter->getSizeStartMax(), emitter->getSizeEndMin(), _endMax->getValue());
            char txt[25];
            sprintf(txt, "Max. End Size\n\n%.1f", _endMax->getValue());
            _endMax->setText(txt);
        }
        else if (control == _energyMin)
        {
            emitter->setEnergy(_energyMin->getValue(), emitter->getEnergyMax());
            char txt[25];
            sprintf(txt, "Min. Energy\n\n%.0f", _energyMin->getValue());
            _energyMin->setText(txt);
        }
        else if (control == _energyMax)
        {
            emitter->setEnergy(emitter->getEnergyMin(), _energyMax->getValue());
            char txt[25];
            sprintf(txt, "Max. Energy\n\n%.0f", _energyMax->getValue());
            _energyMax->setText(txt);
        }
        else if (control == _emissionRate)
        {
            emitter->setEmissionRate(_emissionRate->getValue());
            char txt[25];
            sprintf(txt, "Emission Rate\n\n%.0f", _emissionRate->getValue());
            _emissionRate->setText(txt);
        }
        else if (control == _burstSize)
        {
            char txt[25];
            sprintf(txt, "Burst Size\n\n%.0f", _burstSize->getValue());
            _burstSize->setText(txt);
        }
        else if (control == _started)
        {
            if (_started->isChecked())
            {
                emitter->start();
            }
            else
            {
                emitter->stop();
            }
        }
        else if (control == _spiralFlame && _spiralFlame->isSelected())
        {
            _particleEmitterIndex = 0;
            emitterChanged();
        }
        else if (control == _smoke && _smoke->isSelected())
        {
            _particleEmitterIndex = 1;
            emitterChanged();
        }
        else if (control == _explosion && _explosion->isSelected())
        {
            _particleEmitterIndex = 2;
            emitterChanged();
        }
        break;
    case Listener::CLICK:
        if (control == _emit)
        {
            unsigned int burstSize = (unsigned int)_burstSize->getValue();
            emitter->emit(burstSize);
        }
        break;
    case Listener::PRESS:
        if (control == _zoomIn)
        {
            _wDown = true;
        }
        else if (control == _zoomOut)
        {
            _sDown = true;
        }
        break;
    case Listener::RELEASE:
        if (control == _zoomIn)
        {
            _wDown = false;
        }
        else if (control == _zoomOut)
        {
            _sDown = false;
        }
        break;
    }
}

void ParticlesGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
    
    for (unsigned int i = 0; i < _particleEmitters.size(); i++)
    {
        SAFE_RELEASE(_particleEmitters[i]);
    }    
}

void ParticlesGame::update(long elapsedTime)
{
    // Update camera movement
    if (_wDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getForwardVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_aDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getLeftVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_sDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getBackVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_dDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getRightVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY);
        _scene->getActiveCamera()->getNode()->translate(v);
    }

    ParticleEmitter* emitter = _particleEmitters[_particleEmitterIndex];
    emitter->update(elapsedTime);

    std::string liveParticlesString("Particles: ");
    char buf[6];
    liveParticlesString.append(itoa(emitter->getParticlesCount(), buf, 10));
    ((Label*)_form->getControl("liveParticlesCount"))->setText(liveParticlesString.c_str());

    std::string fpsString("FPS: ");
    fpsString.append(itoa(getFrameRate(), buf, 10));
    ((Label*)_form->getControl("FPS"))->setText(fpsString.c_str());

    _form->update();
}

void ParticlesGame::render(long elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, BACKGROUND_COLOR, 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &ParticlesGame::drawScene);

    _form->draw();
}

bool ParticlesGame::drawScene(Node* node, void* cookie)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel();
    if (model)
    {
        model->draw();
    }

    ParticleEmitter* emitter = node->getParticleEmitter();
    if (emitter)
    {
        emitter->draw();
    }
    return true;
}

void ParticlesGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _touched = true;
        _prevX = x;
        _prevY = y;
        break;
    case Touch::TOUCH_RELEASE:
        _touched = false;
        break;
    case Touch::TOUCH_MOVE:
        {
            if (_touched)
            {
                int deltaX = x - _prevX;
                int deltaY = y - _prevY;
                _prevX = x;
                _prevY = y;

                _cameraParent->rotateY(MATH_DEG_TO_RAD(deltaX * -0.5f));
                _cameraParent->rotateX(MATH_DEG_TO_RAD(deltaY * -0.5f));
            }
        }
        break;
    default:
        break;
    };
}

void ParticlesGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    switch(evt)
    {
    case Keyboard::KEY_PRESS:
        switch (key)
        {
        case Keyboard::KEY_W:
            _wDown = true;
            break;
        case Keyboard::KEY_A:
            _aDown = true;
            break;
        case Keyboard::KEY_S:
            _sDown = true;
            break;
        case Keyboard::KEY_D:
            _dDown = true;
            break;
        case Keyboard::KEY_P:
            _particleEmitterIndex++;
            if (_particleEmitterIndex >= _particleFilesCount)
            {
                _particleEmitterIndex = 0;
            }
            emitterChanged();
            break;
        }
        break;
    case Keyboard::KEY_RELEASE:
        switch (key)
        {
        case Keyboard::KEY_W:
            _wDown = false;
            break;
        case Keyboard::KEY_A:
            _aDown = false;
            break;
        case Keyboard::KEY_S:
            _sDown = false;
            break;
        case Keyboard::KEY_D:
            _dDown = false;
            break;
        }
        break;
    }
}

void ParticlesGame::loadParticleEmitters()
{
    for (unsigned int i = 0; i < _particleFilesCount; i++)
    {
        std::string s = "res/";
        s += _particleFiles[i].c_str();
        s += ".particle";
        ParticleEmitter* emitter = ParticleEmitter::create(s.c_str());
        _particleEmitters.push_back(emitter);
    }
    _particleEmitterIndex = 0;

    _particleEmitterNode = _scene->addNode("Particle Emitter");
    _particleEmitterNode->setTranslation(0.0f, 0.0f, 0.0f);
}

void ParticlesGame::emitterChanged()
{
    ParticleEmitter* prevEmitter = _particleEmitterNode->getParticleEmitter();
    if (prevEmitter)
    {
        prevEmitter->stop();
        prevEmitter->addRef();
    }

    ParticleEmitter* emitter = _particleEmitters[_particleEmitterIndex];
    _particleEmitterNode->setParticleEmitter(emitter);

    if (_particleEmitterIndex == 2)
    {
        _started->setChecked(false);
        emitter->emit(20);
    }
    else
    {
        _started->setChecked(true);
        emitter->start();
    }

    char txt[25];

    _startMin->setMax(PARTICLE_SIZE_MAX[_particleEmitterIndex]);
    _startMin->setValue(emitter->getSizeStartMin());
    sprintf(txt, "Min. Begin Size\n\n%.1f", _startMin->getValue());
    _startMin->setText(txt);

    _startMax->setMax(PARTICLE_SIZE_MAX[_particleEmitterIndex]);
    _startMax->setValue(emitter->getSizeStartMax());
    sprintf(txt, "Max. Begin Size\n\n%.1f", _startMax->getValue());
    _startMax->setText(txt);
    
    _endMin->setMax(PARTICLE_SIZE_MAX[_particleEmitterIndex]);
    _endMin->setValue(emitter->getSizeEndMin());
    sprintf(txt, "Min. End Size\n\n%.1f", _endMin->getValue());
    _endMin->setText(txt);

    _endMax->setMax(PARTICLE_SIZE_MAX[_particleEmitterIndex]);
    _endMax->setValue(emitter->getSizeEndMax());
    sprintf(txt, "Max. End Size\n\n%.1f", _endMax->getValue());
    _endMax->setText(txt);

    _energyMin->setValue(emitter->getEnergyMin());
    sprintf(txt, "Min. Energy\n\n%.0f", _energyMin->getValue());
    _energyMin->setText(txt);

    _energyMax->setValue(emitter->getEnergyMax());
    sprintf(txt, "Max. Energy\n\n%.0f", _energyMax->getValue());
    _energyMax->setText(txt);

    _emissionRate->setMax(EMIT_RATE_MAX[_particleEmitterIndex]);
    _emissionRate->setValue(emitter->getEmissionRate());
    sprintf(txt, "Emission Rate\n\n%.0f", _emissionRate->getValue());
    _emissionRate->setText(txt);

    sprintf(txt, "Burst Size\n\n%.0f", _burstSize->getValue());
    _burstSize->setText(txt);
}