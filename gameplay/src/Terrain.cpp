#include "Base.h"
#include "Terrain.h"
#include "TerrainPatch.h"
#include "Node.h"
#include "FileSystem.h"

namespace gameplay
{

// The default square size of terrain patches for a terrain that
// does not have an explicitly specified patch size.
//
#define DEFAULT_TERRAIN_PATCH_SIZE 32

// The default height of a terrain that does not have an explicitly
// specified terrain size, expressed as a ratio of the average
// of the dimensions of the terrain heightfield:
//
//   heightMax = (image.width + image.height) / 2 * DEFAULT_TERRAIN_HEIGHT_RATIO
//
#define DEFAULT_TERRAIN_HEIGHT_RATIO 0.3f

// Terrain dirty flag bits
#define TERRAIN_DIRTY_WORLD_MATRIX 1
#define TERRAIN_DIRTY_INV_WORLD_MATRIX 2
#define TERRAIN_DIRTY_NORMAL_MATRIX 4

/**
 * @script{ignore}
 */
float getDefaultHeight(unsigned int width, unsigned int height);

Terrain::Terrain() :
    _heightfield(NULL), _node(NULL), _normalMap(NULL), _flags(FRUSTUM_CULLING | LEVEL_OF_DETAIL),
    _dirtyFlags(TERRAIN_DIRTY_WORLD_MATRIX | TERRAIN_DIRTY_INV_WORLD_MATRIX | TERRAIN_DIRTY_NORMAL_MATRIX)
{
}

Terrain::~Terrain()
{
    _listeners.clear();

    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        SAFE_DELETE(_patches[i]);
    }

    if (_node)
        _node->removeListener(this);

    SAFE_RELEASE(_normalMap);
    SAFE_RELEASE(_heightfield);
}

Terrain* Terrain::create(const char* path)
{
    return create(path, NULL);
}

Terrain* Terrain::create(Properties* properties)
{
    return create(properties->getNamespace(), properties);
}

Terrain* Terrain::create(const char* path, Properties* properties)
{
    // Terrain properties
    Properties* p = properties;
    Properties* pTerrain = NULL;
    bool externalProperties = (p != NULL);
    HeightField* heightfield = NULL;
    Vector3 terrainSize;
    int patchSize = 0;
    int detailLevels = 1;
    float skirtScale = 0;
    const char* normalMap = NULL;

    if (!p && path)
    {
        p = Properties::create(path);
    }

    if (p)
    {
        pTerrain = strlen(p->getNamespace()) > 0 ? p : p->getNextNamespace();
        if (pTerrain == NULL)
        {
            GP_WARN("Invalid terrain definition.");
            if (!externalProperties)
                SAFE_DELETE(p);
            return NULL;
        }

        // Read heightmap info
        Properties* pHeightmap = pTerrain->getNamespace("heightmap", true);
        if (pHeightmap)
        {
            // Read heightmap path
            std::string heightmap;
            if (!pHeightmap->getPath("path", &heightmap))
            {
                GP_WARN("No 'path' property supplied in heightmap section of terrain definition: %s", path);
                if (!externalProperties)
                    SAFE_DELETE(p);
                return NULL;
            }

            std::string ext = FileSystem::getExtension(heightmap.c_str());
            if (ext == ".PNG")
            {
                // Read normalized height values from heightmap image
                heightfield = HeightField::createFromImage(heightmap.c_str(), 0, 1);
            }
            else if (ext == ".RAW" || ext == ".R16")
            {
                // Require additional properties to be specified for RAW files
                Vector2 imageSize;
                if (!pHeightmap->getVector2("size", &imageSize))
                {
                    GP_WARN("Invalid or missing 'size' attribute in heightmap defintion of terrain definition: %s", path);
                    if (!externalProperties)
                        SAFE_DELETE(p);
                    return NULL;
                }

                // Read normalized height values from RAW file
                heightfield = HeightField::createFromRAW(heightmap.c_str(), (unsigned int)imageSize.x, (unsigned int)imageSize.y, 0, 1);
            }
            else
            {
                // Unsupported heightmap format
                GP_WARN("Unsupported heightmap format ('%s') in terrain definition: %s", heightmap.c_str(), path);
                if (!externalProperties)
                    SAFE_DELETE(p);
                return NULL;
            }
        }
        else
        {
            // Try to read 'heightmap' as a simple string property
            std::string heightmap;
            if (!pTerrain->getPath("heightmap", &heightmap))
            {
                GP_WARN("No 'heightmap' property supplied in terrain definition: %s", path);
                if (!externalProperties)
                    SAFE_DELETE(p);
                return NULL;
            }

            std::string ext = FileSystem::getExtension(heightmap.c_str());
            if (ext == ".PNG")
            {
                // Read normalized height values from heightmap image
                heightfield = HeightField::createFromImage(heightmap.c_str(), 0, 1);
            }
            else if (ext == ".RAW" || ext == ".R16")
            {
                GP_WARN("RAW heightmaps must be specified inside a heightmap block with width and height properties.");
                if (!externalProperties)
                    SAFE_DELETE(p);
                return NULL;
            }
            else
            {
                GP_WARN("Unsupported 'heightmap' format ('%s') in terrain definition: %s.", heightmap.c_str(), path);
                if (!externalProperties)
                    SAFE_DELETE(p);
                return NULL;
            }
        }

        // Read terrain 'size'
        if (pTerrain->exists("size"))
        {
            if (!pTerrain->getVector3("size", &terrainSize))
            {
                GP_WARN("Invalid 'size' value ('%s') in terrain definition: %s", pTerrain->getString("size"), path);
            }
        }

        // Read terrain 'patch size'
        if (pTerrain->exists("patchSize"))
        {
            patchSize = pTerrain->getInt("patchSize");
        }

        // Read terrain 'detailLevels'
        if (pTerrain->exists("detailLevels"))
        {
            detailLevels = pTerrain->getInt("detailLevels");
        }

        // Read 'skirtScale'
        if (pTerrain->exists("skirtScale"))
        {
            skirtScale = pTerrain->getFloat("skirtScale");
        }

        // Read 'normalMap'
        normalMap = pTerrain->getString("normalMap");
    }

    if (heightfield == NULL)
    {
        GP_WARN("Failed to read heightfield heights for terrain definition: %s", path);
        if (!externalProperties)
            SAFE_DELETE(p);
        return NULL;
    }

    if (terrainSize.isZero())
    {
        terrainSize.set(heightfield->getColumnCount(), getDefaultHeight(heightfield->getColumnCount(), heightfield->getRowCount()), heightfield->getRowCount());
    }

    if (patchSize <= 0 || patchSize > (int)heightfield->getColumnCount() || patchSize > (int)heightfield->getRowCount())
    {
        patchSize = std::min(heightfield->getRowCount(), std::min(heightfield->getColumnCount(), (unsigned int)DEFAULT_TERRAIN_PATCH_SIZE));
    }

    if (detailLevels <= 0)
        detailLevels = 1;

    if (skirtScale < 0)
        skirtScale = 0;

    // Compute terrain scale
    Vector3 scale(terrainSize.x / (heightfield->getColumnCount()-1), terrainSize.y, terrainSize.z / (heightfield->getRowCount()-1));

    // Create terrain
    Terrain* terrain = create(heightfield, scale, (unsigned int)patchSize, (unsigned int)detailLevels, skirtScale, normalMap, pTerrain);

    if (!externalProperties)
        SAFE_DELETE(p);

    return terrain;
}

