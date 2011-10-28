/*
 * SpriteBatch.cpp
 */

#include "Base.h"
#include "SpriteBatch.h"
#include "Game.h"

// Default size of a newly created sprite batch
#define SPRITE_BATCH_DEFAULT_SIZE 128

// Factor to grow a sprite batch by when its size is exceeded
#define SPRITE_BATCH_GROW_FACTOR 2.0f

// Macro to add a sprite vertex
#define ADD_SPRITE_VERTEX(ptr, x, y, z, u, v, c) \
    ptr[0] = x; ptr[1] = y; ptr[2] = z; ptr[3] = u; ptr[4] = v; \
    ptr[5] = c.r; ptr[6] = c.g; ptr[7] = c.b; ptr[8] = c.a

// Default sprite vertex shader
#define SPRITE_VSH \
    "uniform mat4 sb_ortho_projection;" \
    "attribute vec3 a_position;" \
    "attribute vec2 a_texcoord;" \
    "attribute vec4 a_color;" \
    "varying vec2 vtexcoord;" \
    "varying vec4 vcolor;" \
    "void main()" \
    "{" \
        "gl_Position = sb_ortho_projection * vec4(a_position, 1);" \
        "vtexcoord = a_texcoord;" \
        "vcolor = a_color;" \
    "}"

// Default sprite fragment shader
#define SPRITE_FSH \
    "precision mediump float;" \
    "varying vec2 vtexcoord;" \
    "varying vec4 vcolor;" \
    "uniform sampler2D texture;" \
    "void main()" \
    "{" \
        "gl_FragColor = vcolor * texture2D(texture, vtexcoord);" \
    "}"

namespace gameplay
{

// Shared sprite effects
static Effect* __spriteEffect = NULL;

SpriteBatch::SpriteBatch() :
    _texture(NULL), _material(NULL), _vaPosition(-1), _vaTexCoord(-1), _vaColor(-1),
    _textureWidthRatio(0.0f), _textureHeightRatio(0.0f), _capacity(0), _count(0),
    _vertices(NULL), _verticesPtr(NULL), _indices(NULL), _indicesPtr(NULL), _index(0),
    _blend(true), _sfactor(GL_SRC_ALPHA), _dfactor(GL_ONE_MINUS_SRC_ALPHA),
    _drawing(false), _projectionMatrix(NULL)
{
}

SpriteBatch::SpriteBatch(const SpriteBatch& copy)
{
    // hiddden
}

SpriteBatch::~SpriteBatch()
{
    if (_vertices)
    {
        delete[] _vertices;
        _vertices = NULL;
    }

    if (_indices)
    {
        delete[] _indices;
        _indices = NULL;
    }

    if (_projectionMatrix)
    {
        delete _projectionMatrix;
        _projectionMatrix = NULL;
    }

    SAFE_RELEASE(_texture);

    SAFE_RELEASE(_material);
}

SpriteBatch* SpriteBatch::create(Texture* texture, Material* material, unsigned int initialCapacity)
{
    assert(texture != NULL);

    if (material == NULL)
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

            material = Material::createMaterial(__spriteEffect);

            // Release effect since Material now has a reference to it.
            SAFE_RELEASE(__spriteEffect);
        }
        else
        {
            material = Material::createMaterial(__spriteEffect);
        }
    }
    else
    {
        // Add a reference to the material.
        material->addRef();
    }

    Effect* effect = material->getEffect();

    // Look up vertex attributes.
    VertexAttribute vaPosition = effect->getVertexAttribute("a_position");
    VertexAttribute vaTexCoord = effect->getVertexAttribute("a_texcoord");
    VertexAttribute vaColor = effect->getVertexAttribute("a_color");
    if (vaPosition == -1 || vaTexCoord == -1 || vaColor == -1)
    {
        LOG_ERROR("Failed to load vertex attributes for sprite effect.");
        SAFE_RELEASE(material);
        return NULL;
    }

    // Search for the first sampler uniform in the material.
    const char* textureUniformName = NULL;
    unsigned int uniformCount = effect->getUniformCount();
    for (unsigned int i = 0; i < uniformCount; ++i)
    {
        Uniform* uniform = effect->getUniform(i);
        if (uniform && uniform->getType() == GL_SAMPLER_2D)
        {
            textureUniformName = uniform->getName();
            break;
        }
    }
    if (textureUniformName == NULL)
    {
        LOG_ERROR("No uniform of type GL_SAMPLER_2D found in sprite effect.");
        SAFE_RELEASE(material);
        return NULL;
    }

    // Set the texture in the material.
    MaterialParameter* textureParameter = material->getParameter(textureUniformName);
    if (textureParameter != NULL)
    {
        textureParameter->setValue(texture);
    }

    // Create the batch.
    SpriteBatch* batch = new SpriteBatch();
    batch->_material = material;
    batch->_vaPosition = vaPosition;
    batch->_vaTexCoord = vaTexCoord;
    batch->_vaColor = vaColor;
    batch->_textureWidthRatio = 1.0f / (float)texture->getWidth();
    batch->_textureHeightRatio = 1.0f / (float)texture->getHeight();
    batch->resizeBatch(initialCapacity > 0 ? initialCapacity : SPRITE_BATCH_DEFAULT_SIZE);

    // If there is a uniform named 'sb_ortho_projection', allocate and bind our projection matrix to it.
    MaterialParameter* projectionParameter = material->getParameter("sb_ortho_projection");
    if (projectionParameter)
    {
        batch->_projectionMatrix = new Matrix();
        projectionParameter->setValue(batch->_projectionMatrix);
    }

    return batch;
}

