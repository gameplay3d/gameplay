#include "Base.h"
#include "SpriteBatch.h"
#include "Game.h"
#include "Material.h"

// Default size of a newly created sprite batch
#define SPRITE_BATCH_DEFAULT_SIZE 128

// Factor to grow a sprite batch by when its size is exceeded
#define SPRITE_BATCH_GROW_FACTOR 2.0f

// Macro for adding a sprite to the batch
#define SPRITE_ADD_VERTEX(vtx, vx, vy, vz, vu, vv, vr, vg, vb, va) \
    vtx.x = vx; vtx.y = vy; vtx.z = vz; \
    vtx.u = vu; vtx.v = vv; \
    vtx.r = vr; vtx.g = vg; vtx.b = vb; vtx.a = va

// Default sprite shaders
#define SPRITE_VSH "res/shaders/sprite.vert"
#define SPRITE_FSH "res/shaders/sprite.frag"

namespace gameplay
{

static Effect* __spriteEffect = NULL;

SpriteBatch::SpriteBatch()
    : _batch(NULL), _sampler(NULL), _textureWidthRatio(0.0f), _textureHeightRatio(0.0f)
{
}

SpriteBatch::~SpriteBatch()
{
    SAFE_DELETE(_batch);
    SAFE_RELEASE(_sampler);
    if (!_customEffect)
    {
        if (__spriteEffect && __spriteEffect->getRefCount() == 1)
        {
            __spriteEffect->release();
            __spriteEffect = NULL;
        }
        else
        {
            __spriteEffect->release();
        }
    }
}

SpriteBatch* SpriteBatch::create(const char* texturePath, Effect* effect, unsigned int initialCapacity)
{
    Texture* texture = Texture::create(texturePath);
    SpriteBatch* batch = SpriteBatch::create(texture, effect, initialCapacity);
    SAFE_RELEASE(texture);
    return batch;
}

SpriteBatch* SpriteBatch::create(Texture* texture,  Effect* effect, unsigned int initialCapacity)
{
    GP_ASSERT(texture != NULL);

    bool customEffect = (effect != NULL);
    if (!customEffect)
    {
        // Create our static sprite effect.
        if (__spriteEffect == NULL)
        {
            __spriteEffect = Effect::createFromFile(SPRITE_VSH, SPRITE_FSH);
            if (__spriteEffect == NULL)
            {
                GP_ERROR("Unable to load sprite effect.");
                return NULL;
            }
            effect = __spriteEffect;
        }
        else
        {
            effect = __spriteEffect;
            __spriteEffect->addRef();
        }
    }

    // Search for the first sampler uniform in the effect.
    Uniform* samplerUniform = NULL;
    for (unsigned int i = 0, count = effect->getUniformCount(); i < count; ++i)
    {
        Uniform* uniform = effect->getUniform(i);
        if (uniform && uniform->getType() == GL_SAMPLER_2D)
        {
            samplerUniform = uniform;
            break;
        }
    }
    if (!samplerUniform)
    {
        GP_ERROR("No uniform of type GL_SAMPLER_2D found in sprite effect.");
        SAFE_RELEASE(effect);
        return NULL;
    }

    // Wrap the effect in a material
    Material* material = Material::create(effect); // +ref effect

    // Set initial material state
    material->getStateBlock()->setBlend(true);
    material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    // Bind the texture to the material as a sampler
    Texture::Sampler* sampler = Texture::Sampler::create(texture); // +ref texture
    material->getParameter(samplerUniform->getName())->setValue(sampler);
    
    // Define the vertex format for the batch
    VertexFormat::Element vertexElements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2),
        VertexFormat::Element(VertexFormat::COLOR, 4)
    };
    VertexFormat vertexFormat(vertexElements, 3);

    // Create the mesh batch
    MeshBatch* meshBatch = MeshBatch::create(vertexFormat, Mesh::TRIANGLE_STRIP, material, true, initialCapacity > 0 ? initialCapacity : SPRITE_BATCH_DEFAULT_SIZE);
    material->release(); // don't call SAFE_RELEASE since material is used below

    // Create the batch
    SpriteBatch* batch = new SpriteBatch();
    batch->_sampler = sampler;
    batch->_customEffect = customEffect;
    batch->_batch = meshBatch;
    batch->_textureWidthRatio = 1.0f / (float)texture->getWidth();
    batch->_textureHeightRatio = 1.0f / (float)texture->getHeight();

	// Bind an ortho projection to the material by default (user can override with setProjectionMatrix)
	Game* game = Game::getInstance();
	Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, &batch->_projectionMatrix);
	material->getParameter("u_projectionMatrix")->bindValue(batch, &SpriteBatch::getProjectionMatrix);
	
    return batch;
}

