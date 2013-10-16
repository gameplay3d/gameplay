#ifndef TERRAINPATCH_H_
#define TERRAINPATCH_H_

#include "Model.h"
#include "Camera.h"

namespace gameplay
{

class Terrain;

/**
 * Represents a single patch for a Terrain.
 *
 * This is an internal class used exclusively by Terrain.
 *
 * @script{ignore}
 */
class TerrainPatch
{
    friend class Terrain;

private:

    struct Layer
    {
        Layer();
        Layer(const Layer&);
        ~Layer();
        Layer& operator=(const Layer&);

        int index;
        int row;
        int column;
        int textureIndex;
        Vector2 textureRepeat;
        int blendIndex;
        int blendChannel;
    };

    struct Level
    {
        Model* model;

        Level();
    };

    struct LayerCompare
    {
        bool operator() (const Layer* lhs, const Layer* rhs) const;
    };

    /**
     * Constructor.
     */
    TerrainPatch();

    /**
     * Hidden copy constructor.
     */
    TerrainPatch(const TerrainPatch&);

    /**
     * Hidden copy assignment operator.
     */
    TerrainPatch& operator=(const TerrainPatch&);

    /**
     * Destructor.
     */
    ~TerrainPatch();

    /**
     * Internal method to create new terrain patch.
     */
    static TerrainPatch* create(Terrain* terrain, 
                                unsigned int row, unsigned int column,
                                float* heights, unsigned int width, unsigned int height,
                                unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
                                float xOffset, float zOffset, unsigned int maxStep, float verticalSkirtSize);

    /**
     * Adds a single LOD level to the terrain patch.
     */
    void addLOD(float* heights, unsigned int width, unsigned int height,
                unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
                float xOffset, float zOffset, unsigned int step, float verticalSkirtSize);

    /**
     * Sets details for a layer of this patch.
     */
    bool setLayer(int index, const char* texturePath, const Vector2& textureRepeat, const char* blendPath, int blendChannel);

    /**
     * Adds a sampler to the patch.
     */
    int addSampler(const char* path);

    /**
     * Deletes the specified layer.
     */
    void deleteLayer(Layer* layer);

    /**
     * Determines whether this patch is current visible by the scene's active camera.
     */
    bool isVisible() const;

    /**
     * Returns the triangle count of the base LOD level of this terrain patch.
     */
    unsigned int getTriangleCount() const;

    /**
     * Returns the currently visible triangle count, taking the current LOD into account.
     */
    unsigned int getVisibleTriangleCount() const;

    /**
     * Draws the terrain patch.
     */
    unsigned int draw(bool wireframe);

    /**
     * Updates the material for the patch.
     */
    bool updateMaterial();

    /**
     * Computes the current LOD for this patch, from the viewpoint of the specified camera.
     */
    size_t computeLOD(Camera* camera, const BoundingBox& worldBounds) const;

    /**
     * Returns the local bounding box for this patch, at the base LOD level.
     */
    BoundingBox getBoundingBox(bool worldSpace) const;

    const Vector3& getAmbientColor() const;

    Terrain* _terrain;
    std::vector<Level*> _levels;
    unsigned int _row;
    unsigned int _column;
    std::set<Layer*, LayerCompare> _layers;
    std::vector<Texture::Sampler*> _samplers;
    bool _materialDirty;
    BoundingBox _boundingBox;

};

}

#endif
