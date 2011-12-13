#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include "Texture.h"
#include "Effect.h"
#include "Mesh.h"
#include "Rectangle.h"
#include "Matrix.h"
#include "RenderState.h"

namespace gameplay
{

/**
 * Enables groups of sprites to be drawn with common settings.
 *
 * This class provides efficient rendering and sorting of two-dimensional
 * sprites. Only a single texture and effect can be used with a SpriteBatch.
 * This limitation promotes efficient batching by using texture atlases and
 * implicit sorting to minimize state changes. Therefore, it is highly
 * recommended to combine multiple small textures into larger texture atlases
 * where possible when drawing sprites.
 */
class SpriteBatch
{
    friend class Package;

public:

    /**
     * Creates a new SpriteBatch for drawing sprites with the given texture.
     *
     * If the effect parameter is NULL, a default effect is used which
     * applies an orthographic projection for the currently bound viewport.
     * A custom projection matrix can be used with the default effect by passing
     * a new projection matrix into the SpriteBatch via the setProjectionMatrix
     * method.
     *
     * If a custom effect is specified, it must meet the following requirements:
     * <ol>
     * <li>The vertex shader inputs must include a vec3 position, a vec2 tex coord
     * and a vec4 color.
     * <li>The names of the the vertex shader inputs must match the names defined
     * by the VERTEX_ATTRIBUTE_XXX constants.
     * <li>The fragment shader must define at least a single sampler/texture uniform.
     * </ol>
     *
     * @param texturePath The path of the texture for this sprite batch.
     * @param effect An optional effect to use with the SpriteBatch.
     * @param initialCapacity An optional initial capacity of the batch (number of sprites).
     * 
     * @return A new SpriteBatch for drawing sprites using the given texture.
     */
    static SpriteBatch* create(const char* texturePath, Effect* effect = NULL, unsigned int initialCapacity = 0);

    /**
     * Creates a new SpriteBatch for drawing sprites with the given texture.
     *
     * If the effect parameter is NULL, a default effect is used which
     * applies an orthographic projection for the currently bound viewport.
     * A custom projection matrix can be used with the default effect by passing
     * a new projection matrix into the SpriteBatch via the setProjectionMatrix
     * method.
     *
     * If a custom effect is specified, it must meet the following requirements:
     * <ol>
     * <li>The vertex shader inputs must include a vec3 position, a vec2 tex coord
     * and a vec4 color.
     * <li>The names of the the vertex shader inputs must match the names defined
     * by the VERTEX_ATTRIBUTE_XXX constants.
     * <li>The fragment shader must define at least a single sampler/texture uniform.
     * </ol>
     *
     * @param texture The texture for this sprite batch.
     * @param effect An optional effect to use with the SpriteBatch.
     * @param initialCapacity An optional initial capacity of the batch (number of sprites).
     * 
     * @return A new SpriteBatch for drawing sprites using the given texture.
     */
    static SpriteBatch* create(Texture* texture, Effect* effect = NULL, unsigned int initialCapacity = 0);

    /**
     * Destructor.
     */
    virtual ~SpriteBatch();

    /**
     * Begins drawing sprites.
     *
     * This method must be called before drawing any sprites and it must eventually be
     * followed by a call to end().
     */
    void begin();

    /**
     * Draws a single sprite.
     * 
     * @param dst The destination rectangle.
     * @param src The source rectangle.
     * @param color The color to tint the sprite. Use white for no tint.
     */
    void draw(const Rectangle& dst, const Rectangle& src, const Vector4& color = Vector4::one());

    /**
     * Draws a single sprite.
     * 
     * @param dst The destination position.
     * @param src The source rectangle.
     * @param scale The X and Y scale.
     * @param color The color to tint the sprite. Use white for no tint.
     */
    void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color = Vector4::one());

