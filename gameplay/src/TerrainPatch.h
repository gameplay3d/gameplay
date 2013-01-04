#ifndef TERRAINPATCH_H_
#define TERRAINPATCH_H_

#include "Model.h"

namespace gameplay
{

class Terrain;

/**
 * Represents a single patch for a Terrain.
 *
 * This is an internal class used exclusively by Terrain.
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
        float xOffset, float zOffset,
        unsigned int maxStep, float verticalSkirtSize);

    /**
     * Adds a single LOD level to the terrain patch.
     */
    void addLOD(float* heights, unsigned int width, unsigned int height,
        unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
        float xOffset, float zOffset,
        unsigned int step, float verticalSkirtSize);

    /**
     * Sets details for a layer of this patch.
     */
    bool setLayer(int index, const char* texturePath, const Vector2& textureRepeat, const char* blendPath, int blendChannel);

    /**
     * Sets the level of detail for the terrain patch.
     */
    void setLod(unsigned int lod);

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
     * Draws the terrain patch.
     */
    void draw(bool wireframe);

    /**
     * Updates the material for the patch.
     */
    bool updateMaterial();

    Terrain* _terrain;
    std::vector<Level*> _levels;
    unsigned int _lod;
    unsigned int _row;
    unsigned int _column;
    std::set<Layer*, LayerCompare> _layers;
    std::vector<Texture::Sampler*> _samplers;
    std::vector<int> _textureIndex;
    std::vector<Vector2> _textureRepeat;
    std::vector<int> _blendIndex;
    std::vector<int> _blendChannel;
    bool _materialDirty;

};

}

#endif