void SpriteBatch::start()
{
    _batch->start();
}

void SpriteBatch::draw(const Rectangle& dst, const Rectangle& src, const Vector4& color)
{
    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst.x, dst.y, dst.width, dst.height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color)
{
    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst.x, dst.y, dst.z, scale.x, scale.y, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color,
                       const Vector2& rotationPoint, float rotationAngle)
{
    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst, scale.x, scale.y, u1, v1, u2, v2, color, rotationPoint, rotationAngle);
}

void SpriteBatch::draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
                       const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter)
{
    draw(dst.x, dst.y, dst.z, width, height, u1, v1, u2, v2, color, rotationPoint, rotationAngle, positionIsCenter);
}

void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
          const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter)
{
    // Treat the given position as the center if the user specified it as such.
    if (positionIsCenter)
    {
        x -= 0.5f * width;
        y -= 0.5f * height;
    }

    // Expand the destination position by scale into 4 points.
    float x2 = x + width;
    float y2 = y + height;
    
    Vector2 upLeft(x, y);
    Vector2 upRight(x2, y);
    Vector2 downLeft(x, y2);
    Vector2 downRight(x2, y2);

    // Rotate points around rotationAxis by rotationAngle.
    Vector2 pivotPoint(rotationPoint);
    pivotPoint.x *= width;
    pivotPoint.y *= height;
    pivotPoint.x += x;
    pivotPoint.y += y;
    upLeft.rotate(pivotPoint, rotationAngle);
    upRight.rotate(pivotPoint, rotationAngle);
    downLeft.rotate(pivotPoint, rotationAngle);
    downRight.rotate(pivotPoint, rotationAngle);

    // Write sprite vertex data.
    static SpriteVertex v[4];
    SPRITE_ADD_VERTEX(v[0], downLeft.x, downLeft.y, z, u1, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[1], upLeft.x, upLeft.y, z, u1, v2, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[2], downRight.x, downRight.y, z, u2, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[3], upRight.x, upRight.y, z, u2, v2, color.x, color.y, color.z, color.w);
    
    static unsigned short indices[4] = { 0, 1, 2, 3 };

    _batch->add(v, 4, indices, 4);
}

void SpriteBatch::draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height,
    float u1, float v1, float u2, float v2, const Vector4& color, const Vector2& rotationPoint, float rotationAngle)
{
    // Calculate the vertex positions.
    Vector3 tRight(right);
    tRight *= width * 0.5f;
    Vector3 tForward(forward);
    tForward *= height * 0.5f;
    
    Vector3 p0 = position;
    p0 -= tRight;
    p0 -= tForward;

    Vector3 p1 = position;
    p1 += tRight;
    p1 -= tForward;

    tForward = forward;
    tForward *= height;
    Vector3 p2 = p0;
    p2 += tForward;
    Vector3 p3 = p1;
    p3 += tForward;

    // Calculate the rotation point.
    Vector3 rp = p0;
    tRight = right;
    tRight *= width * rotationPoint.x;
    tForward *= rotationPoint.y;
    rp += tRight;
    rp += tForward;

    // Rotate all points the specified amount about the given point (about the up vector).
    static Vector3 u;
    Vector3::cross(right, forward, &u);
    static Matrix rotation;
    Matrix::createRotation(u, rotationAngle, &rotation);

    p0 -= rp;
    p0 *= rotation;
    p0 += rp;

    p1 -= rp;
    p1 *= rotation;
    p1 += rp;

    p2 -= rp;
    p2 *= rotation;
    p2 += rp;

    p3 -= rp;
    p3 *= rotation;
    p3 += rp;

    // Add the sprite vertex data to the batch.
    static SpriteVertex v[4];
    SPRITE_ADD_VERTEX(v[0], p0.x, p0.y, p0.z, u1, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[1], p1.x, p1.y, p1.z, u2, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[2], p2.x, p2.y, p2.z, u1, v2, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[3], p3.x, p3.y, p3.z, u2, v2, color.x, color.y, color.z, color.w);
    
    static const unsigned short indices[4] = { 0, 1, 2, 3 };
    _batch->add(v, 4, const_cast<unsigned short*>(indices), 4);
}

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color)
{
    draw(x, y, 0, width, height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip)
{
    // Only draw if at least part of the sprite is within the clip region.
    if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
        draw(x, y, 0, width, height, u1, v1, u2, v2, color);
}

void SpriteBatch::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, SpriteBatch::SpriteVertex* vertices)
{
    GP_ASSERT(vertices);

    const float x2 = x + width;
    const float y2 = y + height;
    SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.x, color.y, color.z, color.w);
}

