#include "ParticlesGame.h"

// Declare our game instance.
ParticlesGame game;

static const std::string _particleFiles[] = 
{
    "res/fire.particle",
    "res/smoke.particle",
    "res/explosion.particle",
};

const static unsigned int _particleFilesCount = 3;
const static float PARTICLE_SIZE_MAX[] = { 5.0f, 30.0f, 30.0f };
const static float EMIT_RATE_MAX[] = { 500, 100, 100 };
const float INPUT_SENSITIVITY = 0.05f;
const Vector4 BACKGROUND_COLOR = Vector4::zero();

ParticlesGame::ParticlesGame() : _scene(NULL)
{
}

void ParticlesGame::initialize()
{
    // Display the gameplay splash screen for at least 1 second.
    displayScreen(this, &ParticlesGame::drawSplash, NULL, 1000L);

    setMultiTouch(true);

    // Set keyboard state.
    _wDown = _aDown = _sDown = _dDown = false;
    _touched = false;
    _prevX = _prevY = 0;

    // Create a scene with a camera node.
    // The camera node is a child of a node at the same location as the particle emitter.
    // The camera node is offset from its parent, looking straight at it.
    // That way, when we rotate the parent node, the camera stays aimed at the particle emitter.
    _scene = Scene::create();
    Node* cameraNode = _scene->addNode("Camera");
    _cameraParent = _scene->addNode("CameraParent");
    _cameraParent->addChild(cameraNode);
    Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 0.25f, 1000.0f);
    cameraNode->setCamera(camera);
    cameraNode->setTranslation(0.0f, 0.0f, 40.0f);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);

    // Create a font for drawing the framerate.
    _font = Font::create("res/arial18.gpb");

    // Load preset emitters.
    loadEmitters();

    // Load the form for editing ParticleEmitters.
    _form = Form::create("res/editor.form");
    _form->setConsumeInputEvents(false);

    // Store pointers to UI controls we care about.
    _startRed = (Slider*)_form->getControl("startRed");
    _startGreen = (Slider*)_form->getControl("startGreen");
    _startBlue = (Slider*)_form->getControl("startBlue");
    _startAlpha = (Slider*)_form->getControl("startAlpha");
    _endRed = (Slider*)_form->getControl("endRed");
    _endGreen = (Slider*)_form->getControl("endGreen");
    _endBlue = (Slider*)_form->getControl("endBlue");
    _endAlpha = (Slider*)_form->getControl("endAlpha");
    _particleProperties = (Container*)_form->getControl("particleProperties");
    _startMin = (Slider*)_form->getControl("startMin");
    _startMax = (Slider*)_form->getControl("startMax");
    _endMin = (Slider*)_form->getControl("endMin");
    _endMax = (Slider*)_form->getControl("endMax");
    _energyMin = (Slider*)_form->getControl("energyMin");
    _energyMax = (Slider*)_form->getControl("energyMax");
    _emissionRate = (Slider*)_form->getControl("emissionRate");
    _started = (CheckBox*)_form->getControl("started");
    _reset = (Button*)_form->getControl("reset");
    _emit = (Button*)_form->getControl("emit");
    _spiralFlame = (RadioButton*)_form->getControl("spiralFlame");
    _smoke = (RadioButton*)_form->getControl("smoke");
    _explosion = (RadioButton*)_form->getControl("explosion");
    _zoomIn = (Button*)_form->getControl("zoomIn");
    _zoomOut = (Button*)_form->getControl("zoomOut");
    _burstSize = (Slider*)_form->getControl("burstSize");
    _posVarX = (Slider*)_form->getControl("posVarX");
    _posVarY = (Slider*)_form->getControl("posVarY");
    _posVarZ = (Slider*)_form->getControl("posVarZ");
    _velX = (Slider*)_form->getControl("velocityX");
    _velY = (Slider*)_form->getControl("velocityY");
    _velZ = (Slider*)_form->getControl("velocityZ");
    _velVarX = (Slider*)_form->getControl("velocityVarX");
    _velVarY = (Slider*)_form->getControl("velocityVarY");
    _velVarZ = (Slider*)_form->getControl("velocityVarZ");
    _accelX = (Slider*)_form->getControl("accelX");
    _accelY = (Slider*)_form->getControl("accelY");
    _accelZ = (Slider*)_form->getControl("accelZ");
    _accelVarX = (Slider*)_form->getControl("accelVarX");
    _accelVarY = (Slider*)_form->getControl("accelVarY");
    _accelVarZ = (Slider*)_form->getControl("accelVarZ");
    _spinSpeedMin = (Slider*)_form->getControl("spinSpeedMin");
    _spinSpeedMax = (Slider*)_form->getControl("spinSpeedMax");
    _axisX = (Slider*)_form->getControl("axisX");
    _axisY = (Slider*)_form->getControl("axisY");
    _axisZ = (Slider*)_form->getControl("axisZ");
    _axisVarX = (Slider*)_form->getControl("axisVarX");
    _axisVarY = (Slider*)_form->getControl("axisVarY");
    _axisVarZ = (Slider*)_form->getControl("axisVarZ");
    _rotationSpeedMin = (Slider*)_form->getControl("rotationSpeedMin");
    _rotationSpeedMax = (Slider*)_form->getControl("rotationSpeedMax");

    // Listen for UI events.
    _startRed->addListener(this, Listener::VALUE_CHANGED);
    _startGreen->addListener(this, Listener::VALUE_CHANGED);
    _startBlue->addListener(this, Listener::VALUE_CHANGED);
    _startAlpha->addListener(this, Listener::VALUE_CHANGED);
    _endRed->addListener(this, Listener::VALUE_CHANGED);
    _endGreen->addListener(this, Listener::VALUE_CHANGED);
    _endBlue->addListener(this, Listener::VALUE_CHANGED);
    _endAlpha->addListener(this, Listener::VALUE_CHANGED);
    _startMin->addListener(this, Listener::VALUE_CHANGED);
    _startMax->addListener(this, Listener::VALUE_CHANGED);
    _endMin->addListener(this, Listener::VALUE_CHANGED);
    _endMax->addListener(this, Listener::VALUE_CHANGED);
    _energyMin->addListener(this, Listener::VALUE_CHANGED);
    _energyMax->addListener(this, Listener::VALUE_CHANGED);
    _emissionRate->addListener(this, Listener::VALUE_CHANGED);
    _started->addListener(this, Listener::VALUE_CHANGED);
    _reset->addListener(this, Listener::CLICK);
    _emit->addListener(this, Listener::CLICK);
    _spiralFlame->addListener(this, Listener::VALUE_CHANGED);
    _smoke->addListener(this, Listener::VALUE_CHANGED);
    _explosion->addListener(this, Listener::VALUE_CHANGED);
    _zoomIn->addListener(this, Listener::PRESS);
    _zoomIn->addListener(this, Listener::RELEASE);
    _zoomOut->addListener(this, Listener::PRESS);
    _zoomOut->addListener(this, Listener::RELEASE);
    _burstSize->addListener(this, Listener::VALUE_CHANGED);
    _posVarX->addListener(this, Listener::VALUE_CHANGED);
    _posVarY->addListener(this, Listener::VALUE_CHANGED);
    _posVarZ->addListener(this, Listener::VALUE_CHANGED);
    _velX->addListener(this, Listener::VALUE_CHANGED);
    _velY->addListener(this, Listener::VALUE_CHANGED);
    _velZ->addListener(this, Listener::VALUE_CHANGED);
    _velVarX->addListener(this, Listener::VALUE_CHANGED);
    _velVarY->addListener(this, Listener::VALUE_CHANGED);
    _velVarZ->addListener(this, Listener::VALUE_CHANGED);
    _accelX->addListener(this, Listener::VALUE_CHANGED);
    _accelY->addListener(this, Listener::VALUE_CHANGED);
    _accelZ->addListener(this, Listener::VALUE_CHANGED);
    _accelVarX->addListener(this, Listener::VALUE_CHANGED);
    _accelVarY->addListener(this, Listener::VALUE_CHANGED);
    _accelVarZ->addListener(this, Listener::VALUE_CHANGED);
    _spinSpeedMin->addListener(this, Listener::VALUE_CHANGED);
    _spinSpeedMax->addListener(this, Listener::VALUE_CHANGED);
    _axisX->addListener(this, Listener::VALUE_CHANGED);
    _axisY->addListener(this, Listener::VALUE_CHANGED);
    _axisZ->addListener(this, Listener::VALUE_CHANGED);
    _axisVarX->addListener(this, Listener::VALUE_CHANGED);
    _axisVarY->addListener(this, Listener::VALUE_CHANGED);
    _axisVarZ->addListener(this, Listener::VALUE_CHANGED);
    _rotationSpeedMin->addListener(this, Listener::VALUE_CHANGED);
    _rotationSpeedMax->addListener(this, Listener::VALUE_CHANGED);
    
    // Apply default emitter values to the UI.
    emitterChanged();
}