Terrain* Terrain::create(HeightField* heightfield, const Vector3& scale, unsigned int patchSize, unsigned int detailLevels, float skirtScale, const char* normalMapPath)
{
    return create(heightfield, scale, patchSize, detailLevels, skirtScale, normalMapPath, NULL);
}

Terrain* Terrain::create(HeightField* heightfield, const Vector3& scale, unsigned int patchSize, unsigned int detailLevels, float skirtScale, const char* normalMapPath, Properties* properties)
{
    GP_ASSERT(heightfield);

    unsigned int width = heightfield->getColumnCount();
    unsigned int height = heightfield->getRowCount();

    // Create the terrain object
    Terrain* terrain = new Terrain();
    terrain->_heightfield = heightfield;
    terrain->_localScale = scale;

    // Store reference to bounding box (it is calculated and updated from TerrainPatch)
    BoundingBox& bounds = terrain->_boundingBox;

    if (normalMapPath)
        terrain->_normalMap = Texture::Sampler::create(normalMapPath, true);

    float halfWidth = (width - 1) * 0.5f;
    float halfHeight = (height - 1) * 0.5f;
    unsigned int maxStep = (unsigned int)std::pow(2.0, (double)(detailLevels-1));

    // Create terrain patches
    unsigned int x1, x2, z1, z2;
    unsigned int row = 0, column = 0;
    for (unsigned int z = 0; z < height-1; z = z2, ++row)
    {
        z1 = z;
        z2 = std::min(z1 + patchSize, height-1);

        for (unsigned int x = 0; x < width-1; x = x2, ++column)
        {
            x1 = x;
            x2 = std::min(x1 + patchSize, width-1);

            // Create this patch
            TerrainPatch* patch = TerrainPatch::create(terrain, row, column, heightfield->getArray(), width, height, x1, z1, x2, z2, -halfWidth, -halfHeight, maxStep, skirtScale);
            terrain->_patches.push_back(patch);

            // Append the new patch's local bounds to the terrain local bounds
            bounds.merge(patch->getBoundingBox(false));
        }
    }

    // Read additional layer information from properties (if specified)
    if (properties)
    {
        // Parse terrain layers
        Properties* lp;
        int index = -1;
        while ((lp = properties->getNextNamespace()) != NULL)
        {
            if (strcmp(lp->getNamespace(), "layer") == 0)
            {
                // If there is no explicitly specified index for this layer, assume it's the 'next' layer
                if (lp->exists("index"))
                    index = lp->getInt("index");
                else
                    ++index;

                std::string textureMap;
                const char* textureMapPtr = NULL;
                std::string blendMap;
                const char* blendMapPtr = NULL;
                Vector2 textureRepeat;
                int blendChannel = 0;
                int row = -1, column = -1;
                Vector4 temp;

                // Read layer textures
                Properties* t = lp->getNamespace("texture", true);
                if (t)
                {
                    if (t->getPath("path", &textureMap))
                    {
                        textureMapPtr = textureMap.c_str();
                    }
                    if (!t->getVector2("repeat", &textureRepeat))
                        textureRepeat.set(1,1);
                }

                Properties* b = lp->getNamespace("blend", true);
                if (b)
                {
                    if (b->getPath("path", &blendMap))
                    {
                        blendMapPtr = blendMap.c_str();
                    }
                    const char* channel = b->getString("channel");
                    if (channel && strlen(channel) > 0)
                    {
                        char c = std::toupper(channel[0]);
                        if (c == 'R' || c == '0')
                            blendChannel = 0;
                        else if (c == 'G' || c == '1')
                            blendChannel = 1;
                        else if (c == 'B' || c == '2')
                            blendChannel = 2;
                        else if (c == 'A' || c == '3')
                            blendChannel = 3;
                    }
                }

                // Get patch row/columns that this layer applies to.
                if (lp->exists("row"))
                    row = lp->getInt("row");
                if (lp->exists("column"))
                    column = lp->getInt("column");

                if (!terrain->setLayer(index, textureMapPtr, textureRepeat, blendMapPtr, blendChannel, row, column))
                {
                    GP_WARN("Failed to load terrain layer: %s", textureMap.c_str());
                }
            }
        }
    }

    // Load materials for all patches
    for (size_t i = 0, count = terrain->_patches.size(); i < count; ++i)
        terrain->_patches[i]->updateMaterial();

    return terrain;
}