void SpriteBatch::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip, SpriteBatch::SpriteVertex* vertices)
{
    GP_ASSERT(vertices);

    // Only add a sprite if at least part of the sprite is within the clip region.
    if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
    {
        const float x2 = x + width;
        const float y2 = y + height;
        SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.x, color.y, color.z, color.w);
        SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.x, color.y, color.z, color.w);
        SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.x, color.y, color.z, color.w);
        SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.x, color.y, color.z, color.w);
    }
}

void SpriteBatch::draw(SpriteBatch::SpriteVertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount)
{
    GP_ASSERT(vertices);
    GP_ASSERT(indices);

    _batch->add(vertices, vertexCount, indices, indexCount);
}

void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, bool positionIsCenter)
{
    // Treat the given position as the center if the user specified it as such.
    if (positionIsCenter)
    {
        x -= 0.5f * width;
        y -= 0.5f * height;
    }

    // Write sprite vertex data.
    const float x2 = x + width;
    const float y2 = y + height;
    static SpriteVertex v[4];
    SPRITE_ADD_VERTEX(v[0], x, y, z, u1, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[1], x, y2, z, u1, v2, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[2], x2, y, z, u2, v1, color.x, color.y, color.z, color.w);
    SPRITE_ADD_VERTEX(v[3], x2, y2, z, u2, v2, color.x, color.y, color.z, color.w);

    static unsigned short indices[4] = { 0, 1, 2, 3 };

    _batch->add(v, 4, indices, 4);
}

void SpriteBatch::finish()
{
    // Finish and draw the batch
    _batch->finish();
    _batch->draw();
}

RenderState::StateBlock* SpriteBatch::getStateBlock() const
{
    return _batch->getMaterial()->getStateBlock();
}

Texture::Sampler* SpriteBatch::getSampler() const
{
    return _sampler;
}

Material* SpriteBatch::getMaterial() const
{
    return _batch->getMaterial();
}

void SpriteBatch::setProjectionMatrix(const Matrix& matrix)
{
    _projectionMatrix = matrix;
}

const Matrix& SpriteBatch::getProjectionMatrix() const
{
    return _projectionMatrix;
}

bool SpriteBatch::clipSprite(const Rectangle& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2)
{
    // Clip the rectangle given by { x, y, width, height } into clip.
    // We need to scale the uvs accordingly as we do this.

    // First check to see if we need to draw at all.
    if (x + width < clip.x || x > clip.x + clip.width ||
        y + height < clip.y || y > clip.y + clip.height)
    {
        return false;
    }

    const float uvWidth = u2 - u1;
    const float uvHeight = v2 - v1;

    // Moving x to the right.
    if (x < clip.x)
    {
        const float percent = (clip.x - x) / width;
        const float dx = clip.x - x;
        x = clip.x;
        width -= dx;
        u1 += uvWidth * percent;
    }

    // Moving y down.
    if (y < clip.y)
    {
        const float percent = (clip.y - y) / height;
        const float dy = clip.y - y;
        y = clip.y;
        height -= dy;
        v1 += uvHeight * percent;
    }

    // Moving width to the left.
    const float clipX2 = clip.x + clip.width;
    float x2 = x + width;
    if (x2 > clipX2)
    {
        const float percent = (x2 - clipX2) / width;
        width = clipX2 - x;
        u2 -= uvWidth * percent;
    }

    // Moving height up.
    const float clipY2 = clip.y + clip.height;
    float y2 = y + height;
    if (y2 > clipY2)
    {
        const float percent = (y2 - clipY2) / height;
        height = clipY2 - y;
        v2 -= uvHeight * percent;
    }

    return true;
}

}
