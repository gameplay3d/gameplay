#include "ParticlesGame.h"

// Declare our game instance.
ParticlesGame game;

#define DEFAULT_PARTICLE_EMITTER "res/fire.particle"

const float INPUT_SENSITIVITY = 0.05f;
const float PANNING_SENSITIVITY = 0.01f;
const float ROTATE_SENSITIVITY = 0.25f;
const Vector4 BACKGROUND_COLOR = Vector4::zero();
const float ZOOM_DEFAULT = 6.0f;

ParticlesGame::ParticlesGame() : _scene(NULL), _panning(false), _rotating(false), _zooming(false)
{
}

void ParticlesGame::addGrid(unsigned int lineCount)
{
    float z = -1;
    
    // There needs to be an odd number of lines
    lineCount |= 1;
    const unsigned int pointCount = lineCount * 4;
    const unsigned int verticesSize = pointCount * (3 + 3);  // (3 (position(xyz) + 3 color(rgb))

    std::vector<float> vertices;
    vertices.resize(verticesSize);

    const float gridLength = (float)(lineCount / 2);
    float value = -gridLength;
    for (unsigned int i = 0; i < verticesSize; ++i)
    {
        // Default line color is dark grey
        Vector4 color(0.3f, 0.3f, 0.3f, 1.0f);

        // Every 10th line is brighter grey
        if (((int)value) % 10 == 0)
        {
            color.set(0.45f, 0.45f, 0.45f, 1.0f);
        }

        // The Z axis is blue
        if (value == 0.0f)
        {
            color.set(0.15f, 0.15f, 0.7f, 1.0f);
        }

        // Build the lines
        vertices[i] = value;
        vertices[++i] = -gridLength;
        vertices[++i] = z;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = value;
        vertices[++i] = gridLength;
        vertices[++i] = z;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        // The X axis is red
        if (value == 0.0f)
        {
            color.set(0.7f, 0.15f, 0.15f, 1.0f);
        }
        vertices[++i] = -gridLength;
        vertices[++i] = value;
        vertices[++i] = z;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = gridLength;
        vertices[++i] = value;
        vertices[++i] = z;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        value += 1.0f;
    }
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), pointCount, false);
    if (mesh == NULL)
        return;

    mesh->setPrimitiveType(Mesh::LINES);
    mesh->setVertexData(&vertices[0], 0, pointCount);

    Model* model = Model::create(mesh);
    model->setMaterial("res/grid.material");
    SAFE_RELEASE(mesh);

    _scene->addNode("grid")->setModel(model);
    model->release();
}

void ParticlesGame::initialize()
{
    // Display the gameplay splash screen
    displayScreen(this, &ParticlesGame::drawSplash, NULL, 250L);

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
    cameraNode->setTranslation(0.0f, 0.0f, ZOOM_DEFAULT);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);

    addGrid(61);

    // Create a font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Load the form for editing ParticleEmitters.
    _form = Form::create("res/editor.form");
    _form->setConsumeInputEvents(false);
    //_form->setState(Control::FOCUS);

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
    _zoomIn = (Button*)_form->getControl("zoomIn");
    _zoomOut = (Button*)_form->getControl("zoomOut");
    _save = (Button*)_form->getControl("save");
    _load = (Button*)_form->getControl("load");
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
    _vsync = (CheckBox*)_form->getControl("vsync");

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
    _zoomIn->addListener(this, Listener::PRESS);
    _zoomIn->addListener(this, Listener::RELEASE);
    _zoomOut->addListener(this, Listener::PRESS);
    _zoomOut->addListener(this, Listener::RELEASE);
    _save->addListener(this, Listener::RELEASE);
    _load->addListener(this, Listener::RELEASE);
    _burstSize->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("posX")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("posY")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("posZ")->addListener(this, Listener::VALUE_CHANGED);
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
    _vsync->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("sprite")->addListener(this, Listener::CLICK);
    _form->getControl("additive")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("transparent")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("multiply")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("opaque")->addListener(this, Listener::VALUE_CHANGED);
    _form->getControl("updateFrames")->addListener(this, Listener::CLICK);

    // Load preset emitters.
    loadEmitters();

    updateImageControl();
}


