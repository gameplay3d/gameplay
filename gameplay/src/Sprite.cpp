#include "Base.h"
#include "Sprite.h"
#include "Scene.h"

namespace gameplay
{

Sprite::Sprite()
    : _width(0), _height(0), _offset(OFFSET_BOTTOM_LEFT), _anchor(Vector2(0.5f, 0.5f)), _flipFlags(FLIP_NONE),
      _frames(NULL), _frameCount(1), _frameStride(0), _framePadding(1), _frameIndex(0),
      _opacity(1.0f), _color(Vector4::one()), _blendMode(BLEND_ALPHA), _batch(NULL), _node(NULL)
{
}

Sprite::~Sprite()
{
    if (_frames)
        SAFE_DELETE_ARRAY(_frames);
    if (_batch)
        SAFE_DELETE(_batch);
}

Sprite& Sprite::operator=(const Sprite& sprite)
{
    return *this;
}
    
Sprite* Sprite::create(const char* imagePath, float width, float height, Effect* effect)
{
    return Sprite::create(imagePath, width, height, Rectangle(0, 0, -1, -1), 1, effect);
}
    
Sprite* Sprite::create(const char* imagePath, float width, float height,
                       const Rectangle& source, unsigned int frameCount, Effect* effect)
{
    GP_ASSERT(imagePath != NULL);
    GP_ASSERT(width >= -1 && height >= -1);
    GP_ASSERT(source.width >= -1 && source.height >= -1);
    GP_ASSERT(frameCount > 0);
    
    SpriteBatch* batch = SpriteBatch::create(imagePath, effect);
    batch->getSampler()->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    batch->getSampler()->setFilterMode(Texture::Filter::LINEAR, Texture::Filter::LINEAR);
    batch->getStateBlock()->setDepthWrite(false);
    batch->getStateBlock()->setDepthTest(true);
    
    unsigned int imageWidth = batch->getSampler()->getTexture()->getWidth();
    unsigned int imageHeight = batch->getSampler()->getTexture()->getHeight();
    if (width == -1)
        width = imageWidth;
    if (height == -1)
        height = imageHeight;
    
    Sprite* sprite = new Sprite();
    sprite->_width = width;
    sprite->_height = height;
    sprite->_batch = batch;
    sprite->_frameCount = frameCount;
    sprite->_frames = new Rectangle[frameCount];
    sprite->_frames[0] = source;
    if (sprite->_frames[0].width == -1.0f)
        sprite->_frames[0].width = imageWidth;
    if (sprite->_frames[0].height == -1.0f)
        sprite->_frames[0].height = imageHeight;
    return sprite;
}

Sprite* Sprite::create(Properties* properties)
{    
    /*
     sprite spaceship
     {
         anchor = 0.5, 0.5
         color = 0,0,0,1
         opacity = 1.0f
         blendMode = BLEND_ALPHA
         path = res/foo/spaceship.png
         width = 100
         height = 100
         source = 0, 0, 70, 70
         flip = FLIP_VERTICAL
         frameCount = 1
     }
     */
    return NULL;
}
    
float Sprite::getWidth() const
{
    return _width;
}

float Sprite::getHeight() const
{
    return _height;
}
    
void Sprite::setOffset(Sprite::Offset offset)
{
    _offset = offset;
}

Sprite::Offset Sprite::getOffset() const
{
    return _offset;
}
    
void Sprite::setAnchor(const Vector2& anchor)
{
    _anchor = anchor;
}

const Vector2& Sprite::getAnchor() const
{
    return _anchor;
}
    
void Sprite::setFlip(int flipFlags)
{
    _flipFlags = flipFlags;
}

int Sprite::getFlip() const
{
    return _flipFlags;
}

void Sprite::setFrameSource(unsigned int frameIndex, const Rectangle& source)
{
    GP_ASSERT(frameIndex < _frameCount);
    
    _frames[frameIndex] = source;
}

const Rectangle& Sprite::getFrameSource(unsigned int frameIndex) const
{
    GP_ASSERT(frameIndex < _frameCount);
    
    return _frames[frameIndex];
}

void Sprite::computeFrames(unsigned int frameStride, unsigned int framePadding)
{
    _frameStride = frameStride;
    _framePadding = framePadding;
    
    if (_frameCount < 2)
        return;
    unsigned int imageWidth = _batch->getSampler()->getTexture()->getWidth();
    unsigned int imageHeight = _batch->getSampler()->getTexture()->getHeight();
    float textureWidthRatio = 1.0f / imageWidth;
    float textureHeightRatio = 1.0f / imageHeight;
    
    // If we have a stride then compute the wrap width
    float strideWidth;
    if (_frameStride > 0)
        strideWidth = _frameStride * _frames[0].width;
    else
        strideWidth = imageWidth;
    
    // Mark the start as reference
    float x = _frames[0].x;
    float y = _frames[0].y;
    float width = _frames[0].width;
    float height = _frames[0].height;
    
    // Compute frames 1+
    for (unsigned int frameIndex = 1; frameIndex < _frameCount; frameIndex++)
    {
        _frames[frameIndex].x = x;
        _frames[frameIndex].y = y;
        _frames[frameIndex].width = _width;
        _frames[frameIndex].height = _height;
        
        x += _frames[frameIndex].width + (float)_framePadding;
        if (x >= imageWidth)
        {
            y += _frames[frameIndex].height + (float)_framePadding;
            if (y >= imageHeight)
            {
                y = 0.0f;
            }
            x = 0.0f;
        }
    }
}

unsigned int Sprite::getFrameCount() const
{
    return _frameCount;
}

unsigned int Sprite::getFramePadding() const
{
    return _framePadding;
}

unsigned int Sprite::getFrameStride() const
{
    return _frameStride;
}

void Sprite::setFrameIndex(unsigned int index)
{
    _frameIndex = index;
}

unsigned int Sprite::getFrameIndex() const
{
    return _frameIndex;
}

void Sprite::setOpacity(float opacity)
{
    _opacity = opacity;
}

float Sprite::getOpacity() const
{
    return _opacity;
}

void Sprite::setColor(const Vector4& color)
{
    _color = color;
}

const Vector4& Sprite::getColor() const
{
    return _color;
}
    
Sprite::BlendMode Sprite::getBlendMode() const
{
    return _blendMode;
}
    
void Sprite::setBlendMode(BlendMode mode)
{
    switch (mode)
    {
        case BLEND_NONE:
            _batch->getStateBlock()->setBlend(false);
            break;
        case BLEND_ALPHA:
            _batch->getStateBlock()->setBlend(true);
            _batch->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
            _batch->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
            break;
        case BLEND_ADDITIVE:
            _batch->getStateBlock()->setBlend(true);
            _batch->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
            _batch->getStateBlock()->setBlendDst(RenderState::BLEND_ONE);
            break;
        case BLEND_MULTIPLIED:
            _batch->getStateBlock()->setBlend(true);
            _batch->getStateBlock()->setBlendSrc(RenderState::BLEND_ZERO);
            _batch->getStateBlock()->setBlendDst(RenderState::BLEND_SRC_COLOR);
            break;
        default:
            GP_ERROR("Unsupported blend mode (%d).", mode);
            break;
    }
}

Texture::Sampler* Sprite::getSampler() const
{
    return _batch->getSampler();
}

RenderState::StateBlock* Sprite::getStateBlock() const
{
    return _batch->getStateBlock();
}

Material* Sprite::getMaterial() const
{
    return _batch->getMaterial();
}

unsigned int Sprite::draw()
{
    // Apply scene camera projection and translation offsets
    Vector3 position = Vector3::zero();
    if (_node && _node->getScene())
    {
        Camera* activeCamera = _node->getScene()->getActiveCamera();
        if (activeCamera)
        {
            Node* cameraNode = _node->getScene()->getActiveCamera()->getNode();
            if (cameraNode)
            {
                // Scene projection
                Matrix projectionMatrix;
                projectionMatrix = _node->getProjectionMatrix();
                _batch->setProjectionMatrix(projectionMatrix);
                
                // Camera translation offsets
                position.x -= cameraNode->getTranslationWorld().x;
                position.y -= cameraNode->getTranslationWorld().y;
                
            }
        }
        
        // Apply node translation offsets
        Vector3 translation = _node->getTranslationWorld();
        position.x += translation.x;
        position.y += translation.y;
        position.z += translation.z;
    }
    
    // Apply local offset translation offsets
    if ((_offset & OFFSET_HCENTER) == OFFSET_HCENTER)
        position.x -= _width * 0.5;
    if ((_offset & OFFSET_RIGHT) == OFFSET_RIGHT)
        position.x -= _width;
    if ((_offset & OFFSET_VCENTER) == OFFSET_VCENTER)
        position.y -= _height * 0.5f;
    if ((_offset & OFFSET_TOP) == OFFSET_TOP)
        position.y -= _height;
    if ((_offset & OFFSET_ANCHOR) == OFFSET_ANCHOR)
    {
        position.x -= _width * _anchor.x;
        position.y -= _height * _anchor.y;
    }
    
    // Apply node scale and rotation
    float rotationAngle = 0.0f;
    Vector2 scale = Vector2(_width, _height);
    if (_node)
    {
        // Apply node rotation
        const Quaternion& rot = _node->getRotation();
        if (rot.x != 0.0f || rot.y != 0.0f || rot.z != 0.0f)
            rotationAngle = rot.toAxisAngle(NULL);
        
        // Apply node scale
        if (_node->getScaleX() != 1.0f)
            scale.x *= _node->getScaleX();
        if (_node->getScaleY() != 1.0f)
            scale.y *= _node->getScaleY();
    }
    
    // Apply flip flags
    if ((_flipFlags & FLIP_HORIZONTAL) == FLIP_HORIZONTAL)
    {
        position.x += scale.x;
        scale.x = -scale.x;
    }
    if ((_flipFlags & FLIP_VERTICAL) == FLIP_VERTICAL)
    {
        position.y += scale.y;
        scale.y = -scale.y;
    }
    
    // TODO: Proper batching from cache based on batching rules (image, layers, etc)
    _batch->start();
    _batch->draw(position, _frames[_frameIndex], scale, Vector4(_color.x, _color.y, _color.z, _color.w * _opacity),
                 _anchor, rotationAngle);
    _batch->finish();
    
    return 1;
}

Node* Sprite::getNode() const
{
    return _node;
}
    
void Sprite::setNode(Node* node)
{
    _node = node;
}
    
Sprite* Sprite::clone(NodeCloneContext &context)
{
    Sprite* copy = new Sprite();
    cloneInto(copy, context);
    return copy;
}

void Sprite::cloneInto(Sprite* sprite, NodeCloneContext &context) const
{
    GP_ASSERT(sprite);

    // Clone animations
    AnimationTarget::cloneInto(static_cast<AnimationTarget*>(sprite), context);

    // Get copied node if it exists
    if (Node* node = getNode())
    {
        Node* clonedNode = context.findClonedNode(node);
        if (clonedNode)
        {
            sprite->setNode(clonedNode);
        }
    }

    // Clone properties
    sprite->_width = _width;
    sprite->_height = _height;
    sprite->_offset = _offset;
    sprite->_anchor = _anchor;
    sprite->_flipFlags = _flipFlags;
    sprite->_opacity = _opacity;
    sprite->_color = _color;
    sprite->_blendMode = _blendMode;
    sprite->_frames = new Rectangle[_frameCount];
    memcpy(sprite->_frames, _frames, sizeof(Rectangle) * _frameCount);
    sprite->_frameCount = _frameCount;
    sprite->_frameStride = _frameStride;
    sprite->_framePadding = _framePadding;
    sprite->_frameIndex = _frameIndex;
    sprite->_batch = _batch;
}

unsigned int Sprite::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
        case ANIMATE_OPACITY:
            return 1;
        case ANIMATE_COLOR:
            return 4;
        case ANIMATE_KEYFRAME:
            return 1;
        default:
            return -1;
    }
}

void Sprite::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    GP_ASSERT(value);
    
    switch (propertyId)
    {
        case ANIMATE_OPACITY:
            value->setFloat(0, _opacity);
            break;
        case ANIMATE_COLOR:
            value->setFloat(0, _color.x);
            value->setFloat(1, _color.y);
            value->setFloat(2, _color.z);
            value->setFloat(3, _color.w);
            break;
        case ANIMATE_KEYFRAME:
            value->setFloat(0, (float)_frameIndex);
            break;
        default:
            break;
    }
}

void Sprite::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    GP_ASSERT(value);
    
    switch(propertyId)
    {
        case ANIMATE_OPACITY:
            setOpacity(Curve::lerp(blendWeight, _opacity, value->getFloat(0)));
            break;
        case ANIMATE_COLOR:
            setColor(Vector4(Curve::lerp(blendWeight, _color.x, value->getFloat(0)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(1)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(2)),
                             Curve::lerp(blendWeight, _color.x, value->getFloat(3))));
            break;
        case ANIMATE_KEYFRAME:
            _frameIndex = (unsigned int)value->getFloat(0);
            break;
        default:
            break;
    }
}

}
