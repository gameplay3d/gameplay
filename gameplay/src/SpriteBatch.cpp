#include "Base.h"
#include "SpriteBatch.h"
#include "Game.h"

// Default size of a newly created sprite batch
#define SPRITE_BATCH_DEFAULT_SIZE 128

// Factor to grow a sprite batch by when its size is exceeded
#define SPRITE_BATCH_GROW_FACTOR 2.0f

// Macro to add a sprite vertex
#define ADD_SPRITE_VERTEX(ptr, x, y, z, u, v, r, g, b, a) \
    ptr[0] = x; ptr[1] = y; ptr[2] = z; ptr[3] = u; ptr[4] = v; \
    ptr[5] = r; ptr[6] = g; ptr[7] = b; ptr[8] = a

// Default sprite vertex shader
#define SPRITE_VSH \
    "uniform mat4 u_projectionMatrix;\n" \
    "attribute vec3 a_position;\n" \
    "attribute vec2 a_texcoord;\n" \
    "attribute vec4 a_color;\n" \
    "varying vec2 v_texcoord;\n" \
    "varying vec4 v_color;\n" \
    "void main()\n" \
    "{\n" \
        "gl_Position = u_projectionMatrix * vec4(a_position, 1);\n" \
        "v_texcoord = a_texcoord;\n" \
        "v_color = a_color;\n" \
    "}\n"

// Default sprite fragment shader
#define SPRITE_FSH \
    "#ifdef OPENGL_ES\n" \
    "precision highp float;\n" \
    "#endif\n" \
    "varying vec2 v_texcoord;\n" \
    "varying vec4 v_color;\n" \
    "uniform sampler2D u_texture;\n" \
    "void main()\n" \
    "{\n" \
        "gl_FragColor = v_color * texture2D(u_texture, v_texcoord);\n" \
    "}\n"

namespace gameplay
{

// Shared sprite effects
static Effect* __spriteEffect = NULL;

SpriteBatch::SpriteBatch() :
    _texture(NULL), _effect(NULL), _stateBlock(NULL), _sampler(NULL), _samplerUniform(NULL), _projectionUniform(NULL), _vaPosition(-1), _vaTexCoord(-1), _vaColor(-1),
    _textureWidthRatio(0.0f), _textureHeightRatio(0.0f), _capacity(0), _count(0),
    _vertices(NULL), _verticesPtr(NULL), _indices(NULL), _indicesPtr(NULL), _index(0),
    _drawing(false), _projectionMatrix(NULL), _customProjectionMatrix(false)
{
    _stateBlock = RenderState::StateBlock::create();
    _stateBlock->setBlend(true);
    _stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    _stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
}

SpriteBatch::SpriteBatch(const SpriteBatch& copy)
{
    // hiddden
}

SpriteBatch::~SpriteBatch()
{
    SAFE_RELEASE(_stateBlock);
    SAFE_DELETE_ARRAY(_vertices);
    SAFE_DELETE_ARRAY(_indices);
    SAFE_DELETE(_projectionMatrix);
    SAFE_RELEASE(_sampler);
    SAFE_RELEASE(_effect);
    SAFE_RELEASE(_texture);
}

SpriteBatch* SpriteBatch::create(const char* texturePath, Effect* effect, unsigned int initialCapacity)
{
    Texture* texture = Texture::create(texturePath);
    SpriteBatch* batch = SpriteBatch::create(texture);
    batch->_texture = texture;
    return batch;
}

SpriteBatch* SpriteBatch::create(Texture* texture, Effect* effect, unsigned int initialCapacity)
{
    assert(texture != NULL);

    if (effect == NULL)
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
            effect->addRef();
        }
    }
    else
    {
        // Add a reference to the effect.
        effect->addRef();
    }

    // Look up vertex attributes.
    VertexAttribute vaPosition = effect->getVertexAttribute("a_position");
    VertexAttribute vaTexCoord = effect->getVertexAttribute("a_texcoord");
    VertexAttribute vaColor = effect->getVertexAttribute("a_color");
    if (vaPosition == -1 || vaTexCoord == -1 || vaColor == -1)
    {
        LOG_ERROR("Failed to load vertex attributes for sprite effect.");
        SAFE_RELEASE(effect);
        return NULL;
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

    // Create the batch.
    SpriteBatch* batch = new SpriteBatch();
    batch->_effect = effect;
    batch->_sampler = Texture::Sampler::create(texture);
    batch->_samplerUniform = samplerUniform;
    batch->_vaPosition = vaPosition;
    batch->_vaTexCoord = vaTexCoord;
    batch->_vaColor = vaColor;
    batch->_textureWidthRatio = 1.0f / (float)texture->getWidth();
    batch->_textureHeightRatio = 1.0f / (float)texture->getHeight();
    batch->resizeBatch(initialCapacity > 0 ? initialCapacity : SPRITE_BATCH_DEFAULT_SIZE);

    // If there is a uniform named 'u_projectionMatrix', store it so that we can set our projection matrix to it
    batch->_projectionUniform = effect->getUniform("u_projectionMatrix");
    if (batch->_projectionUniform)
    {
        batch->_projectionMatrix = new Matrix();
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

void SpriteBatch::draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
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
    ADD_SPRITE_VERTEX(_verticesPtr, upLeft.x, upLeft.y, dst.z, u1, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 9), upRight.x, upRight.y, dst.z, u1, v2, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 18), downLeft.x, downLeft.y, dst.z, u2, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 27), downRight.x, downRight.y, dst.z, u2, v2, color.x, color.y, color.z, color.w);
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

void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color)
{
    draw(x, y, 0, width, height, u1, v1, u2, v2, color);
}

void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color)
{
    assert(_drawing);

    if (_count >= _capacity)
    {
        growBatch();
    }

    // Write sprite vertex data.
    float x2 = x + width;
    float y2 = y + height;
    ADD_SPRITE_VERTEX(_verticesPtr, x, y, z, u1, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 9), x, y2, z, u1, v2, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 18), x2, y, z, u2, v1, color.x, color.y, color.z, color.w);
    ADD_SPRITE_VERTEX((_verticesPtr + 27), x2, y2, z, u2, v2, color.x, color.y, color.z, color.w);
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
        if (_projectionMatrix && !_customProjectionMatrix)
        {
            // Update projection matrix with ortho projection.
            Game* game = Game::getInstance();
            Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, _projectionMatrix);
        }

        // Apply render state
        _stateBlock->bind();

        // Bind our effect and any required parameters
        _effect->bind();
        if (_samplerUniform && _sampler)
        {
            _effect->setValue(_samplerUniform, _sampler);
        }
        if (_projectionMatrix)
        {
            _effect->setValue(_projectionUniform, _projectionMatrix);
        }

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
    }

    _drawing = false;
}

RenderState::StateBlock* SpriteBatch::getStateBlock() const
{
    return _stateBlock;
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
        SAFE_DELETE_ARRAY(_vertices);
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
        SAFE_DELETE_ARRAY(_indices);
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

Effect* SpriteBatch::getEffect()
{
    return _effect;
}

void SpriteBatch::setProjectionMatrix(const Matrix& matrix)
{
    if (_projectionMatrix)
    {
        _projectionMatrix->set(matrix);
        _customProjectionMatrix = true;
    }
}

}