void SpriteBatch::begin()
{
    assert(!_drawing);

    // Simply clear our sprite count to start writing to the beginning of the batch.
    _count = 0;
    _index = 0;
    _verticesPtr = _vertices;
    _indicesPtr = _indices;
    _drawing = true;
}

void SpriteBatch::draw(const Rectangle& dst, const Rectangle& src, const Color& color)
{
    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst.x, dst.y, dst.width, dst.height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Color& color)
{
    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst.x, dst.y, dst.z, scale.x, scale.y, u2, v2, u1, v1, color);
}

void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Color& color,
                       const Vector2& rotationPoint, float rotationAngle)
{
    assert(_drawing);

    if (_count >= _capacity)
    {
        growBatch();
    }

    // Calculate uvs.
    float u1 = _textureWidthRatio * src.x;
    float v1 = 1.0f - _textureHeightRatio * src.y;
    float u2 = u1 + _textureWidthRatio * src.width;
    float v2 = v1 - _textureHeightRatio * src.height;

    draw(dst, scale.x, scale.y, u1, v1, u2, v2, color, rotationPoint, rotationAngle);
}

void SpriteBatch::draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
                       const Vector2& rotationPoint, float rotationAngle)
{
    // Expand dst by scale into 4 points.
    float x2 = dst.x + width;
    float y2 = dst.y + height;
    
    Vector2 upLeft(dst.x, dst.y);
    Vector2 upRight(x2, dst.y);
    Vector2 downLeft(dst.x, y2);
    Vector2 downRight(x2, y2);

    // Rotate points around rotationAxis by rotationAngle.
    Vector2 pivotPoint(rotationPoint);
    pivotPoint.x *= width;
    pivotPoint.y *= height;
    pivotPoint.x += dst.x;
    pivotPoint.y += dst.y;
    upLeft.rotate(pivotPoint, rotationAngle);
    upRight.rotate(pivotPoint, rotationAngle);
    downLeft.rotate(pivotPoint, rotationAngle);
    downRight.rotate(pivotPoint, rotationAngle);

    // Write sprite vertex data.
    ADD_SPRITE_VERTEX(_verticesPtr, upLeft.x, upLeft.y, dst.z, u1, v1, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 9), upRight.x, upRight.y, dst.z, u1, v2, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 18), downLeft.x, downLeft.y, dst.z, u2, v1, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 27), downRight.x, downRight.y, dst.z, u2, v2, color);
    _verticesPtr += 36; // 4 vertices per sprite, 9 elements per vertex (4*9)

    // Write sprite index data.
    if (_count > 0)
    {
        // Create a degenerate triangle to connect two triangle strips
        // by duplicating the previous and next vertices.
        _indicesPtr[0] = *(_indicesPtr-1);
        _indicesPtr[1] = _index;
        _indicesPtr += 2;
    }
    _indicesPtr[0] = _index;
    _indicesPtr[1] = _index + 1;
    _indicesPtr[2] = _index + 2;
    _indicesPtr[3] = _index + 3;
    _indicesPtr += 4;
    _index += 4;

    ++_count;
}

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color)
{
    draw(x, y, 0, width, height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color)
{
    assert(_drawing);

    if (_count >= _capacity)
    {
        growBatch();
    }

    // Write sprite vertex data.
    float x2 = x + width;
    float y2 = y + height;
    ADD_SPRITE_VERTEX(_verticesPtr, x, y, z, u1, v1, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 9), x, y2, z, u1, v2, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 18), x2, y, z, u2, v1, color);
    ADD_SPRITE_VERTEX((_verticesPtr + 27), x2, y2, z, u2, v2, color);
    _verticesPtr += 36; // 4 vertices per sprite, 9 elements per vertex (4*9)

    // Write sprite index data.
    if (_count > 0)
    {
        // Create a degenerate triangle to connect two triangle strips
        // by duplicating the previous and next vertices.
        _indicesPtr[0] = *(_indicesPtr-1);
        _indicesPtr[1] = _index;
        _indicesPtr += 2;
    }
    _indicesPtr[0] = _index;
    _indicesPtr[1] = _index + 1;
    _indicesPtr[2] = _index + 2;
    _indicesPtr[3] = _index + 3;
    _indicesPtr += 4;
    _index += 4;

    ++_count;
}