std::string ParticlesGame::toString(bool b)
{
    return b ? "true" : "false";
}

std::string ParticlesGame::toString(int i)
{
    char buf[1024];
    sprintf(buf, "%d", i);
    return buf;
}

std::string ParticlesGame::toString(unsigned int i)
{
    char buf[1024];
    sprintf(buf, "%d", i);
    return buf;
}

std::string ParticlesGame::toString(const Vector3& v)
{
    std::ostringstream s;
    s << v.x << ", " << v.y << ", " << v.z;
    return s.str();
}

std::string ParticlesGame::toString(const Vector4& v)
{
    std::ostringstream s;
    s << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
    return s.str();
}

std::string ParticlesGame::toString(const Quaternion& q)
{
    std::ostringstream s;
    s << q.x << ", " << q.y << ", " << q.z << ", " << q.w;
    return s.str();
}

std::string ParticlesGame::toString(ParticleEmitter::TextureBlending blending)
{
    switch (blending)
    {
        case ParticleEmitter::BLEND_OPAQUE:
            return "OPAQUE";
        case ParticleEmitter::BLEND_TRANSPARENT:
            return "TRANSPARENT";
        case ParticleEmitter::BLEND_ADDITIVE:
            return "ADDITIVE";
        case ParticleEmitter::BLEND_MULTIPLIED:
            return "MULTIPLIED";
        default:
            return "TRANSPARENT";
    }
}

void ParticlesGame::saveFile()
{
    std::string filename;
    filename = FileSystem::displayFileDialog(FileSystem::SAVE, "Save Particle File", "Particle Files", "particle", "res");

    if (filename.length() == 0)
        return;

    ParticleEmitter* e = _particleEmitter;

    // Extract just the particle name from the filename
    std::string dir = FileSystem::getDirectoryName(filename.c_str());
    std::string ext = FileSystem::getExtension(filename.c_str());
    std::string name = filename.substr(dir.length(), filename.length() - dir.length() - ext.length());

    Texture* texture = e->getTexture();
    std::string texturePath = texture->getPath();
    std::string textureDir = FileSystem::getDirectoryName(texturePath.c_str());
    texturePath = texturePath.substr(textureDir.length());

    // Get camera rotation as axis-angle
    Vector3 cameraAxis;
    float cameraAngle = MATH_RAD_TO_DEG(_cameraParent->getRotation().toAxisAngle(&cameraAxis));

    // Write out a properties file
    std::ostringstream s;
    s << 
        "particle " << name << "\n" <<
        "{\n" <<
        "    sprite\n" <<
        "    {\n" <<
        "        path = " << texturePath << "\n" <<
        "        width = " << e->getSpriteWidth() << "\n" <<
        "        height = " << e->getSpriteHeight() << "\n" <<
        "        blending = " << toString(e->getTextureBlending()) << "\n" <<
        "        animated = " << toString(e->isSpriteAnimated()) << "\n" <<
        "        looped = " << toString(e->isSpriteLooped()) << "\n" <<
        "        frameCount = " << e->getSpriteFrameCount() << "\n" <<
        "        frameRandomOffset = " << e->getSpriteFrameRandomOffset() << "\n" <<
        "        frameDuration = " << e->getSpriteFrameDuration() << "\n" <<
        "    }\n" <<
        "\n" <<
        "    particleCountMax = " << e->getParticleCountMax() << "\n" <<
        "    emissionRate = " << e->getEmissionRate() << "\n" <<
        "    ellipsoid = " << toString(e->isEllipsoid()) << "\n" <<
        "    orbitPosition = " << toString(e->getOrbitPosition()) << "\n" <<
        "    orbitVelocity = " << toString(e->getOrbitVelocity()) << "\n" <<
        "    orbitAcceleration = " << toString(e->getOrbitAcceleration()) << "\n" <<
        "    sizeStartMin = " << e->getSizeStartMin() << "\n" <<
        "    sizeStartMax = " << e->getSizeStartMax() << "\n" <<
        "    sizeEndMin = " << e->getSizeEndMin() << "\n" <<
        "    sizeEndMax = " << e->getSizeEndMax() << "\n" <<
        "    energyMin = " << e->getEnergyMin() << "\n" <<
        "    energyMax = " << e->getEnergyMax() << "\n" <<
        "    colorStart = " << toString(e->getColorStart()) << "\n" <<
        "    colorStartVar = " << toString(e->getColorStartVariance()) << "\n" <<
        "    colorEnd = " << toString(e->getColorEnd()) << "\n" <<
        "    colorEndVar = " << toString(e->getColorEndVariance()) << "\n" <<
        "    position = " << toString(e->getPosition()) << "\n" <<
        "    positionVar = " << toString(e->getPositionVariance()) << "\n" <<
        "    velocity = " << toString(e->getVelocity()) << "\n" <<
        "    velocityVar = " << toString(e->getVelocityVariance()) << "\n" <<
        "    acceleration = " << toString(e->getAcceleration()) << "\n" <<
        "    accelerationVar = " << toString(e->getAccelerationVariance()) << "\n" <<
        "    rotationPerParticleSpeedMin = " << e->getRotationPerParticleSpeedMin() << "\n" <<
        "    rotationPerParticleSpeedMax = " << e->getRotationPerParticleSpeedMax() << "\n" <<
        "\n" <<
        "    editor\n" <<
        "    {\n" <<
        "        cameraTranslation = " << toString(_cameraParent->getTranslation()) << "\n" <<
        "        cameraZoom = " << toString(_scene->getActiveCamera()->getNode()->getTranslation()) << "\n" <<
        "        cameraRotation = " << toString(cameraAxis) << ", " << cameraAngle << "\n" <<
        "        sizeMax = " << _startMax->getMax() << "\n" <<
        "        energyMax = " << _energyMax->getMax() << "\n" <<
        "    }\n"
        "}\n";

    std::string text = s.str();
    Stream* stream = FileSystem::open(filename.c_str(), FileSystem::WRITE);
    stream->write(text.c_str(), 1, text.length());
    stream->close();
    SAFE_DELETE(stream);
}

