#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Transform.h"
#include "Properties.h"
#include "HeightField.h"
#include "Texture.h"
#include "BoundingBox.h"
#include "TerrainPatch.h"

namespace gameplay
{

class Node;
class TerrainPatch;

/**
 * Defines a Terrain that is capable of rendering large landscapes from 2D heightmap images.
 *
 * Terrains can be constructed from several different heightmap sources:
 *
 * 1. Basic intensity image (PNG), where the intensity of pixel represents the height of the
 *    terrain.
 * 2. 24-bit high precision heightmap image (PNG), which can be generated from a mesh using
 *    gameplay-encoder.
 * 3. 8-bit or 16-bit RAW heightmap image using PC byte ordering (little endian), which is
 *    compatible with many external tools such as World Machine, Unity and more. The file
 *    extension must be either .raw or .r16 for RAW files.
 *
 * Physics/collision is supported by setting a rigid body collision object on the Node that
 * the terrain is attached to. The collision shape should be specified using
 * PhysicsCollisionShape::heightfield(), which will utilize the internal height array of the
 * terrain to define the collision shape. Define a collision object in this way will allow
 * the terrain to automatically interact with other rigid bodies, characters and vehicles in
 * the scene.
 *
 * Surface detail is provided via texture splatting, where multiple texture layers can be added
 * along with blend maps to define how different layers blend with each other. These layers
 * can be defined in terrain properties files, as well as with the setLayer method. The number
 * of supported layers depends on the target hardware, although typically 2-3 levels is
 * sufficient. Multiple blend maps for different layers can be packed into different channels
 * of a single texture for more efficient texture utilization. Levels can be applied across
 * the entire terrain, or in more complex cases, for individual patches only.
 *
 * Surface lighting is achieved with either vertex normals or with a normal map. If a
 * normal map is used, it should be an object-space normal map containing normal vectors for
 * the entire terrain, encoded into the red, green and blue channels of the texture. This is
 * useful as a replacement for vertex normals, especially when using level-of-detail, since
 * it allows you to provide high quality normal vectors regardless of the tessellation or 
 * LOD of the terrain. This also eliminates lighting artifacts/popping from LOD changes,
 * which commonly occurs when vertex normals are used. A terrain normal map can only be
 * specified at creation time, since it requires omission of vertex normal information in
 * the generated terrain geometry data.
 *
 * Internally, Terrain is broken into smaller, more managable patches, which can be culled
 * separately for more efficient rendering. The size of the terrain patches can be controlled
 * via the patchSize property. Patches can be previewed by enabling the DEBUG_PATCHES flag
 * via the setFlag method. Other terrain behavior can also be enabled and disabled using terrain
 * flags.
 * 
 * Level of detail (LOD) is supported using a technique that is similar to texture mipmapping.
 * A distance-to-camera based test, using a simple screen-space error metric is used to decide
 * the appropriate LOD for a terrain patch. The number of LOD levels is 1 by default (which
 * means only the base level is used), but can be specified via the detailLevels property.
 * Using too large a number for detailLevels can result in excessive popping in the distance
 * for very hilly terrains, so a smaller number (2-3) often works best in these cases.
 *
 * Finally, when LOD is enabled, cracks can begin to appear between terrain patches of
 * different LOD levels. If the cracks are only minor (depends on your terrain topology
 * and tetures used), an acceptable appraoch might be to simply use a background clear 
 * color that closely matches your terrain to make the cracks much less visible. However,
 * often that is not acceptable, so the Terrain class also supports a simple solution called
 * "vertical skirts". When enabled (via the skirtScale parameter in the terrain file), a vertical
 * edge will extend down along the sides of all terrain patches, which fills in the crack.
 * This is a very fast approach as it adds only a small number of triangles per patch and requires
 * zero extra CPU time or draw calls, which are often needed for more complex stitching 
 * approaches. In practice, the skirts are often not noticable at all unless the LOD variation
 * is very large and the terrain is excessively hilly on the edge of a LOD transition.
 */
class Terrain : public Ref, public Transform::Listener
{
    friend class Node;
    friend class TerrainPatch;
    friend class PhysicsController;
    friend class PhysicsRigidBody;

public:

    /**
     * Terrain flags.
     */
    enum Flags
    {
        /**
         * Draw terrain patches different colors (off by default).
         */
        DEBUG_PATCHES = 1,

        /**
         * Enables view frustum culling (on by default).
         *
         * Frustum culling uses the scene's active camera. The terrain must be attached
         * to a node that is within a scene for this to work.
         */
        FRUSTUM_CULLING = 2,

         /**
          * Enables level of detail (on by default).
          *
          * This flag enables or disables level of detail, however it does nothing if
          * "detailLevels" was not set to a value greater than 1 in the terrain
          * properties file at creation time.
          */
         LEVEL_OF_DETAIL = 8
    };

    /**
     * Interface for various terrain-specific events that can be handled.
     */
    class Listener
    {
    public:

        virtual ~Listener() { }

        /**
         * Fired when a material is updated for the terrain or a patch within it.
         *
         * This method can be handled to override material parameters for the terrain.
         * Note that this method will usually be fired several times since there are
         * normally separate materials defined per patch.
         *
         * @param terrain The terrain firing the event.
         * @param material The new material.
         */
        virtual void materialUpdated(Terrain* terrain, Material* material) = 0;
    };

    /**
     * Loads a Terrain from the given properties file.
     *
     * The specified properties file can contain a full terrain definition, including a 
     * heightmap (PNG, RAW8, RAW16), level of detail information, patch size, layer texture
     * details and vertical skirt size.
     *
     * @param path Path to a properties file describing the terrain.
     *
     * @return A new Terrain.
     * @script{create}
     */
    static Terrain* create(const char* path);

    /**
     * Creates a new terrain definition from the configuration in the specified Properties object.
     *
     * @param properties Properties object containing the terrain definition.
     *
     * @return A new Terrain.
     *
     * @see create(const char*)
     * @script{create}
     */
    static Terrain* create(Properties* properties);

    /**
     * Creates a terrain from the given heightfield.
     *
     * Terrain geometry is loaded from the given height array, using the specified parameters for
     * size, patch size, detail levels and skirt scale.
     *
     * The newly created terrain increases the reference count of the HeightField.
     *
     * @param heightfield The heightfield object containing height data for the terrain.
     * @param scale A scale to apply to the terrain along the X, Y and Z axes. The terrain and any associated
     *      physics hegihtfield is scaled by this amount. Pass Vector3::one() to use the exact dimensions and heights
     *      in the supplied height array.
     * @param patchSize Size of terrain patches (number of quads).
     * @param detailLevels Number of detail levels to generate for the terrain (a value of one generates only the base
     *      level, resulting in no LOD at runtime.
     * @param skirtScale A positive value indicates that vertical skirts should be generated at the specified
     *      scale, which is relative to the height of the terrain. For example, a value of 0.5f indicates that
     *      vertical skirts should extend down by half of the maximum height of the terrain. A value of zero
     *      disables vertical skirts.
     * @param normalMapPath Path to an object-space normal map to use for terrain lighting, instead of vertex normals.
     *
     * @return A new Terrain.
     * @script{create}
     */
    static Terrain* create(HeightField* heightfield, const Vector3& scale = Vector3::one(), unsigned int patchSize = 32,  
                           unsigned int detailLevels = 1, float skirtScale = 0.0f, const char* normalMapPath = NULL);

    /**
     * Sets the detail textures information for a terrain layer.
     *
     * A detail layer includes a color texture, a repeat count across the terrain for the texture and
     * a region of the texture to use.
     *
     * Optionally, a layer can also include a blend texture, which is used to instruct the terrain how
     * to blend the new layer with the layer underneath it. Blend maps use only a single channel of a 
     * texture and are best supplied by packing the blend map for a layer into the alpha channel of
     * the color texture. Blend maps are always stretched over the entire terrain 
     *
     * The lowest/base layer of the terrain should not include a blend map, since there is no lower
     * level to blend with. All other layers should normally include a blend map. However, since no
     * blend map will result in the texture completely masking the layer underneath it.
     *
     * Detail layers can be applied globally (to the entire terrain), or to one or more specific
     * patches in the terrain. Patches are specified by row and column number, which is dependent
     * on the patch size configuration of your terrain. For layers that span the entire terrain, 
     * the repeat count is relative to the entire terrain. For layers that span only specific
     * patches, the repeat count is relative to those patches only.
     *
     * @param index Layer index number. Layer indexes do not neccessarily need to be sequential and
     *      are used simply to uinquely identify layers, where higher numbers specificy higher-level
     *      layers.
     * @param texturePath Path to the color texture for this layer.
     * @param textureRepeat Repeat count for the color texture across the terrain or patches.
     * @param blendPath Path to the blend texture for this layer (optional).
     * @param blendChannel Channel of the blend texture to sample for the blend map (0 == R, 1 == G, 2 == B, 3 == A).
     * @param row Specifies the row index of patches to use this layer (optional, -1 means all rows).
     * @param column Specifies the column index of patches to use this layer (optional, -1 means all columns).
     *
     * @return True if the layer was successfully set, false otherwise. The most common reason for failure is an
     *      invalid texture path.
     *
     * @script{ignore}
     */
    bool setLayer(int index,
                  const char* texturePath, const Vector2& textureRepeat = Vector2::one(),
                  const char* blendPath = NULL, int blendChannel = 0, 
                  int row = -1, int column = -1);

    /**
     * Returns the node that this terrain is bound to.
     *
     * @return The node this terrain is bound to, or NULL if the terrain is not bound to a node.
     */
    Node* getNode() const;

