#include "Base.h"
#include "SpriteBatch.h"
#include "Game.h"

// Default size of a newly created sprite batch
#define SPRITE_BATCH_DEFAULT_SIZE 128

// Factor to grow a sprite batch by when its size is exceeded
#define SPRITE_BATCH_GROW_FACTOR 2.0f

// Macro for adding a sprite to the batch
#define ADD_SPRITE_VERTEX(vtx, vx, vy, vz, vu, vv, vr, vg, vb, va) \
    vtx.x = vx; vtx.y = vy; vtx.z = vz; \
    vtx.u = vu; vtx.v = vv; \
    vtx.r = vr; vtx.g = vg; vtx.b = vb; vtx.a = va

// Default sprite vertex shader
#define SPRITE_VSH \
    "uniform mat4 u_projectionMatrix;\n" \
    "attribute vec3 a_position;\n" \
    "attribute vec2 a_texCoord;\n" \
    "attribute vec4 a_color;\n" \
    "varying vec2 v_texCoord;\n" \
    "varying vec4 v_color;\n" \
    "void main()\n" \
    "{\n" \
        "gl_Position = u_projectionMatrix * vec4(a_position, 1);\n" \
        "v_texCoord = a_texCoord;\n" \
        "v_color = a_color;\n" \
    "}\n"

// Default sprite fragment shader
#define SPRITE_FSH \
    "#ifdef OPENGL_ES\n" \
    "precision highp float;\n" \
    "#endif\n" \
    "varying vec2 v_texCoord;\n" \
    "varying vec4 v_color;\n" \
    "uniform sampler2D u_texture;\n" \
    "void main()\n" \
    "{\n" \
        "gl_FragColor = v_color * texture2D(u_texture, v_texCoord);\n" \
    "}\n"

namespace gameplay
{

/**
 * Sprite vertex structure used for batching.
 */
struct SpriteVertex
{
    /**
     * The x coordinate of the vertex.
     */
    float x;
    
    /**
     * The y coordinate of the vertex.
     */
    float y;
    
    /**
     * The z coordinate of the vertex.
     */
    float z;

    /**
     * The u component of the (u, v) texture coordinates for the vertex.
     */
    float u;
    
    /**
     * The v component of the (u, v) texture coordinates for the vertex.
     */
    float v;

    /**
     * The red color component of the vertex.
     */
    float r;
    
    /**
     * The green color component of the vertex.
     */
    float g;
    
    /**
     * The blue color component of the vertex.
     */
    float b;
    