void ParticlesGame::controlEvent(Control* control, EventType evt)
{
    std::string id = control->getId();

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
        }
        else if (control == _startGreen)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.y = _startGreen->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());
        }
        else if (control == _startBlue)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.z = _startBlue->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());
        }
        else if (control == _startAlpha)
        {
            Vector4 startColor = emitter->getColorStart();
            startColor.w = _startAlpha->getValue();
            emitter->setColor(startColor, emitter->getColorStartVariance(), emitter->getColorEnd(), emitter->getColorEndVariance());
        }
        else if (control == _endRed)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.x = _endRed->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());
        }
        else if (control == _endGreen)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.y = _endGreen->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());
        }
        else if (control == _endBlue)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.z = _endBlue->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());
        }
        else if (control == _endAlpha)
        {
            Vector4 endColor = emitter->getColorEnd();
            endColor.w = _endAlpha->getValue();
            emitter->setColor(emitter->getColorStart(), emitter->getColorStartVariance(), endColor, emitter->getColorEndVariance());
        }
        else if (control == _startMin)
        {
            emitter->setSize(_startMin->getValue(), emitter->getSizeStartMax(), emitter->getSizeEndMin(), emitter->getSizeEndMax());
        }
        else if (control == _startMax)
        {
            emitter->setSize(emitter->getSizeStartMin(), _startMax->getValue(), emitter->getSizeEndMin(), emitter->getSizeEndMax());
        }
        else if (control == _endMin)
        {
            emitter->setSize(emitter->getSizeStartMin(), emitter->getSizeStartMax(), _endMin->getValue(), emitter->getSizeEndMax());
        }
        else if (control == _endMax)
        {
            emitter->setSize(emitter->getSizeStartMin(), emitter->getSizeStartMax(), emitter->getSizeEndMin(), _endMax->getValue());
        }
        else if (control == _energyMin)
        {
            emitter->setEnergy(_energyMin->getValue(), emitter->getEnergyMax());
        }
        else if (control == _energyMax)
        {
            emitter->setEnergy(emitter->getEnergyMin(), _energyMax->getValue());
        }
        else if (control == _emissionRate)
        {
            emitter->setEmissionRate(_emissionRate->getValue());
        }
        else if (id == "posX")
        {
            Vector3 pos(emitter->getPosition());
            pos.x = ((Slider*)control)->getValue();
            emitter->setPosition(pos, emitter->getPositionVariance());
        }
        else if (id == "posY")
        {
            Vector3 pos(emitter->getPosition());
            pos.y = ((Slider*)control)->getValue();
            emitter->setPosition(pos, emitter->getPositionVariance());
        }
        else if (id == "posZ")
        {
            Vector3 pos(emitter->getPosition());
            pos.z = ((Slider*)control)->getValue();
            emitter->setPosition(pos, emitter->getPositionVariance());
        }
        else if (control == _posVarX)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.x = _posVarX->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
        }
        else if (control == _posVarY)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.y = _posVarY->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
        }
        else if (control == _posVarZ)
        {
            Vector3 posVar = emitter->getPositionVariance();
            posVar.z = _posVarZ->getValue();
            emitter->setPosition(emitter->getPosition(), posVar);
        }
        else if (control == _velX)
        {
            Vector3 vel = emitter->getVelocity();
            vel.x = _velX->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
        }
        else if (control == _velY)
        {
            Vector3 vel = emitter->getVelocity();
            vel.y = _velY->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
        }
        else if (control == _velZ)
        {
            Vector3 vel = emitter->getVelocity();
            vel.z = _velZ->getValue();
            emitter->setVelocity(vel, emitter->getVelocityVariance());
        }
        else if (control == _velVarX)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.x = _velVarX->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
        }
        else if (control == _velVarY)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.y = _velVarY->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
        }
        else if (control == _velVarZ)
        {
            Vector3 velVar = emitter->getVelocityVariance();
            velVar.z = _velVarZ->getValue();
            emitter->setVelocity(emitter->getVelocity(), velVar);
        }
        else if (control == _accelX)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.x = _accelX->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
        }
        else if (control == _accelY)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.y = _accelY->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
        }
        else if (control == _accelZ)
        {
            Vector3 accel = emitter->getAcceleration();
            accel.z = _accelZ->getValue();
            emitter->setAcceleration(accel, emitter->getAccelerationVariance());
        }
        else if (control == _accelVarX)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.x = _accelVarX->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
        }
        else if (control == _accelVarY)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.y = _accelVarY->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
        }
        else if (control == _accelVarZ)
        {
            Vector3 accelVar = emitter->getAccelerationVariance();
            accelVar.z = _accelVarZ->getValue();
            emitter->setAcceleration(emitter->getAcceleration(), accelVar);
        }
        else if (control == _spinSpeedMin)
        {
            emitter->setRotationPerParticle(_spinSpeedMin->getValue(), emitter->getRotationPerParticleSpeedMax());
        }
        else if (control == _spinSpeedMax)
        {
            emitter->setRotationPerParticle(emitter->getRotationPerParticleSpeedMin(), _spinSpeedMax->getValue());
        }
        else if (control == _axisX)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.x = _axisX->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
        }
        else if (control == _axisY)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.y = _axisY->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
        }
        else if (control == _axisZ)
        {
            Vector3 axis = emitter->getRotationAxis();
            axis.z = _axisZ->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), axis, emitter->getRotationAxisVariance());
        }
        else if (control == _axisVarX)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.x = _axisVarX->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
        }
        else if (control == _axisVarY)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.y = _axisVarY->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
        }
        else if (control == _axisVarZ)
        {
            Vector3 axisVar = emitter->getRotationAxisVariance();
            axisVar.z = _axisVarZ->getValue();
            emitter->setRotation(emitter->getRotationSpeedMin(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), axisVar);
        }
        else if (control == _rotationSpeedMin)
        {
            emitter->setRotation(_rotationSpeedMin->getValue(), emitter->getRotationSpeedMax(), emitter->getRotationAxis(), emitter->getRotationAxisVariance());
        }
        else if (control == _rotationSpeedMax)
        {
            emitter->setRotation(emitter->getRotationSpeedMin(), _rotationSpeedMax->getValue(), emitter->getRotationAxis(), emitter->getRotationAxisVariance());
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
        else if (control == _vsync)
        {
            Game::getInstance()->setVsync(_vsync->isChecked());
        }
        else if (id == "additive")
        {
            if (((RadioButton*)control)->isSelected())
                emitter->setTextureBlending(ParticleEmitter::BLEND_ADDITIVE);
        }
        else if (id == "transparent")
        {
            if (((RadioButton*)control)->isSelected())
                emitter->setTextureBlending(ParticleEmitter::BLEND_TRANSPARENT);
        }
        else if (id == "multiply")
        {
            if (((RadioButton*)control)->isSelected())
                emitter->setTextureBlending(ParticleEmitter::BLEND_MULTIPLIED);
        }
        else if (id == "opaque")
        {
            if (((RadioButton*)control)->isSelected())
                emitter->setTextureBlending(ParticleEmitter::BLEND_OPAQUE);
        }
        break;
    case Listener::CLICK:
        if (control == _reset)
        {
            // Re-load the current emitter and reset the view
            _particleEmitter = ParticleEmitter::create(_url.c_str());
            emitterChanged();
        }
        else if (control == _emit)
        {
            // Emit a burst of particles.
            unsigned int burstSize = (unsigned int)_burstSize->getValue();
            emitter->emitOnce(burstSize);
        }
        else if (id == "sprite")
        {
            updateTexture();
        }
        else if (id == "updateFrames")
        {
            updateFrames();
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
        else if (control == _save)
        {
            Game::getInstance()->pause();
            saveFile();
            Game::getInstance()->resume();
        }
        else if (control == _load)
        {
            Game::getInstance()->pause();
            std::string filename = FileSystem::displayFileDialog(FileSystem::OPEN, "Select Particle File", "Particle Files", "particle", "res");
            if (filename.length() > 0)
            {
                _url = filename;
                _particleEmitter = ParticleEmitter::create(_url.c_str());
                emitterChanged();
            }
            Game::getInstance()->resume();
        }
        break;
    }
}