void Terrain::setNode(Node* node)
{
    if (_node != node)
    {
        if (_node)
            _node->removeListener(this);

        _node = node;

        if (_node)
            _node->addListener(this);

        _dirtyFlags |= TERRAIN_DIRTY_WORLD_MATRIX | TERRAIN_DIRTY_INV_WORLD_MATRIX | TERRAIN_DIRTY_NORMAL_MATRIX;
    }
}

bool Terrain::setLayer(int index, const char* texturePath, const Vector2& textureRepeat, const char* blendPath, int blendChannel, int row, int column)
{
    if (!texturePath)
        return false;

    // Set layer on applicable patches
    bool result = true;
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        TerrainPatch* patch = _patches[i];

        if ((row == -1 || (int)patch->_row == row) && (column == -1 || (int)patch->_column == column))
        {
            if (!patch->setLayer(index, texturePath, textureRepeat, blendPath, blendChannel))
                result = false;
        }
    }

    return result;
}

Node* Terrain::getNode() const
{
    return _node;
}

bool Terrain::isFlagSet(Flags flag) const
{
    return (_flags & flag) == flag;
}

void Terrain::setFlag(Flags flag, bool on)
{
    bool changed = false;

    if (on)
    {
        if ((_flags & flag) == 0)
        {
            _flags |= flag;
            changed = true;
        }
    }
    else
    {
        if ((_flags & flag) == flag)
        {
            _flags &= ~flag;
            changed = true;
        }
    }

    if (flag == DEBUG_PATCHES && changed)
    {
        // Dirty all materials since they need to be updated to support debug drawing
        for (size_t i = 0, count = _patches.size(); i < count; ++i)
            _patches[i]->_materialDirty = true;
    }
}

unsigned int Terrain::getPatchCount() const
{
    return _patches.size();
}

unsigned int Terrain::getVisiblePatchCount() const
{
    unsigned int visibleCount = 0;
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        if (_patches[i]->isVisible())
            ++visibleCount;
    }
    return visibleCount;
}

unsigned int Terrain::getTriangleCount() const
{
    unsigned int triangleCount = 0;
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        triangleCount += _patches[i]->getTriangleCount();
    }
    return triangleCount;
}

unsigned int Terrain::getVisibleTriangleCount() const
{
    unsigned int triangleCount = 0;
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        triangleCount += _patches[i]->getVisibleTriangleCount();
    }
    return triangleCount;
}

