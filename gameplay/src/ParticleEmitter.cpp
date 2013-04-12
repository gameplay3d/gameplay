#include "Base.h"
#include "ParticleEmitter.h"
#include "Game.h"
#include "Node.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Properties.h"

#define PARTICLE_COUNT_MAX                       100
#define PARTICLE_EMISSION_RATE                   10
#define PARTICLE_EMISSION_RATE_TIME_INTERVAL     1000.0f / (float)PARTICLE_EMISSION_RATE

namespace gameplay
{

ParticleEmitter::ParticleEmitter(SpriteBatch* batch, unsigned int particleCountMax) :
    _particleCountMax(particleCountMax), _particleCount(0), _particles(NULL),
    _emissionRate(PARTICLE_EMISSION_RATE), _started(false), _ellipsoid(false),
    _sizeStartMin(1.0f), _sizeStartMax(1.0f), _sizeEndMin(1.0f), _sizeEndMax(1.0f),
    _energyMin(1000L), _energyMax(1000L),
    _colorStart(Vector4::zero()), _colorStartVar(Vector4::zero()), _colorEnd(Vector4::one()), _colorEndVar(Vector4::zero()),
    _position(Vector3::zero()), _positionVar(Vector3::zero()),
    _velocity(Vector3::zero()), _velocityVar(Vector3::one()),
    _acceleration(Vector3::zero()), _accelerationVar(Vector3::zero()),
    _rotationPerParticleSpeedMin(0.0f), _rotationPerParticleSpeedMax(0.0f),
    _rotationSpeedMin(0.0f), _rotationSpeedMax(0.0f),
    _rotationAxis(Vector3::zero()), _rotation(Matrix::identity()),
    _spriteBatch(batch), _spriteTextureBlending(BLEND_TRANSPARENT),  _spriteTextureWidth(0), _spriteTextureHeight(0), _spriteTextureWidthRatio(0), _spriteTextureHeightRatio(0), _spriteTextureCoords(NULL),
    _spriteAnimated(false),  _spriteLooped(false), _spriteFrameCount(1), _spriteFrameRandomOffset(0),_spriteFrameDuration(0L), _spriteFrameDurationSecs(0.0f), _spritePercentPerFrame(0.0f),
    _node(NULL), _orbitPosition(false), _orbitVelocity(false), _orbitAcceleration(false),
    _timePerEmission(PARTICLE_EMISSION_RATE_TIME_INTERVAL), _timeRunning(0)
{
    GP_ASSERT(particleCountMax);
    _particles = new Particle[particleCountMax];

    GP_ASSERT(_spriteBatch);
    GP_ASSERT(_spriteBatch->getStateBlock());
    _spriteBatch->getStateBlock()->setDepthWrite(false);
    _spriteBatch->getStateBlock()->setDepthTest(true);
}

ParticleEmitter::~ParticleEmitter()
{
    SAFE_DELETE(_spriteBatch);
    SAFE_DELETE_ARRAY(_particles);
    SAFE_DELETE_ARRAY(_spriteTextureCoords);
}

ParticleEmitter* ParticleEmitter::create(const char* textureFile, TextureBlending textureBlending, unsigned int particleCountMax)
{
    Texture* texture = Texture::create(textureFile, false);

    if (!texture)
    {
        GP_ERROR("Failed to create texture for particle emitter.");
        return NULL;
    }
    GP_ASSERT(texture->getWidth());
    GP_ASSERT(texture->getHeight());

    // Use default SpriteBatch material.
    SpriteBatch* batch =  SpriteBatch::create(texture, NULL, particleCountMax);
    texture->release(); // batch owns the texture.
    GP_ASSERT(batch);

    ParticleEmitter* emitter = new ParticleEmitter(batch, particleCountMax);
    GP_ASSERT(emitter);

    // By default assume only one frame which uses the entire texture.
    emitter->setTextureBlending(textureBlending);
    emitter->_spriteTextureWidth = texture->getWidth();
    emitter->_spriteTextureHeight = texture->getHeight();
    emitter->_spriteTextureWidthRatio = 1.0f / (float)texture->getWidth();
    emitter->_spriteTextureHeightRatio = 1.0f / (float)texture->getHeight();

    Rectangle texCoord((float)texture->getWidth(), (float)texture->getHeight());
    emitter->setSpriteFrameCoords(1, &texCoord);

    return emitter;
}

ParticleEmitter* ParticleEmitter::create(const char* url)
{
    Properties* properties = Properties::create(url);
    if (!properties)
    {
        GP_ERROR("Failed to create particle emitter from file.");
        return NULL;
    }

    ParticleEmitter* particle = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
    SAFE_DELETE(properties);

    return particle;
}

ParticleEmitter* ParticleEmitter::create(Properties* properties)
{
    if (!properties || strcmp(properties->getNamespace(), "particle") != 0)
    {
        GP_ERROR("Properties object must be non-null and have namespace equal to 'particle'.");
        return NULL;
    }

    Properties* sprite = properties->getNextNamespace();
    if (!sprite || strcmp(sprite->getNamespace(), "sprite") != 0)
    {
        GP_ERROR("Failed to load particle emitter: required namespace 'sprite' is missing.");
        return NULL;
    }

    // Load sprite properties.
    // Path to image file is required.
    std::string texturePath;
    if (!sprite->getPath("path", &texturePath))
    {
        GP_ERROR("Failed to load particle emitter: required image file path ('path') is missing.");
        return NULL;
    }

    const char* blendingString = sprite->getString("blending");
    TextureBlending textureBlending = getTextureBlendingFromString(blendingString);
    int spriteWidth = sprite->getInt("width");
    int spriteHeight = sprite->getInt("height");
    bool spriteAnimated = sprite->getBool("animated");
    bool spriteLooped = sprite->getBool("looped");
    int spriteFrameCount = sprite->getInt("frameCount");
    int spriteFrameRandomOffset = sprite->getInt("frameRandomOffset");
    float spriteFrameDuration = sprite->getFloat("frameDuration");

    // Emitter properties.
    unsigned int particleCountMax = (unsigned int)properties->getInt("particleCountMax");
    if (particleCountMax == 0)
    {
        // Set sensible default.
        particleCountMax = PARTICLE_COUNT_MAX;
    }

    unsigned int emissionRate = (unsigned int)properties->getInt("emissionRate");
    if (emissionRate == 0)
    {
        emissionRate = PARTICLE_EMISSION_RATE;
    }

    bool ellipsoid = properties->getBool("ellipsoid");
    float sizeStartMin = properties->getFloat("sizeStartMin");
    float sizeStartMax = properties->getFloat("sizeStartMax");
    float sizeEndMin = properties->getFloat("sizeEndMin");
    float sizeEndMax = properties->getFloat("sizeEndMax");
    long energyMin = properties->getLong("energyMin");
    long energyMax = properties->getLong("energyMax");

    Vector4 colorStart;
    Vector4 colorStartVar;
    Vector4 colorEnd;
    Vector4 colorEndVar;
    properties->getVector4("colorStart", &colorStart);
    properties->getVector4("colorStartVar", &colorStartVar);
    properties->getVector4("colorEnd", &colorEnd);
    properties->getVector4("colorEndVar", &colorEndVar);

    Vector3 position;
    Vector3 positionVar;
    Vector3 velocity;
    Vector3 velocityVar;
    Vector3 acceleration;
    Vector3 accelerationVar;
    Vector3 rotationAxis;
    Vector3 rotationAxisVar;
    properties->getVector3("position", &position);
    properties->getVector3("positionVar", &positionVar);
    properties->getVector3("velocity", &velocity);
    properties->getVector3("velocityVar", &velocityVar);
    properties->getVector3("acceleration", &acceleration);
    properties->getVector3("accelerationVar", &accelerationVar);
    float rotationPerParticleSpeedMin = properties->getFloat("rotationPerParticleSpeedMin");
    float rotationPerParticleSpeedMax = properties->getFloat("rotationPerParticleSpeedMax");
    float rotationSpeedMin = properties->getFloat("rotationSpeedMin");
    float rotationSpeedMax = properties->getFloat("rotationSpeedMax");
    properties->getVector3("rotationAxis", &rotationAxis);
    properties->getVector3("rotationAxisVar", &rotationAxisVar);
    bool orbitPosition = properties->getBool("orbitPosition");
    bool orbitVelocity = properties->getBool("orbitVelocity");
    bool orbitAcceleration = properties->getBool("orbitAcceleration");

    // Apply all properties to a newly created ParticleEmitter.
    ParticleEmitter* emitter = ParticleEmitter::create(texturePath.c_str(), textureBlending, particleCountMax);
    if (!emitter)
    {
        GP_ERROR("Failed to create particle emitter.");
        return NULL;
    }
    emitter->setEmissionRate(emissionRate);
    emitter->setEllipsoid(ellipsoid);
    emitter->setSize(sizeStartMin, sizeStartMax, sizeEndMin, sizeEndMax);
    emitter->setEnergy(energyMin, energyMax);
    emitter->setColor(colorStart, colorStartVar, colorEnd, colorEndVar);
    emitter->setPosition(position, positionVar);
    emitter->setVelocity(velocity, velocityVar);
    emitter->setAcceleration(acceleration, accelerationVar);
    emitter->setRotationPerParticle(rotationPerParticleSpeedMin, rotationPerParticleSpeedMax);
    emitter->setRotation(rotationSpeedMin, rotationSpeedMax, rotationAxis, rotationAxisVar);

    emitter->setSpriteAnimated(spriteAnimated);
    emitter->setSpriteLooped(spriteLooped);
    emitter->setSpriteFrameRandomOffset(spriteFrameRandomOffset);
    emitter->setSpriteFrameDuration(spriteFrameDuration);
    emitter->setSpriteFrameCoords(spriteFrameCount, spriteWidth, spriteHeight);

    emitter->setOrbit(orbitPosition, orbitVelocity, orbitAcceleration);

    return emitter;
}

unsigned int ParticleEmitter::getEmissionRate() const
{
    return _emissionRate;
}

void ParticleEmitter::setEmissionRate(unsigned int rate)
{
    GP_ASSERT(rate);
    _emissionRate = rate;
    _timePerEmission = 1000.0f / (float)_emissionRate;
}

void ParticleEmitter::start()
{
    _started = true;
}

void ParticleEmitter::stop()
{
    _started = false;
}

bool ParticleEmitter::isStarted() const
{
    return _started;
}

bool ParticleEmitter::isActive() const
{
    if (_started)
        return true;

    if (!_node)
        return false;

    GP_ASSERT(_particles);
    bool active = false;
    for (unsigned int i = 0; i < _particleCount; i++)
    {
        if (_particles[i]._energy > 0)
        {
            active = true;
            break;
        }
    }

    return active;
}

void ParticleEmitter::emitOnce(unsigned int particleCount)
{
    GP_ASSERT(_node);
    GP_ASSERT(_particles);

    // Limit particleCount so as not to go over _particleCountMax.
    if (particleCount + _particleCount > _particleCountMax)
    {
        particleCount = _particleCountMax - _particleCount;
    }

    Vector3 translation;
    Matrix world = _node->getWorldMatrix();
    world.getTranslation(&translation);

    // Take translation out of world matrix so it can be used to rotate orbiting properties.
    world.m[12] = 0.0f;
    world.m[13] = 0.0f;
    world.m[14] = 0.0f;

    // Emit the new particles.
    for (unsigned int i = 0; i < particleCount; i++)
    {
        Particle* p = &_particles[_particleCount];
        p->_visible = true;

        generateColor(_colorStart, _colorStartVar, &p->_colorStart);
        generateColor(_colorEnd, _colorEndVar, &p->_colorEnd);
        p->_color.set(p->_colorStart);

        p->_energy = p->_energyStart = generateScalar(_energyMin, _energyMax);
        p->_size = p->_sizeStart = generateScalar(_sizeStartMin, _sizeStartMax);
        p->_sizeEnd = generateScalar(_sizeEndMin, _sizeEndMax);
        p->_rotationPerParticleSpeed = generateScalar(_rotationPerParticleSpeedMin, _rotationPerParticleSpeedMax);
        p->_angle = generateScalar(0.0f, p->_rotationPerParticleSpeed);
        p->_rotationSpeed = generateScalar(_rotationSpeedMin, _rotationSpeedMax);

        // Only initial position can be generated within an ellipsoidal domain.
        generateVector(_position, _positionVar, &p->_position, _ellipsoid);
        generateVector(_velocity, _velocityVar, &p->_velocity, false);
        generateVector(_acceleration, _accelerationVar, &p->_acceleration, false);
        generateVector(_rotationAxis, _rotationAxisVar, &p->_rotationAxis, false);

        // Initial position, velocity and acceleration can all be relative to the emitter's transform.
        // Rotate specified properties by the node's rotation.
        if (_orbitPosition)
        {
            world.transformPoint(p->_position, &p->_position);
        }

        if (_orbitVelocity)
        {
            world.transformPoint(p->_velocity, &p->_velocity);
        }

        if (_orbitAcceleration)
        {
            world.transformPoint(p->_acceleration, &p->_acceleration);
        }

        // The rotation axis always orbits the node.
        if (p->_rotationSpeed != 0.0f && !p->_rotationAxis.isZero())
        {
            world.transformPoint(p->_rotationAxis, &p->_rotationAxis);
        }

        // Translate position relative to the node's world space.
        p->_position.add(translation);

        // Initial sprite frame.
        if (_spriteFrameRandomOffset > 0)
        {
            p->_frame = rand() % _spriteFrameRandomOffset;
        }
        else
        {
            p->_frame = 0;
        }
        p->_timeOnCurrentFrame = 0.0f;

        ++_particleCount;
    }
}

unsigned int ParticleEmitter::getParticlesCount() const
{
    return _particleCount;
}

void ParticleEmitter::setEllipsoid(bool ellipsoid)
{
    _ellipsoid = ellipsoid;
}

bool ParticleEmitter::isEllipsoid() const
{
    return _ellipsoid;
}

void ParticleEmitter::setSize(float startMin, float startMax, float endMin, float endMax)
{
    _sizeStartMin = startMin;
    _sizeStartMax = startMax;
    _sizeEndMin = endMin;
    _sizeEndMax = endMax;
}

float ParticleEmitter::getSizeStartMin() const
{
    return _sizeStartMin;
}

float ParticleEmitter::getSizeStartMax() const
{
    return _sizeStartMax;
}

float ParticleEmitter::getSizeEndMin() const
{
    return _sizeEndMin;
}

float ParticleEmitter::getSizeEndMax() const
{
    return _sizeEndMax;
}

void ParticleEmitter::setEnergy(long energyMin, long energyMax)
{
    _energyMin = energyMin;
    _energyMax = energyMax;
}

long ParticleEmitter::getEnergyMin() const
{
    return _energyMin;
}

long ParticleEmitter::getEnergyMax() const
{
    return _energyMax;
}

void ParticleEmitter::setColor(const Vector4& startColor, const Vector4& startColorVar, const Vector4& endColor, const Vector4& endColorVar)
{
    _colorStart.set(startColor);
    _colorStartVar.set(startColorVar);
    _colorEnd.set(endColor);
    _colorEndVar.set(endColorVar);
}

const Vector4& ParticleEmitter::getColorStart() const
{
    return _colorStart;
}

const Vector4& ParticleEmitter::getColorStartVariance() const
{
    return _colorStartVar;
}

const Vector4& ParticleEmitter::getColorEnd() const
{
    return _colorEnd;
}

const Vector4& ParticleEmitter::getColorEndVariance() const
{
    return _colorEndVar;
}

void ParticleEmitter::setPosition(const Vector3& position, const Vector3& positionVar)
{
    _position.set(position);
    _positionVar.set(positionVar);
}

const Vector3& ParticleEmitter::getPosition() const
{
    return _position;
}

const Vector3& ParticleEmitter::getPositionVariance() const
{
    return _positionVar;
}

const Vector3& ParticleEmitter::getVelocity() const
{
    return _velocity;
}

const Vector3& ParticleEmitter::getVelocityVariance() const
{
    return _velocityVar;
}

void ParticleEmitter::setVelocity(const Vector3& velocity, const Vector3& velocityVar)
{
    _velocity.set(velocity);
    _velocityVar.set(velocityVar);
}

const Vector3& ParticleEmitter::getAcceleration() const
{
    return _acceleration;
}

const Vector3& ParticleEmitter::getAccelerationVariance() const
{
    return _accelerationVar;
}

void ParticleEmitter::setAcceleration(const Vector3& acceleration, const Vector3& accelerationVar)
{
    _acceleration.set(acceleration);
    _accelerationVar.set(accelerationVar);
}

void ParticleEmitter::setRotationPerParticle(float speedMin, float speedMax)
{
    _rotationPerParticleSpeedMin = speedMin;
    _rotationPerParticleSpeedMax = speedMax;
}

float ParticleEmitter::getRotationPerParticleSpeedMin() const
{
    return _rotationPerParticleSpeedMin;
}

float ParticleEmitter::getRotationPerParticleSpeedMax() const
{
    return _rotationPerParticleSpeedMax;
}

void ParticleEmitter::setRotation(float speedMin, float speedMax, const Vector3& axis, const Vector3& axisVariance)
{
    _rotationSpeedMin = speedMin;
    _rotationSpeedMax = speedMax;
    _rotationAxis.set(axis);
    _rotationAxisVar.set(axisVariance);
}

float ParticleEmitter::getRotationSpeedMin() const
{
    return _rotationSpeedMin;
}

float ParticleEmitter::getRotationSpeedMax() const
{
    return _rotationSpeedMax;
}

const Vector3& ParticleEmitter::getRotationAxis() const
{
    return _rotationAxis;
}

const Vector3& ParticleEmitter::getRotationAxisVariance() const
{
    return _rotationAxisVar;
}

void ParticleEmitter::setTextureBlending(TextureBlending textureBlending)
{
    GP_ASSERT(_spriteBatch);
    GP_ASSERT(_spriteBatch->getStateBlock());

    switch (textureBlending)
    {
        case BLEND_OPAQUE:
            _spriteBatch->getStateBlock()->setBlend(false);
            break;
        case BLEND_TRANSPARENT:
            _spriteBatch->getStateBlock()->setBlend(true);
            _spriteBatch->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
            _spriteBatch->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
            break;
        case BLEND_ADDITIVE:
            _spriteBatch->getStateBlock()->setBlend(true);
            _spriteBatch->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
            _spriteBatch->getStateBlock()->setBlendDst(RenderState::BLEND_ONE);
            break;
        case BLEND_MULTIPLIED:
            _spriteBatch->getStateBlock()->setBlend(true);
            _spriteBatch->getStateBlock()->setBlendSrc(RenderState::BLEND_ZERO);
            _spriteBatch->getStateBlock()->setBlendDst(RenderState::BLEND_SRC_COLOR);
            break;
        default:
            GP_ERROR("Unsupported texture blending mode (%d).", textureBlending);
            break;
    }
}

void ParticleEmitter::setSpriteAnimated(bool animated)
{
    _spriteAnimated = animated;
}

bool ParticleEmitter::isSpriteAnimated() const
{
    return _spriteAnimated;
}

void ParticleEmitter::setSpriteLooped(bool looped)
{
    _spriteLooped = looped;
}

bool ParticleEmitter::isSpriteLooped() const
{
    return _spriteLooped;
}

void ParticleEmitter::setSpriteFrameRandomOffset(int maxOffset)
{
    _spriteFrameRandomOffset = maxOffset;
}


int ParticleEmitter::getSpriteFrameRandomOffset() const
{
    return _spriteFrameRandomOffset;
}

void ParticleEmitter::setSpriteFrameDuration(long duration)
{
    _spriteFrameDuration = duration;
    _spriteFrameDurationSecs = (float)duration / 1000.0f;
}

long ParticleEmitter::getSpriteFrameDuration() const
{
    return _spriteFrameDuration;
}

void ParticleEmitter::setSpriteTexCoords(unsigned int frameCount, float* texCoords)
{
    GP_ASSERT(frameCount);
    GP_ASSERT(texCoords);

    _spriteFrameCount = frameCount;
    _spritePercentPerFrame = 1.0f / (float)frameCount;

    SAFE_DELETE_ARRAY(_spriteTextureCoords);
    _spriteTextureCoords = new float[frameCount * 4];
    memcpy(_spriteTextureCoords, texCoords, frameCount * 4 * sizeof(float));
}

void ParticleEmitter::setSpriteFrameCoords(unsigned int frameCount, Rectangle* frameCoords)
{
    GP_ASSERT(frameCount);
    GP_ASSERT(frameCoords);

    _spriteFrameCount = frameCount;
    _spritePercentPerFrame = 1.0f / (float)frameCount;

    SAFE_DELETE_ARRAY(_spriteTextureCoords);
    _spriteTextureCoords = new float[frameCount * 4];

    // Pre-compute texture coordinates from rects.
    for (unsigned int i = 0; i < frameCount; i++)
    {
        _spriteTextureCoords[i*4] = _spriteTextureWidthRatio * frameCoords[i].x;
        _spriteTextureCoords[i*4 + 1] = 1.0f - _spriteTextureHeightRatio * frameCoords[i].y;
        _spriteTextureCoords[i*4 + 2] = _spriteTextureCoords[i*4] + _spriteTextureWidthRatio * frameCoords[i].width;
        _spriteTextureCoords[i*4 + 3] = _spriteTextureCoords[i*4 + 1] - _spriteTextureHeightRatio * frameCoords[i].height;
    }
}

void ParticleEmitter::setSpriteFrameCoords(unsigned int frameCount, int width, int height)
{
    GP_ASSERT(width);
    GP_ASSERT(height);

    int x;
    int y;
    Rectangle* frameCoords = new Rectangle[frameCount];
    unsigned int cols = _spriteTextureWidth / width;
    unsigned int rows = _spriteTextureHeight / height;

    unsigned int n = 0;
    for (unsigned int i = 0; i < rows; ++i)
    {
        y = i * height;
        for (unsigned int j = 0; j < cols; ++j)
        {
            x = j * width;
            frameCoords[i*cols + j] = Rectangle(x, y, width, height);
            if (++n == frameCount)
            {
                break;
            }
        }

        if (n == frameCount)
        {
            break;
        }
    }

    setSpriteFrameCoords(frameCount, frameCoords);

    SAFE_DELETE_ARRAY(frameCoords);
}

Node* ParticleEmitter::getNode() const
{
    return _node;
}

void ParticleEmitter::setNode(Node* node)
{
    // Connect the new node.
    _node = node;
}

void ParticleEmitter::setOrbit(bool orbitPosition, bool orbitVelocity, bool orbitAcceleration)
{
    _orbitPosition = orbitPosition;
    _orbitVelocity = orbitVelocity;
    _orbitAcceleration = orbitAcceleration;
}

long ParticleEmitter::generateScalar(long min, long max)
{
    // Note: this is not a very good RNG, but it should be suitable for our purposes.
    long r = 0;
    for (unsigned int i = 0; i < sizeof(long)/sizeof(int); i++)
    {
        r = r << 8; // sizeof(int) * CHAR_BITS
        r |= rand();
    }

    // Now we have a random long between 0 and MAX_LONG.  We need to clamp it between min and max.
    r %= max - min;
    r += min;

    return r;
}

float ParticleEmitter::generateScalar(float min, float max)
{
    return min + (max - min) * MATH_RANDOM_0_1();
}

void ParticleEmitter::generateVectorInRect(const Vector3& base, const Vector3& variance, Vector3* dst)
{
    GP_ASSERT(dst);

    // Scale each component of the variance vector by a random float
    // between -1 and 1, then add this to the corresponding base component.
    dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
    dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
    dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
}

void ParticleEmitter::generateVectorInEllipsoid(const Vector3& center, const Vector3& scale, Vector3* dst)
{
    GP_ASSERT(dst);

    // Generate a point within a unit cube, then reject if the point is not in a unit sphere.
    do
    {
        dst->x = MATH_RANDOM_MINUS1_1();
        dst->y = MATH_RANDOM_MINUS1_1();
        dst->z = MATH_RANDOM_MINUS1_1();
    } while (dst->length() > 1.0f);
    
    // Scale this point by the scaling vector.
    dst->x *= scale.x;
    dst->y *= scale.y;
    dst->z *= scale.z;

    // Translate by the center point.
    dst->add(center);
}

void ParticleEmitter::generateVector(const Vector3& base, const Vector3& variance, Vector3* dst, bool ellipsoid)
{
    if (ellipsoid)
    {
        generateVectorInEllipsoid(base, variance, dst);
    }
    else
    {
        generateVectorInRect(base, variance, dst);
    }
}

void ParticleEmitter::generateColor(const Vector4& base, const Vector4& variance, Vector4* dst)
{
    GP_ASSERT(dst);

    // Scale each component of the variance color by a random float
    // between -1 and 1, then add this to the corresponding base component.
    dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
    dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
    dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
    dst->w = base.w + variance.w * MATH_RANDOM_MINUS1_1();
}

ParticleEmitter::TextureBlending ParticleEmitter::getTextureBlendingFromString(const char* str)
{
    GP_ASSERT(str);

    if (strcmp(str, "BLEND_OPAQUE") == 0 || strcmp(str, "OPAQUE") == 0)
    {
        return BLEND_OPAQUE;
    }
    else if (strcmp(str, "BLEND_TRANSPARENT") == 0 || strcmp(str, "TRANSPARENT") == 0)
    {
        return BLEND_TRANSPARENT;
    }
    else if (strcmp(str, "BLEND_ADDITIVE") == 0 || strcmp(str, "ADDITIVE") == 0)
    {
        return BLEND_ADDITIVE;
    }
    else if (strcmp(str, "BLEND_MULTIPLIED") == 0 || strcmp(str, "MULTIPLIED") == 0)
    {
        return BLEND_MULTIPLIED;
    }
    else
    {
        return BLEND_TRANSPARENT;
    }
}

void ParticleEmitter::update(float elapsedTime)
{
    if (!isActive())
    {
        return;
    }

    // Calculate the time passed since last update.
    float elapsedSecs = elapsedTime * 0.001f;

    if (_started && _emissionRate)
    {
        // Calculate how much time has passed since we last emitted particles.
        _timeRunning += elapsedTime;

        // How many particles should we emit this frame?
        GP_ASSERT(_timePerEmission);
        unsigned int emitCount = (unsigned int)(_timeRunning / _timePerEmission);

        if (emitCount)
        {
            if ((int)_timePerEmission > 0)
            {
                _timeRunning = fmod(_timeRunning, (double)_timePerEmission);
            }
            emitOnce(emitCount);
        }
    }

    GP_ASSERT(_node && _node->getScene() && _node->getScene()->getActiveCamera());
    const Frustum& frustum = _node->getScene()->getActiveCamera()->getFrustum();

    // Now update all currently living particles.
    GP_ASSERT(_particles);
    for (unsigned int particlesIndex = 0; particlesIndex < _particleCount; ++particlesIndex)
    {
        Particle* p = &_particles[particlesIndex];
        p->_energy -= elapsedTime;

        if (p->_energy > 0L)
        {
            if (p->_rotationSpeed != 0.0f && !p->_rotationAxis.isZero())
            {
                Matrix::createRotation(p->_rotationAxis, p->_rotationSpeed * elapsedSecs, &_rotation);

                _rotation.transformPoint(p->_velocity, &p->_velocity);
                _rotation.transformPoint(p->_acceleration, &p->_acceleration);
            }

            // Particle is still alive.
            p->_velocity.x += p->_acceleration.x * elapsedSecs;
            p->_velocity.y += p->_acceleration.y * elapsedSecs;
            p->_velocity.z += p->_acceleration.z * elapsedSecs;

            p->_position.x += p->_velocity.x * elapsedSecs;
            p->_position.y += p->_velocity.y * elapsedSecs;
            p->_position.z += p->_velocity.z * elapsedSecs;

            if (!frustum.intersects(p->_position))
            {
                p->_visible = false;
                continue;
            }

            p->_angle += p->_rotationPerParticleSpeed * elapsedSecs;

            // Simple linear interpolation of color and size.
            float percent = 1.0f - ((float)p->_energy / (float)p->_energyStart);

            p->_color.x = p->_colorStart.x + (p->_colorEnd.x - p->_colorStart.x) * percent;
            p->_color.y = p->_colorStart.y + (p->_colorEnd.y - p->_colorStart.y) * percent;
            p->_color.z = p->_colorStart.z + (p->_colorEnd.z - p->_colorStart.z) * percent;
            p->_color.w = p->_colorStart.w + (p->_colorEnd.w - p->_colorStart.w) * percent;

            p->_size = p->_sizeStart + (p->_sizeEnd - p->_sizeStart) * percent;

            // Handle sprite animations.
            if (_spriteAnimated)
            {
                if (!_spriteLooped)
                {
                    // The last frame should finish exactly when the particle dies.
                    float percentSpent = 0.0f;
                    for (unsigned int i = 0; i < p->_frame; i++)
                    {
                        percentSpent += _spritePercentPerFrame;
                    }
                    p->_timeOnCurrentFrame = percent - percentSpent;
                    if (p->_frame < _spriteFrameCount - 1 &&
                        p->_timeOnCurrentFrame >= _spritePercentPerFrame)
                    {
                        ++p->_frame;
                    }
                }
                else
                {
                    // _spriteFrameDurationSecs is an absolute time measured in seconds,
                    // and the animation repeats indefinitely.
                    p->_timeOnCurrentFrame += elapsedSecs;
                    if (p->_timeOnCurrentFrame >= _spriteFrameDurationSecs)
                    {
                        p->_timeOnCurrentFrame -= _spriteFrameDurationSecs;
                        ++p->_frame;
                        if (p->_frame == _spriteFrameCount)
                        {
                            p->_frame = 0;
                        }
                    }
                }
            }
        }
        else
        {
            // Particle is dead.  Move the particle furthest from the start of the array
            // down to take its place, and re-use the slot at the end of the list of living particles.
            if (particlesIndex != _particleCount - 1)
            {
                _particles[particlesIndex] = _particles[_particleCount - 1];
            }
            --_particleCount;
        }
    }
}

void ParticleEmitter::draw()
{
    if (!isActive())
    {
        return;
    }

    if (_particleCount > 0)
    {
        GP_ASSERT(_spriteBatch);
        GP_ASSERT(_particles);
        GP_ASSERT(_spriteTextureCoords);

        // Set our node's view projection matrix to this emitter's effect.
        if (_node)
        {
            _spriteBatch->setProjectionMatrix(_node->getViewProjectionMatrix());
        }

        // Begin sprite batch drawing
        _spriteBatch->start();

        // 2D Rotation.
        static const Vector2 pivot(0.5f, 0.5f);

        // 3D Rotation so that particles always face the camera.
        GP_ASSERT(_node && _node->getScene() && _node->getScene()->getActiveCamera() && _node->getScene()->getActiveCamera()->getNode());
        const Matrix& cameraWorldMatrix = _node->getScene()->getActiveCamera()->getNode()->getWorldMatrix();

        Vector3 right;
        cameraWorldMatrix.getRightVector(&right);
        Vector3 up;
        cameraWorldMatrix.getUpVector(&up);

        for (unsigned int i = 0; i < _particleCount; i++)
        {
            Particle* p = &_particles[i];

            if (p->_visible)
            {
                _spriteBatch->draw(p->_position, right, up, p->_size, p->_size,
                                   _spriteTextureCoords[p->_frame * 4], _spriteTextureCoords[p->_frame * 4 + 1], _spriteTextureCoords[p->_frame * 4 + 2], _spriteTextureCoords[p->_frame * 4 + 3],
                                   p->_color, pivot, p->_angle);
            }
        }

        // Render.
        _spriteBatch->finish();
    }
}

}