void ParticlesGame::updateFrames()
{
    Texture* texture = _particleEmitter->getTexture();
    TextBox* cBox = (TextBox*)_form->getControl("frameCount");
    TextBox* wBox = (TextBox*)_form->getControl("frameWidth");
    TextBox* hBox = (TextBox*)_form->getControl("frameHeight");
    unsigned int fc = (unsigned int)atoi(cBox->getText());
    unsigned int w = (unsigned int)atoi(wBox->getText());
    unsigned int h = (unsigned int)atoi(hBox->getText());
    if (fc > 0 && fc < 256 && fc < 1000 && w > 0 && h > 0 && w < 4096 && h < 4096)
    {
        if (w > _particleEmitter->getTexture()->getWidth())
        {
            wBox->setText(toString(texture->getWidth()).c_str());
        }
        if (h > texture->getHeight())
        {
            hBox->setText(toString(texture->getHeight()).c_str());
        }

        _particleEmitter->setSpriteFrameCoords(fc, w, h);
    }
}

void ParticlesGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_particleEmitter);
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
}

void ParticlesGame::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, BACKGROUND_COLOR, 1.0f, 0);

    // Visit all the nodes in the scene for drawing.
    _scene->visit(this, &ParticlesGame::drawScene, (void*)0);

    // Draw the UI.
    _form->draw();

    // Draw the framerate and number of live particles.
    drawFrameRate(_font, Vector4(1, 1, 1, 1), 205, 40, getFrameRate());
}

