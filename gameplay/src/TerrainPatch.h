#ifndef TERRAINPATCH_H_
#define TERRAINPATCH_H_

#include "Model.h"
#include "Camera.h"

namespace gameplay
{

class Terrain;
class TerrainAutoBindingResolver;

/**
 * Defines a single patch for a Terrain.
 */
class TerrainPatch : public Camera::Listener
{
    friend class Terrain;
    friend class TerrainAutoBindingResolver;

public:

    /**
     * Gets the number of material for this patch for all level of details.
     *
     * @return The number of material for this patch for all level of details. 
     */
    unsigned int getMaterialCount() const;

    /**
     * Gets the material for the specified level of detail index or -1 for the current level of detail
     * based on the scene camera.
     *
     * @param index The index for the level of detail to get the material for.
     */
    Material* getMaterial(int index = -1) const;

    /**
     * Gets the local bounding box for this patch, at the base LOD level.
     */
    const BoundingBox& getBoundingBox(bool worldSpace) const;

    /**
     * @see Camera::Listener
     */
    void cameraChanged(Camera* camera);

    /**
     * Internal use only.
     *
     * @script{ignore}
     */
    static std::string passCallback(Pass* pass, void* cookie);

private:

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

    static TerrainPatch* create(Terrain* terrain, unsigned int index,
                                unsigned int row, unsigned int column,
                                float* heights, unsigned int width, unsigned int height,
                                unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
                                float xOffset, float zOffset, unsigned int maxStep, float verticalSkirtSize);

    void addLOD(float* heights, unsigned int width, unsigned int height,
                unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
                float xOffset, float zOffset, unsigned int step, float verticalSkirtSize);


    bool setLayer(int index, const char* texturePath, const Vector2& textureRepeat, const char* blendPath, int blendChannel);

    void deleteLayer(Layer* layer);

    int addSampler(const char* path);

    unsigned int draw(bool wireframe);

    bool updateMaterial();

    unsigned int computeLOD(Camera* camera, const BoundingBox& worldBounds);

    const Vector3& getAmbientColor() const;

    void setMaterialDirty();

    float computeHeight(float* heights, unsigned int width, unsigned int x, unsigned int z);

    void updateNodeBindings();

    std::string passCreated(Pass* pass);

    Terrain* _terrain;
    unsigned int _index;
    unsigned int _row;
    unsigned int _column;
    std::vector<Level*> _levels;
    std::set<Layer*, LayerCompare> _layers;
    std::vector<Texture::Sampler*> _samplers;
    mutable BoundingBox _boundingBox;
    mutable BoundingBox _boundingBoxWorld;
    mutable Camera* _camera;
    mutable unsigned int _level;
    mutable int _bits;
};

}

#endif