    /**
     * Draws a single sprite, rotated around rotationPoint by rotationAngle.
     *
     * @param dst The destination position.
     * @param src The source rectangle.
     * @param scale The X and Y scale.
     * @param color The color to tint the sprite. Use white for no tint.
     * @param rotationPoint The point to rotate around, relative to dst's x and y values.
     *                      (e.g. Use Vector2(0.5f, 0.5f) to rotate around the quad's center.)
     * @param rotationAngle The rotation angle.
     */
    void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color,
              const Vector2& rotationPoint, float rotationAngle);

    /**
     * Draws a single sprite, rotated around rotationPoint by rotationAngle.
     * 
     * @param dst The destination position.
     * @param width The source width.
     * @param height The source height.
     * @param u1 Texture coordinate.
     * @param v1 Texture coordinate.
     * @param u2 Texture coordinate.
     * @param v2 Texture coordinate.
     * @param color The color to tint the sprite. Use white for no tint.
     * @param rotationPoint The point to rotate around, relative to dst's x and y values.
     *                      (e.g. Use Vector2(0.5f, 0.5f) to rotate around the quad's center.)
     * @param rotationAngle The rotation angle.
     */
    void draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
              const Vector2& rotationPoint, float rotationAngle);

    /**
     * Draws a single sprite.
     * 
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param width The sprite width.
     * @param height The sprite height
     * @param u1 Texture coordinate.
     * @param v1 Texture coordinate.
     * @param u2 Texture coordinate.
     * @param v2 Texture coordinate.
     * @param color The color to tint the sprite. Use white for no tint.
     */
    void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color);

    /**
     * Draws a single sprite.
     * 
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     * @param width The sprite width.
     * @param height The sprite height
     * @param u1 Texture coordinate.
     * @param v1 Texture coordinate.
     * @param u2 Texture coordinate.
     * @param v2 Texture coordinate.
     * @param color The color to tint the sprite. Use white for no tint.
     */
    void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color);

    /**
     * Ends sprite drawing.
     *
     * This method flushes the batch and commits rendering of all sprites that were
     * drawn since the last call to begin().
     */
    void end();

    /**
     * Returns the StateBlock for the SpriteBatch.
     *
     * The returned state block controls the renderer state used when drawing items
     * with this sprite batch. Modification can be made to the returned state block
     * to change how primitives are rendered.
     *
     * @return The StateBlock for this SpriteBatch.
     */
    RenderState::StateBlock* getStateBlock() const;

    /**
     * Gets the effect used by this batch.
     * 
     * @return The effect.
     */
    Effect* getEffect();

    /**
     * Sets a custom projection matrix to use with the sprite batch.
     *
     * When the default effect is used with a SpriteBatch (i.e. when
     * NULL is passed into the 'effect' parameter of SpriteBatch::create),
     * this method sets a custom projection matrix to be used instead
     * of the default orthographic projection.
     *
     * @param matrix The new projection matrix to be used with the default effect.
     */
    void setProjectionMatrix(const Matrix& matrix);

private:

    /**
     * Constructor.
     */
    SpriteBatch();

    /**
     * Copy constructor.
     * 
     * @param copy The SpriteBatch to copy.
     */
    SpriteBatch(const SpriteBatch& copy);

    void growBatch();

    void resizeBatch(unsigned int capacity);

    Texture* _texture;
    Effect* _effect;
    RenderState::StateBlock* _stateBlock;
    Texture::Sampler* _sampler;
    Uniform* _samplerUniform;
    Uniform* _projectionUniform;
    bool _customEffect;
    VertexAttribute _vaPosition;
    VertexAttribute _vaTexCoord;
    VertexAttribute _vaColor;
    float _textureWidthRatio;
    float _textureHeightRatio;
    unsigned int _capacity;
    unsigned int _count;
    float* _vertices;
    float* _verticesPtr;
    unsigned short* _indices;
    unsigned short* _indicesPtr;
    unsigned short _index;
    bool _drawing;
    Matrix* _projectionMatrix;
    bool _customProjectionMatrix;
};

}

#endif