    /**
     * Determines if the specified terrain flag is currently set.
     */
    bool isFlagSet(Flags flag) const;

    /**
     * Enables or disables the specified terrain flag.
     *
     * @param flag The terrain flag to set.
     * @param on True to turn the flag on, false to turn it off.
     */
    void setFlag(Flags flag, bool on);

    /**
     * Returns the total number of terrain patches.
     *
     * @return The number of terrain patches.
     */
    unsigned int getPatchCount() const;

    /**
     * Returns the number of patches that are currently visible from the scene camera's point of view.
     *
     * If the terrain is not attached to a scene, or if there is no active scene camera, this method
     * returns zero.
     *
     * This method is not exact - it may return false positives since it only determines if the
     * bounding box of terrain patches intersect the view frustum. Should be used for debug 
     * purposes only.
     *
     * @return The number of currently visible patches.
     */
    unsigned int getVisiblePatchCount() const;

    /**
     * Returns the total number of triangles for this terrain at the base LOD.
     *
     * @return The total triangle count for the terrain at the base LOD.
     */
    unsigned int getTriangleCount() const;

    /**
     * Returns the number of currently visible triangles, taking LOD and view frustum
     * (if enabled) into consideration.
     *
     * @return The current visible triangle count.
     */
    unsigned int getVisibleTriangleCount() const;

    /**
     * Returns the local bounding box for this terrain.
     *
     * @return The local bounding box for the terrain.
     */
    const BoundingBox& getBoundingBox() const;

    /**
     * Returns the world-space height of the terrain at the specified position on the X,Z plane.
     *
     * The specified X and Z coordinates should be in world units and may fall between height values.
     * In this case, an interpolated value will be returned between neighboring heightfield heights.
     * If the specified point lies outside of the terrain, it is clamped to the terrain boundaries.
     *
     * @param x The X coordinate, in world space.
     * @param z The Z coordinate, in world space.
     *
     * @return The height at the specified point, clamped to the boundaries of the terrain.
     */
    float getHeight(float x, float z) const;

    /**
     * Draws the terrain.
     *
     * @param wireframe True to draw the terrain as wireframe, false to draw it solid (default).
     */
    void draw(bool wireframe = false);

    /**
     * @see Transform::Listener::transformChanged.
     *
     * Internal use only.
     *
     * @script{ignore}
     */
    void transformChanged(Transform* transform, long cookie);

    /**
     * Adds a listener to this terrain.
     *
     * @param listener Listener to start receiving terrain events.
     */
    void addListener(Terrain::Listener* listener);

    /**
     * Removes a listener from this terrain.
     *
     * @param listener Listener to stop receiving terrain events.
     */
    void removeListener(Terrain::Listener* listener);

    /**
     * Returns the world matrix of the terrain, factoring in terrain local scaling.
     *
     * @return The world matrix for the terrain.
     */
    const Matrix& getWorldMatrix() const;

    /**
     * Returns the terrain's inverse world matrix.
     *
     * @return The inverse world matrix for the terrain.
     */
    const Matrix& getInverseWorldMatrix() const;

    /**
     * Returns a matrix to be used for transforming normal vectors for the terrain.
     *
     * @return The matrix used for normal vector transformation for the terrain.
     */
    const Matrix& getNormalMatrix() const;

    /**
     * Returns the world view matrix for the terrain, factoring in terrain local scaling.
     *
     * @return The world-view matrix for the terrain.
     */
    const Matrix& getWorldViewMatrix() const;

    /**
     * Returns the world view projection matrix for the terrain, factoring in terrain local scaling.
     *
     * @return The world-view-projection matrix for the terrain.
     */
    const Matrix& getWorldViewProjectionMatrix() const;

private:

    /**
     * Constructor.
     */
    Terrain();

    /**
     * Hidden copy constructor.
     */
    Terrain(const Terrain&);

    /**
     * Hidden copy assignment operator.
     */
    Terrain& operator=(const Terrain&);

    /**
     * Destructor.
     */
    ~Terrain();

    /**
     * Internal method for creating terrain.
     */
    static Terrain* create(HeightField* heightfield, const Vector3& scale, unsigned int patchSize, unsigned int detailLevels, float skirtScale, const char* normalMapPath, Properties* properties);

    /**
     * Internal method for creating terrain.
     */
    static Terrain* create(const char* path, Properties* properties);

    /**
     * Sets the node that the terrain is attached to.
     */
    void setNode(Node* node);

    HeightField* _heightfield;
    Node* _node;
    std::vector<TerrainPatch*> _patches;
    Vector3 _localScale;
    Texture::Sampler* _normalMap;
    unsigned int _flags;
    mutable Matrix _worldMatrix;
    mutable Matrix _inverseWorldMatrix;
    mutable Matrix _normalMatrix;
    mutable unsigned int _dirtyFlags;
    BoundingBox _boundingBox;
    std::vector<Terrain::Listener*> _listeners;
};

}

#endif