bool ParticlesGame::drawScene(Node* node, void* cookie)
{
    if (node->getModel())
        node->getModel()->draw();
    if (node->getParticleEmitter())
        node->getParticleEmitter()->draw();
    return true;
}

bool ParticlesGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_PRESS_MIDDLE_BUTTON:
        Game::getInstance()->setMouseCaptured(true);
        _panning = true;
        return true;
    case Mouse::MOUSE_RELEASE_MIDDLE_BUTTON:
        Game::getInstance()->setMouseCaptured(false);
        _panning = false;
        return true;
    case Mouse::MOUSE_PRESS_LEFT_BUTTON:
        Game::getInstance()->setMouseCaptured(true);
        _rotating = true;
        return true;
    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        Game::getInstance()->setMouseCaptured(false);
        _rotating = false;
        return true;
    case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
        Game::getInstance()->setMouseCaptured(true);
        _zooming = true;
        return true;
    case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
        Game::getInstance()->setMouseCaptured(false);
        _zooming = false;
        return true;
    case Mouse::MOUSE_MOVE:
        if (_panning)
        {
            Vector3 n(-(float)x * PANNING_SENSITIVITY, (float)y * PANNING_SENSITIVITY, 0);
            _cameraParent->getMatrix().transformVector(&n);
            _cameraParent->translate(n);
            return true;
        }
        else if (_rotating)
        {
            _cameraParent->rotateY(-MATH_DEG_TO_RAD((float)x * ROTATE_SENSITIVITY));
            _cameraParent->rotateX(-MATH_DEG_TO_RAD((float)y * ROTATE_SENSITIVITY));
            return true;
        }
        else if (_zooming)
        {
            Vector3 v = _scene->getActiveCamera()->getNode()->getForwardVector();
            v.normalize();
            v.scale((float)(x-y) * INPUT_SENSITIVITY);
            _scene->getActiveCamera()->getNode()->translate(v);
            return true;
        }
        break;

    case Mouse::MOUSE_WHEEL:
        if (wheelDelta != 0)
        {
            Vector3 v = _scene->getActiveCamera()->getNode()->getForwardVector();
            v.normalize();
            v.scale((float)(wheelDelta));
            _scene->getActiveCamera()->getNode()->translate(v);
            return true;
        }
        break;
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
    // Load the default particle emitter
    _url = DEFAULT_PARTICLE_EMITTER;
    _particleEmitter = ParticleEmitter::create(_url.c_str());

    _particleEmitterNode = _scene->addNode("Particle Emitter");
    _particleEmitterNode->setTranslation(0.0f, 0.0f, 0.0f);

    emitterChanged();
}