void ParticlesGame::controlEvent(Control* control, EventType evt)
{
    // Handle UI events.
    ParticleEmitter* emitter = _particleEmitterNode->getParticleEmitter();
    switch(evt)
    {
    case Listener::VALUE_CHANGED:
        if (control == _startRed)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.x = _startRed->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Red\n\n%.2f", _startRed->getValue());
            _startRed->setText(txt);
        }
        else if (control == _startGreen)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.y = _startGreen->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Green\n\n%.2f", _startGreen->getValue());
            _startGreen->setText(txt);
        }
        else if (control == _startBlue)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.z = _startBlue->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Blue\n\n%.2f", _startBlue->getValue());
            _startBlue->setText(txt);
        }
        else if (control == _startAlpha)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.w = _startAlpha->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Alpha\n\n%.2f", _startAlpha->getValue());
            _startAlpha->setText(txt);
        }
        else if (control == _endRed)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.x = _endRed->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Red\n\n%.2f", _endRed->getValue());
            _endRed->setText(txt);
        }
        else if (control == _endGreen)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.y = _endGreen->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Green\n\n%.2f", _endGreen->getValue());
            _endGreen->setText(txt);
        }
        else if (control == _endBlue)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.z = _endBlue->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Blue\n\n%.2f", _endBlue->getValue());
            _endBlue->setText(txt);
        }
        else if (control == _endAlpha)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.w = _endAlpha->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());

            char txt[15];
            sprintf(txt, "Alpha\n\n%.2f", _endAlpha->getValue());
            _endAlpha->setText(txt);
        }
        else if (control == _startMin)
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
        else if (control == _posVarX)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.x = _posVarX->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
            char txt[25];
            sprintf(txt, "X\n\n%.2f", posVar.x);
            _posVarX->setText(txt);
        }
        else if (control == _posVarY)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.y = _posVarY->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", posVar.y);
            _posVarY->setText(txt);
        }
        else if (control == _posVarZ)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.z = _posVarZ->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", posVar.z);
            _posVarZ->setText(txt);
        }
        else if (control == _velX)
        {
            Vector3 vel = emitter->getVelocity();
            vel.x = _velX->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
            char txt[25];
            sprintf(txt, "X\n\n%.2f", vel.x);
            _velX->setText(txt);
        }
        else if (control == _velY)
        {
            Vector3 vel = emitter->getVelocity();
            vel.y = _velY->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", vel.y);
            _velY->setText(txt);
        }
        else if (control == _velZ)
        {
            Vector3 vel = emitter->getVelocity();
            vel.z = _velZ->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", vel.z);
            _velZ->setText(txt);
        }
        else if (control == _velVarX)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.x = _velVarX->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
            char txt[25];
            sprintf(txt, "X\n\n%.2f", velVar.x);
            _velVarX->setText(txt);
        }
        else if (control == _velVarY)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.y = _velVarY->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", velVar.y);
            _velVarY->setText(txt);
        }
        else if (control == _velVarZ)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.z = _velVarZ->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", velVar.z);
            _velVarZ->setText(txt);
        }
        else if (control == _accelX)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.x = _accelX->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
            char txt[25];
            sprintf(txt, "X\n\n%.2f", accel.x);
            _accelX->setText(txt);
        }
        else if (control == _accelY)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.y = _accelY->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", accel.y);
            _accelY->setText(txt);
        }
        else if (control == _accelZ)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.z = _accelZ->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", accel.z);
            _accelZ->setText(txt);
        }
        else if (control == _accelVarX)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.x = _accelVarX->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
            char txt[25];
            sprintf(txt, "X\n\n%.2f", accelVar.x);
            _accelVarX->setText(txt);
        }
        else if (control == _accelVarY)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.y = _accelVarY->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", accelVar.y);
            _accelVarY->setText(txt);
        }
        else if (control == _accelVarZ)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.z = _accelVarZ->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", accelVar.z);
            _accelVarZ->setText(txt);
        }
        else if (control == _spinSpeedMin)
        {
            emitter->setRotationPerParticle(_spinSpeedMin->getValue(), emitter->getRotationPerParticleSpeedMax());
            char txt[25];
            sprintf(txt, "Min.\n\n%.2f", _spinSpeedMin->getValue());
            _spinSpeedMin->setText(txt);
        }
        else if (control == _spinSpeedMax)
        {
            emitter->setRotationPerParticle(emitter->getRotationPerParticleSpeedMin(), _spinSpeedMax->getValue());
            char txt[25];
            sprintf(txt, "Min.\n\n%.2f", _spinSpeedMax->getValue());
            _spinSpeedMax->setText(txt);
        }
        else if (control == _axisX)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.x = _axisX->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
            char txt[25];
            sprintf(txt, "X\n\n%.2f", _axisX->getValue());
            _axisX->setText(txt);
        }
        else if (control == _axisY)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.y = _axisY->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", _axisY->getValue());
            _axisY->setText(txt);
        }
        else if (control == _axisZ)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.z = _axisZ->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", _axisZ->getValue());
            _axisZ->setText(txt);
        }
        else if (control == _axisVarX)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.x = _axisVarX->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
            char txt[25];
            sprintf(txt, "X\n\n%.2f", _axisVarX->getValue());
            _axisVarX->setText(txt);
        }
        else if (control == _axisVarY)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.y = _axisVarY->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
            char txt[25];
            sprintf(txt, "Y\n\n%.2f", _axisVarY->getValue());
            _axisVarY->setText(txt);
        }
        else if (control == _axisVarZ)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.z = _axisVarZ->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
            char txt[25];
            sprintf(txt, "Z\n\n%.2f", _axisVarZ->getValue());
            _axisVarZ->setText(txt);
        }
        else if (control == _rotationSpeedMin)
        {
            emitter->setRotation(_rotationSpeedMin->getValue(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), emitter->getRotationAxisVariance());
            char txt[25];
            sprintf(txt, "Min.\n\n%.2f", _rotationSpeedMin->getValue());
            _rotationSpeedMin->setText(txt);
        }
        else if (control == _rotationSpeedMax)
        {
            emitter->setRotation(emitter->getRotationSpeedMin(), _rotationSpeedMax->getValue(), emitter->getRotationAxis(), emitter->getRotationAxisVariance());
            char txt[25];
            sprintf(txt, "Max.\n\n%.2f", _rotationSpeedMax->getValue());
            _rotationSpeedMax->setText(txt);
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
        if (control == _reset)
        {
            // Re-load the current emitter.
            _particleEmitterNode->setParticleEmitter(NULL);
            SAFE_RELEASE(emitter);
            emitter = _particleEmitters[_particleEmitterIndex] = ParticleEmitter::create(_particleFiles[_particleEmitterIndex].c_str());
            emitterChanged();
        }
        else if (control == _emit)
        {
            // Emit a burst of particles.
            unsigned int burstSize = (unsigned int)_burstSize->getValue();
            emitter->emitOnce(burstSize);
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
    SAFE_RELEASE(_font);

    for (unsigned int i = 0; i < _particleEmitters.size(); i++)
    {
        SAFE_RELEASE(_particleEmitters[i]);
    }    
}

void ParticlesGame::update(float elapsedTime)
{
    // Update camera movement
    if (_wDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getForwardVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY * elapsedTime);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_aDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getLeftVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY * elapsedTime);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_sDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getBackVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY * elapsedTime);
        _scene->getActiveCamera()->getNode()->translate(v);
    }
    if (_dDown)
    {
        Vector3 v = _scene->getActiveCamera()->getNode()->getRightVector();
        v.normalize();
        v.scale(INPUT_SENSITIVITY * elapsedTime);
        _scene->getActiveCamera()->getNode()->translate(v);
    }

    // Update particles.
    _particleEmitterNode->getParticleEmitter()->update(elapsedTime);
    
    // Update UI.
    _form->update(elapsedTime);
}