    /**
     * The alpha component of the vertex.
     */
    float a;
};

// Shared sprite effects
static Effect* __spriteEffect = NULL;

SpriteBatch::SpriteBatch()
    : _batch(NULL), _textureWidthRatio(0.0f), _textureHeightRatio(0.0f)
{
}

SpriteBatch::SpriteBatch(const SpriteBatch& copy)
{
    // hiddden
}

SpriteBatch::~SpriteBatch()
{
    SAFE_DELETE(_batch);
    if (!_customEffect)
    {
        if (__spriteEffect->getRefCount() == 1)
        {
            __spriteEffect->release();
            __spriteEffect = NULL;
        }
        else
            __spriteEffect->release();
    }
}

SpriteBatch* SpriteBatch::create(const char* texturePath, Effect* effect, unsigned int initialCapacity)
{
    Texture* texture = Texture::create(texturePath);
    SpriteBatch* batch = SpriteBatch::create(texture);
    SAFE_RELEASE(texture);
    return batch;
}

SpriteBatch* SpriteBatch::create(Texture* texture, Effect* effect, unsigned int initialCapacity)
{
    assert(texture != NULL);

    bool customEffect = (effect != NULL);
    if (!customEffect)
    {
        // Create our static sprite effect.
        if (__spriteEffect == NULL)
        {
            __spriteEffect = Effect::createFromSource(SPRITE_VSH, SPRITE_FSH);
            if (__spriteEffect == NULL)
            {
                LOG_ERROR("Unable to load sprite effect.");
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
        LOG_ERROR("No uniform of type GL_SAMPLER_2D found in sprite effect.");
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
    SAFE_RELEASE(sampler);

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
    batch->_customEffect = customEffect;
    batch->_batch = meshBatch;
    batch->_textureWidthRatio = 1.0f / (float)texture->getWidth();
    batch->_textureHeightRatio = 1.0f / (float)texture->getHeight();

    // Bind an ortho projection to the material by default (user can override with setProjectionMatrix)
    material->getParameter("u_projectionMatrix")->bindValue(batch, &SpriteBatch::getOrthoMatrix);

    return batch;
}

void SpriteBatch::begin()
{
    _batch->begin();
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

    draw(dst.x, dst.y, dst.z, scale.x, scale.y, u2, v2, u1, v1, color);
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
    float x = dst.x;
    float y = dst.y;

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
    ADD_SPRITE_VERTEX(v[0], upLeft.x, upLeft.y, dst.z, u1, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[1], upRight.x, upRight.y, dst.z, u1, v2, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[2], downLeft.x, downLeft.y, dst.z, u2, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[3], downRight.x, downRight.y, dst.z, u2, v2, color.x, color.y, color.z, color.w);
    
    static unsigned short indices[4] = { 0, 1, 2, 3 };

    _batch->add(v, 4, indices, 4);
}

void SpriteBatch::draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height,
    float u1, float v1, float u2, float v2, const Vector4& color, const Vector2& rotationPoint, float rotationAngle)
{
    // Calculate the vertex positions.
    Vector3 p[4];
    p[0] = position - 0.5f * width * right - 0.5f * height * forward;
    p[1] = position + 0.5f * width * right - 0.5f * height * forward;
    p[2] = p[0] + height * forward;
    p[3] = p[1] + height * forward;

    // Calculate the rotation point.
    Vector3 rp = p[0] + (rotationPoint.x * width * right) + (rotationPoint.y * height * forward);

    // Rotate all points the specified amount about the given point (about the up vector).
    Vector3 u;
    Vector3::cross(right, forward, &u);
    Matrix rotation;
    Matrix::createRotation(u, rotationAngle, &rotation);
    p[0] = (rotation * (p[0] - rp)) + rp;
    p[1] = (rotation * (p[1] - rp)) + rp;
    p[2] = (rotation * (p[2] - rp)) + rp;
    p[3] = (rotation * (p[3] - rp)) + rp;

    // Add the sprite vertex data to the batch.
    static SpriteVertex v[4];
    ADD_SPRITE_VERTEX(v[0], p[0].x, p[0].y, p[0].z, u1, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[1], p[1].x, p[1].y, p[1].z, u2, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[2], p[2].x, p[2].y, p[2].z, u1, v2, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[3], p[3].x, p[3].y, p[3].z, u2, v2, color.x, color.y, color.z, color.w);
    
    static const unsigned short indices[4] = { 0, 1, 2, 3 };
    _batch->add(v, 4, const_cast<unsigned short*>(indices), 4);
}

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color)
{
    draw(x, y, 0, width, height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip)
{
    // Need to clip the rectangle given by { x, y, width, height } into clip by potentially:
    //  - Moving x to the right.
    //  - Moving y down.
    //  - Moving width to the left.
    //  - Moving height up.
    //  - A combination of the above.
    //  - Not drawing at all.
    //
    // We need to scale the uvs accordingly as we do this.

    // First check to see if we need to draw at all.
    if (x + width < clip.x || x > clip.x + clip.width ||
        y + height < clip.y || y > clip.y + clip.height)
    {
        return;
    }

    const float uvWidth = u2 - u1;
    const float uvHeight = v2 - v1;

    // Moving x to the right.
    if (x < clip.x)
    {
        const float percent = (clip.x - x) / width;
        x = clip.x;
        u1 += uvWidth * percent;
    }

    // Moving y down.
    if (y < clip.y)
    {
        const float percent = (clip.y - y) / height;
        y = clip.y;
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

    // Now we can perform a normal draw call.
    draw(x, y, 0, width, height, u1, v1, u2, v2, color);
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
    ADD_SPRITE_VERTEX(v[0], x, y, z, u1, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[1], x, y2, z, u1, v2, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[2], x2, y, z, u2, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX(v[3], x2, y2, z, u2, v2, color.x, color.y, color.z, color.w);

    static unsigned short indices[4] = { 0, 1, 2, 3 };

    _batch->add(v, 4, indices, 4);
}

void SpriteBatch::end()
{
    // Finish and draw the batch
    _batch->end();
    _batch->draw();
}

RenderState::StateBlock* SpriteBatch::getStateBlock() const
{
    return _batch->getMaterial()->getStateBlock();
}

Material* SpriteBatch::getMaterial()
{
    return _batch->getMaterial();
}

void SpriteBatch::setProjectionMatrix(const Matrix& matrix)
{
    // Bind the specified matrix to a parameter named 'u_projectionMatrix' (assumed to exist).
    _batch->getMaterial()->getParameter("u_projectionMatrix")->setValue(matrix);
}

const Matrix& SpriteBatch::getOrthoMatrix() const
{
    // Update matrix with ortho projection and return it.
    Game* game = Game::getInstance();
    Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, &_projectionMatrix);
    return _projectionMatrix;
}

}