const BoundingBox& Terrain::getBoundingBox() const
{
    return _boundingBox;
}

float Terrain::getHeight(float x, float z) const
{
    // Calculate the correct x, z position relative to the heightfield data.
    float cols = _heightfield->getColumnCount();
    float rows = _heightfield->getRowCount();

    GP_ASSERT(cols > 0);
    GP_ASSERT(rows > 0);

    // Since the specified coordinates are in world space, we need to use the 
    // inverse of our world matrix to transform the world x,z coords back into
    // local heightfield coordinates for indexing into the height array.
    Vector3 v = getInverseWorldMatrix() * Vector3(x, 0.0f, z);
    x = v.x + (cols - 1) * 0.5f;
    z = v.z + (rows - 1) * 0.5f;

    // Get the unscaled height value from the HeightField
    float height = _heightfield->getHeight(x, z);

    // Now apply world scale (this includes local terrain scale) to the heightfield value
    Vector3 worldScale;
    getWorldMatrix().getScale(&worldScale);
    height *= worldScale.y;

    return height;
}

void Terrain::draw(bool wireframe)
{
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        _patches[i]->draw(wireframe);
    }
}

void Terrain::transformChanged(Transform* transform, long cookie)
{
    _dirtyFlags |= TERRAIN_DIRTY_WORLD_MATRIX | TERRAIN_DIRTY_INV_WORLD_MATRIX | TERRAIN_DIRTY_NORMAL_MATRIX;
}

void Terrain::addListener(Terrain::Listener* listener)
{
    _listeners.push_back(listener);

    // Fire initial events in case this listener may have missed them
    for (size_t i = 0, patchCount = _patches.size(); i < patchCount; ++i)
    {
        TerrainPatch* patch = _patches[i];
        for (size_t j = 0, levelCount = patch->_levels.size(); j < levelCount; ++j)
        {
            TerrainPatch::Level* level = patch->_levels[j];
            Material* material = level->model ? level->model->getMaterial() : NULL;
            if (material)
            {
                // Fire materialUpdated event for materials that are already active
                for (size_t k = 0, lcount = _listeners.size(); k < lcount; ++k)
                {
                    _listeners[k]->materialUpdated(this, material);
                }
            }
        }
    }
}

void Terrain::removeListener(Terrain::Listener* listener)
{
    std::vector<Terrain::Listener*>::iterator itr = std::find(_listeners.begin(), _listeners.end(), listener);
    if (itr != _listeners.end())
        _listeners.erase(itr);
}

const Matrix& Terrain::getWorldMatrix() const
{
    if (_dirtyFlags & TERRAIN_DIRTY_WORLD_MATRIX)
    {
        _dirtyFlags &= ~TERRAIN_DIRTY_WORLD_MATRIX;

        // Apply our attached node's world matrix
        if (_node)
        {
            _worldMatrix = _node->getWorldMatrix();
        }
        else
        {
            _worldMatrix.setIdentity();
        }

        // Factor in our local scaling
        _worldMatrix.scale(_localScale);
    }

    return _worldMatrix;
}

const Matrix& Terrain::getInverseWorldMatrix() const
{
    if (_dirtyFlags & TERRAIN_DIRTY_INV_WORLD_MATRIX)
    {
        _dirtyFlags &= ~TERRAIN_DIRTY_INV_WORLD_MATRIX;

        getWorldMatrix().invert(&_inverseWorldMatrix);
    }

    return _inverseWorldMatrix;
}

const Matrix& Terrain::getNormalMatrix() const
{
    if (_dirtyFlags & TERRAIN_DIRTY_NORMAL_MATRIX)
    {
        _dirtyFlags &= ~TERRAIN_DIRTY_NORMAL_MATRIX;

        getInverseWorldMatrix().transpose(&_normalMatrix);
    }

    return _normalMatrix;
}

const Matrix& Terrain::getWorldViewMatrix() const
{
    static Matrix worldView;

    if (_node)
        Matrix::multiply(_node->getViewMatrix(), getWorldMatrix(), &worldView);
    else
        worldView = getWorldMatrix(); // no node, so nothing to get view from

    return worldView;
}

const Matrix& Terrain::getWorldViewProjectionMatrix() const
{
    static Matrix worldViewProj;

    if (_node)
        Matrix::multiply(_node->getViewProjectionMatrix(), getWorldMatrix(), &worldViewProj);
    else
        worldViewProj = getWorldMatrix(); // no node, so nothing to get viewProjection from

    return worldViewProj;
}

float getDefaultHeight(unsigned int width, unsigned int height)
{
    // When terrain height is not specified, we'll use a default height of ~ 0.3 of the image dimensions
    return ((width + height) * 0.5f) * DEFAULT_TERRAIN_HEIGHT_RATIO;
}

}