void ParticlesGame::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, BACKGROUND_COLOR, 1.0f, 0);

    // Draw the UI.
    _form->draw();

    // Visit all the nodes in the scene for drawing.
    _scene->visit(this, &ParticlesGame::drawScene, (void*)0);

    // Draw the framerate and number of live particles.
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 170, 10, getFrameRate());
}

bool ParticlesGame::drawScene(Node* node, void* cookie)
{
    ParticleEmitter* emitter = node->getParticleEmitter();
    if (emitter)
    {
        emitter->draw();
    }
    return true;
}

void ParticlesGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // Touch events that don't hit the UI
    // allow the camera to rotate around the particle emitter.
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
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        case Keyboard::KEY_B:
            // Disable blending.
            _particleEmitterNode->getParticleEmitter()->setTextureBlending(ParticleEmitter::BLEND_OPAQUE);
            break;
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

void ParticlesGame::loadEmitters()
{
    for (unsigned int i = 0; i < _particleFilesCount; i++)
    {
        ParticleEmitter* emitter = ParticleEmitter::create(_particleFiles[i].c_str());
        _particleEmitters.push_back(emitter);
    }
    _particleEmitterIndex = 0;

    _particleEmitterNode = _scene->addNode("Particle Emitter");
    _particleEmitterNode->setTranslation(0.0f, 0.0f, 0.0f);
}