void ParticlesGame::emitterChanged()
{
    ParticleEmitter* emitter = _particleEmitter;

    // Set the new emitter on the node.
    _particleEmitterNode->setParticleEmitter(_particleEmitter);
    _particleEmitter->release();

    // Reset camera view and zoom.
    _scene->getActiveCamera()->getNode()->setTranslation(0.0f, 0.0f, 40.0f);
    _cameraParent->setIdentity();
    _particleEmitterNode->setIdentity();

    // Parse editor section of particle properties
    Properties* p = Properties::create(_url.c_str());
    Properties* ns = p->getNamespace("editor", true);
    if (ns)
    {
        Vector3 v3;
        if (ns->getVector3("cameraTranslation", &v3))
        {
            _cameraParent->setTranslation(v3);
        }
        if (ns->getVector3("cameraZoom", &v3))
        {
            _scene->getActiveCamera()->getNode()->setTranslation(v3);
        }
        Quaternion q;
        if (ns->getQuaternionFromAxisAngle("cameraRotation", &q))
        {
            _cameraParent->setRotation(q);
        }
        float f;
        if ((f = ns->getFloat("sizeMax")) != 0.0f)
        {
            _startMin->setMax(f);
            _startMax->setMax(f);
            _endMin->setMax(f);
            _endMax->setMax(f);
        }
        if ((f = ns->getFloat("energyMax")) != 0.0f)
        {
            _energyMin->setMax(f);
            _energyMax->setMax(f);
        }
    }
    SAFE_DELETE(p);

    // Set the values of UI controls to display the new emitter's settings.
    _startRed->setValue(emitter->getColorStart().x);
    _startGreen->setValue(emitter->getColorStart().y);
    _startBlue->setValue(emitter->getColorStart().z);
    _startAlpha->setValue(emitter->getColorStart().w);    

    _endRed->setValue(emitter->getColorEnd().x);
    _endGreen->setValue(emitter->getColorEnd().y);
    _endBlue->setValue(emitter->getColorEnd().z);
    _endAlpha->setValue(emitter->getColorEnd().w);

    _startMin->setValue(emitter->getSizeStartMin());
    _startMax->setValue(emitter->getSizeStartMax());
    _endMin->setValue(emitter->getSizeEndMin());
    _endMax->setValue(emitter->getSizeEndMax());

    _energyMin->setValue(emitter->getEnergyMin());
    _energyMax->setValue(emitter->getEnergyMax());

    _emissionRate->setValue(emitter->getEmissionRate());

    const Vector3& posVar = emitter->getPositionVariance();
    _posVarX->setValue(posVar.x);
    _posVarY->setValue(posVar.y);
    _posVarZ->setValue(posVar.z);

    const Vector3& vel = emitter->getVelocity();
    _velX->setValue(vel.x);    
    _velY->setValue(vel.y);
    _velZ->setValue(vel.z);

    const Vector3& velVar = emitter->getVelocityVariance();
    _velVarX->setValue(velVar.x);
    _velVarY->setValue(velVar.y);
    _velVarZ->setValue(velVar.z);

    const Vector3& accel = emitter->getAcceleration();
    _accelX->setValue(accel.x);
    _accelY->setValue(accel.y);
    _accelZ->setValue(accel.z);

    const Vector3& accelVar = emitter->getAccelerationVariance();
    _accelVarX->setValue(accelVar.x);
    _accelVarY->setValue(accelVar.y);
    _accelVarZ->setValue(accelVar.z);

    _spinSpeedMin->setValue(emitter->getRotationPerParticleSpeedMin());
    _spinSpeedMax->setValue(emitter->getRotationPerParticleSpeedMax());

    const Vector3& axis = emitter->getRotationAxis();
    _axisX->setValue(axis.x);
    _axisY->setValue(axis.y);    
    _axisZ->setValue(axis.z);


    const Vector3& axisVar = emitter->getRotationAxisVariance();
    _axisVarX->setValue(axisVar.x);
    _axisVarY->setValue(axisVar.y);
    _axisVarZ->setValue(axisVar.z);

    _rotationSpeedMin->setValue(emitter->getRotationSpeedMin());
    _rotationSpeedMax->setValue(emitter->getRotationSpeedMax());

    // Update our image control
    updateImageControl();

    // Start the emitter
    emitter->start();
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
    font->drawText(buffer, x, y, color, 22);
    font->finish();
}

