/*
 * SpriteBatch.h
 */

#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Rectangle.h"
#include "Color.h"
#include "Vector2.h"
#include "Matrix.h"

namespace gameplay
{

/**
 * Enables groups of sprites to be drawn with common settings.
 *
 * This class provides efficient rendering and sorting of two-dimensional
 * sprites. Only a single texture and material can be used with a SpriteBatch.
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
     * If the material parameter is NULL, a default material is used which
     * utilizes an orthographic projection for the currently bound viewport.
     * If a custom material is specified, it must meet the following requirements:
     * <ol>
     * <li>The vertex shader inputs must include a vec3 position, a vec2 tex coord
     * and a vec4 color.
     * <li>The names of the the vertex shader inputs must match the names defined
     * by the VERTEX_ATTRIBUTE_XXX constants.
     * <li>The fragment shader must define at least a single sampler/texture uniform.
     * </ol>
     *
     * @param texture The texture for this sprite batch.
     * @param material An optional material to use with the SpriteBatch.
     * @param initialCapacity An optional initial capacity of the batch (number of sprites).
     * 
     * @return A new SpriteBatch for drawing sprites using the given texture.
     */
    static SpriteBatch* create(Texture* texture, Material* material = NULL, unsigned int initialCapacity = 0);

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
    void draw(const Rectangle& dst, const Rectangle& src, const Color& color = Color::white());

    /**
     * Draws a single sprite.
     * 
     * @param dst The destination position.
     * @param src The source rectangle.
     * @param scale The X and Y scale.
     * @param color The color to tint the sprite. Use white for no tint.
     */
    void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Color& color = Color::white());

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
    void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Color& color,
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
    void draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
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
    void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color);

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
    void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color);

    /**
     * Ends sprite drawing.
     *
     * This method flushes the batch and commits rendering of all sprites that were
     * drawn since the last call to begin().
     */
    void end();

    /**
     * Sets the blend mode to use when rendering.  This blend mode will be set before
     * rendering and then immediately reset to its previous value.
     */
    void setBlendMode(const GLenum sfactor, const GLenum dfactor);

    /**
     * Gets the material used by this batch.
     * 
     * @return The material.
     */
    Material* getMaterial();

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
    Material* _material;
    bool _customMaterial;
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
    GLint _sfactor;
    GLint _dfactor;
    bool _drawing;
    Matrix* _projectionMatrix;
};

}

#endif