void SpriteBatch::end()
{
    assert(_drawing);

    if (_count > 0)
    {
        // Flush the batch.
        if (_projectionMatrix)
        {
            // Update projection matrix with ortho projection.
            Game* game = Game::getInstance();
            Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, _projectionMatrix);
        }

        // Preserve current blend function.
        GLboolean blend;
        GLint sfactorRGB;
        GLint dfactorRGB;
        GLint sfactorAlpha;
        GLint dfactorAlpha;
        glGetBooleanv(GL_BLEND, &blend);

        if (_blend)
        {
            glGetIntegerv(GL_BLEND_SRC_RGB, &sfactorRGB);
            glGetIntegerv(GL_BLEND_DST_RGB, &dfactorRGB);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &sfactorAlpha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, &dfactorAlpha);

            glEnable(GL_BLEND);
            glBlendFunc(_sfactor, _dfactor);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        // Bind our material.
        _material->bind();

        // Unbind any currently bound VBOs so we can use client arrays.
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0 ) );
        GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );

        GL_ASSERT( glEnableVertexAttribArray(_vaPosition) );
        GL_ASSERT( glVertexAttribPointer(_vaPosition, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid*)_vertices) );

        GL_ASSERT( glEnableVertexAttribArray(_vaTexCoord) );
        GL_ASSERT( glVertexAttribPointer(_vaTexCoord, 2, GL_FLOAT, GL_FALSE, 36, (GLvoid*)(_vertices + 3)) );

        GL_ASSERT( glEnableVertexAttribArray(_vaColor) );
        GL_ASSERT( glVertexAttribPointer(_vaColor, 4, GL_FLOAT, GL_FALSE, 36, (GLvoid*)(_vertices + 5)) );

        GLsizei indexCount = _count * 4 + ((_count - 1) * 2);
        GL_ASSERT( glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, (GLvoid*)_indices) );

        glDisableVertexAttribArray(_vaPosition);
        glDisableVertexAttribArray(_vaTexCoord);
        glDisableVertexAttribArray(_vaColor);

        // Reset blend function.
        if (blend)
        {
            glEnable(GL_BLEND);
            glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    _drawing = false;
}

void SpriteBatch::growBatch()
{
    resizeBatch(_capacity == 0 ? SPRITE_BATCH_DEFAULT_SIZE : (int)((float)_capacity * SPRITE_BATCH_GROW_FACTOR));
}

void SpriteBatch::resizeBatch(unsigned int capacity)
{
    // 4 verts per sprite
    unsigned int newVertexCapacity = capacity * 4;

    // 2 extra indices per sprite for degenerate vertices
    unsigned int newIndexCapacity = capacity * 4 + ((capacity - 1) * 2);

    // 9 elements per vertex (x,y,z,u,v,r,g,b,a)
    float* newVertices = new float[newVertexCapacity * 9];
    unsigned short* newIndices = new unsigned short[newIndexCapacity];

    // Copy and destroy old arrays.
    if (_vertices)
    {
        if (_count > 0)
        {
            unsigned int vertexCount = _count * 4;
            if (vertexCount > newVertexCapacity)
            {
                vertexCount = newVertexCapacity;
            }
            memcpy(newVertices, _vertices, vertexCount * 9 * sizeof(float));
        }
        delete[] _vertices;
    }
    if (_indices)
    {
        if (_count > 0)
        {
            unsigned int indexCount = _count * 4 + ((_count - 1) * 2);
            if (indexCount > newIndexCapacity)
            {
                indexCount = newIndexCapacity;
            }
            memcpy(newIndices, _indices, indexCount * sizeof(unsigned short));
        }
        delete[] _indices;
    }

    // Store new arrays.
    _vertices = newVertices;
    _indices = newIndices;
    _capacity = capacity;
    if (_count > _capacity)
    {
        _count = capacity;
    }

    // Update current pointers.
    if (_count > 0)
    {
        _verticesPtr = _vertices + (_count * 36);
        _indicesPtr = _indices + (_count * 4) + ((_count - 1) * 2);
    }
    else
    {
        _verticesPtr = _vertices;
        _indicesPtr = _indices;
    }
}

void SpriteBatch::setBlendEnabled(bool blend)
{
    _blend = blend;
}

bool SpriteBatch::getBlendEnabled() const
{
    return _blend;
}

void SpriteBatch::setBlendMode(const GLenum sfactor, const GLenum dfactor)
{
    _sfactor = sfactor;
    _dfactor = dfactor;
}

void SpriteBatch::getBlendMode(GLenum* sfactor, GLenum* dfactor)
{
    assert(sfactor);
    assert(dfactor);

    *sfactor = _sfactor;
    *dfactor = _dfactor;
}

Material* SpriteBatch::getMaterial()
{
    return _material;
}

}