void ParticlesGame::resizeEvent(unsigned int width, unsigned int height)
{
    setViewport(gameplay::Rectangle(width, height));
    _form->setSize(width, height);
    _scene->getActiveCamera()->setAspectRatio((float)getWidth() / (float)getHeight());
}

void ParticlesGame::updateTexture()
{
    std::string file = FileSystem::displayFileDialog(FileSystem::OPEN, "Select Texture", "Texture Files", "png", "res");
    if (file.length() > 0)
    {
        // Set new sprite on our emitter
        _particleEmitter->setTexture(file.c_str(), _particleEmitter->getTextureBlending());

        // Update the UI to display the new sprite
        updateImageControl();
    }
}

void ParticlesGame::updateImageControl()
{
    ImageControl* img = (ImageControl*)_form->getControl("sprite");
    img->setImage(_particleEmitter->getTexture()->getPath());

    // Resize the image control so keep it to scale
    int w = _particleEmitter->getTexture()->getWidth();
    int h = _particleEmitter->getTexture()->getHeight();
    int max = w > h ? w : h;
    if (max > 120)
    {
        float ratio = 120.0f / max;
        w *= ratio;
        h *= ratio;
    }
    img->setSize(w, h);
    _form->getControl("image")->setHeight(h + _form->getControl("imageSettings")->getHeight() + 50);

    ((TextBox*)_form->getControl("frameCount"))->setText(toString(_particleEmitter->getSpriteFrameCount()).c_str());
    ((TextBox*)_form->getControl("frameWidth"))->setText(toString(_particleEmitter->getSpriteWidth()).c_str());
    ((TextBox*)_form->getControl("frameHeight"))->setText(toString(_particleEmitter->getSpriteHeight()).c_str());

    switch (_particleEmitter->getTextureBlending())
    {
    case ParticleEmitter::BLEND_ADDITIVE:
        ((RadioButton*)_form->getControl("additive"))->setSelected(true);
        break;
    case ParticleEmitter::BLEND_MULTIPLIED:
        ((RadioButton*)_form->getControl("multiply"))->setSelected(true);
        break;
    case ParticleEmitter::BLEND_OPAQUE:
        ((RadioButton*)_form->getControl("opaque"))->setSelected(true);
        break;
    case ParticleEmitter::BLEND_TRANSPARENT:
        ((RadioButton*)_form->getControl("transparent"))->setSelected(true);
        break;
    }
}