void ParticlesGame::emitterChanged()
{
    // Stop the current emitter.
    ParticleEmitter* prevEmitter = _particleEmitterNode->getParticleEmitter();
    if (prevEmitter)
    {
        prevEmitter->stop();
    }

    // Set the new emitter on the node.
    ParticleEmitter* emitter = _particleEmitters[_particleEmitterIndex];
    _particleEmitterNode->setParticleEmitter(emitter);

    // The 'explosion' emitter is meant to emit in bursts.
    if (_particleEmitterIndex == 2)
    {
        _started->setChecked(false);
        emitter->emitOnce(20);
    }
    else
    {
        _started->setChecked(true);
        emitter->start();
    }

    // Reset camera view and zoom.
    _scene->getActiveCamera()->getNode()->setTranslation(0.0f, 0.0f, 40.0f);
    _cameraParent->setIdentity();

    // Set the values of UI controls to display the new emitter's settings.
    char txt[25];

    _startRed->setValue(emitter->getColorStart().x);
    sprintf(txt, "Red\n\n%.2f", _startRed->getValue());
    _startRed->setText(txt);

    _startGreen->setValue(emitter->getColorStart().y);
    sprintf(txt, "Green\n\n%.2f", _startGreen->getValue());
    _startGreen->setText(txt);

    _startBlue->setValue(emitter->getColorStart().z);
    sprintf(txt, "Blue\n\n%.2f", _startBlue->getValue());
    _startBlue->setText(txt);

    _startAlpha->setValue(emitter->getColorStart().w);
    sprintf(txt, "Alpha\n\n%.2f", _startAlpha->getValue());
    _startAlpha->setText(txt);

    _endRed->setValue(emitter->getColorEnd().x);
    sprintf(txt, "Red\n\n%.2f", _endRed->getValue());
    _endRed->setText(txt);

    _endGreen->setValue(emitter->getColorEnd().y);
    sprintf(txt, "Green\n\n%.2f", _endGreen->getValue());
    _endGreen->setText(txt);

    _endBlue->setValue(emitter->getColorEnd().z);
    sprintf(txt, "Blue\n\n%.2f", _endBlue->getValue());
    _endBlue->setText(txt);

    _endAlpha->setValue(emitter->getColorEnd().w);
    sprintf(txt, "Alpha\n\n%.2f", _endAlpha->getValue());
    _endAlpha->setText(txt);

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

    const Vector3& posVar = emitter->getPositionVariance();
    _posVarX->setValue(posVar.x);
    sprintf(txt, "X\n\n%.2f", posVar.x);
    _posVarX->setText(txt);

    _posVarY->setValue(posVar.y);
    sprintf(txt, "Y\n\n%.2f", posVar.y);
    _posVarY->setText(txt);

    _posVarZ->setValue(posVar.z);
    sprintf(txt, "Z\n\n%.2f", posVar.z);
    _posVarZ->setText(txt);

    const Vector3& vel = emitter->getVelocity();
    _velX->setValue(vel.x);
    sprintf(txt, "X\n\n%.2f", vel.x);
    _velX->setText(txt);

    _velY->setValue(vel.y);
    sprintf(txt, "Y\n\n%.2f", vel.y);
    _velY->setText(txt);

    _velZ->setValue(vel.z);
    sprintf(txt, "Z\n\n%.2f", vel.z);
    _velZ->setText(txt);

    const Vector3& velVar = emitter->getVelocityVariance();
    _velVarX->setValue(velVar.x);
    sprintf(txt, "X\n\n%.2f", velVar.x);
    _velVarX->setText(txt);

    _velVarY->setValue(velVar.y);
    sprintf(txt, "Y\n\n%.2f", velVar.y);
    _velVarY->setText(txt);
    
    _velVarZ->setValue(velVar.z);
    sprintf(txt, "Z\n\n%.2f", velVar.z);
    _velVarZ->setText(txt);

    const Vector3& accel = emitter->getAcceleration();
    _accelX->setValue(accel.x);
    sprintf(txt, "X\n\n%.2f", accel.x);
    _accelX->setText(txt);

    _accelY->setValue(accel.y);
    sprintf(txt, "Y\n\n%.2f", accel.y);
    _accelY->setText(txt);

    _accelZ->setValue(accel.z);
    sprintf(txt, "Z\n\n%.2f", accel.z);
    _accelZ->setText(txt);

    const Vector3& accelVar = emitter->getAccelerationVariance();
    _accelVarX->setValue(accelVar.x);
    sprintf(txt, "X\n\n%.2f", accelVar.x);
    _accelVarX->setText(txt);

    _accelVarY->setValue(accelVar.y);
    sprintf(txt, "Y\n\n%.2f", accelVar.y);
    _accelVarY->setText(txt);

    _accelVarZ->setValue(accelVar.z);
    sprintf(txt, "Z\n\n%.2f", accelVar.z);
    _accelVarZ->setText(txt);

    _spinSpeedMin->setValue(emitter->getRotationPerParticleSpeedMin());
    sprintf(txt, "Min.\n\n%.2f", emitter->getRotationPerParticleSpeedMin());
    _spinSpeedMin->setText(txt);

    _spinSpeedMax->setValue(emitter->getRotationPerParticleSpeedMax());
    sprintf(txt, "Max.\n\n%.2f", emitter->getRotationPerParticleSpeedMax());
    _spinSpeedMax->setText(txt);

    const Vector3& axis = emitter->getRotationAxis();
    _axisX->setValue(axis.x);
    sprintf(txt, "X\n\n%.2f", axis.x);
    _axisX->setText(txt);

    _axisY->setValue(axis.y);
    sprintf(txt, "Y\n\n%.2f", axis.y);
    _axisY->setText(txt);
    
    _axisZ->setValue(axis.z);
    sprintf(txt, "Z\n\n%.2f", axis.z);
    _axisZ->setText(txt);

    const Vector3& axisVar = emitter->getRotationAxisVariance();
    _axisVarX->setValue(axisVar.x);
    sprintf(txt, "X\n\n%.2f", axisVar.x);
    _axisVarX->setText(txt);

    _axisVarY->setValue(axisVar.y);
    sprintf(txt, "Y\n\n%.2f", axisVar.y);
    _axisVarY->setText(txt);

    _axisVarZ->setValue(axisVar.z);
    sprintf(txt, "Z\n\n%.2f", axisVar.z);
    _axisVarZ->setText(txt);

    _rotationSpeedMin->setValue(emitter->getRotationSpeedMin());
    sprintf(txt, "Min.\n\n%.2f", emitter->getRotationSpeedMin());
    _rotationSpeedMin->setText(txt);

    _rotationSpeedMax->setValue(emitter->getRotationSpeedMax());
    sprintf(txt, "Max.\n\n%.2f", emitter->getRotationSpeedMax());
    _rotationSpeedMax->setText(txt);
}

void ParticlesGame::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(this->getWidth() * 0.5f, this->getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}

void ParticlesGame::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[30];
    sprintf(buffer, "FPS: %u\nParticles: %u", fps, _particleEmitterNode->getParticleEmitter()->getParticlesCount());
    font->start();
    font->drawText(buffer, x, y, color, font->getSize());
    font->finish();
}
