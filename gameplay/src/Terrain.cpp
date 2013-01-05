#include "Base.h"
#include "Terrain.h"
#include "Node.h"
#include "Image.h"
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
//   maxHeight = (image.width + image.height) / 2 * DEFAULT_TERRAIN_HEIGHT_RATIO
//
#define DEFAULT_TERRAIN_HEIGHT_RATIO 0.3f

/**
 * @script{ignore}
 */
std::string getExtension(const char* filename);

/**
 * @script{ignore}
 */
float getDefaultHeight(unsigned int width, unsigned int height);

Terrain::Terrain() :
    _heightfield(NULL), _node(NULL), _normalMap(NULL), _flags(ENABLE_FRUSTUM_CULLING | ENABLE_LEVEL_OF_DETAIL), _worldMatrixDirty(true)
{
}

Terrain::~Terrain()
{
    if (_node)
        _node->removeListener(this);

    SAFE_RELEASE(_normalMap);
    SAFE_RELEASE(_heightfield);
}

Terrain* Terrain::create(const char* terrainFile)
{
    // Terrain properties
    HeightField* heightfield = NULL;
    Vector3 terrainSize;
    int patchSize = 0;
    int detailLevels = 1;
    float skirtScale = 0;
    Properties* p = NULL;
    Properties* pTerrain = NULL;
    const char* normalMap = NULL;

    std::string ext = getExtension(terrainFile);
    if (ext == "PNG")
    {
        // Load terrain directly from a heightmap image
        heightfield = HeightField::create(terrainFile, 0, 0, 0, 1);
    }
    else
    {
        // Read terrain from properties file
        p = Properties::create(terrainFile);
        if (p == NULL)
            return NULL;

        pTerrain = strlen(p->getNamespace()) > 0 ? p : p->getNextNamespace();
        if (pTerrain == NULL)
        {
            GP_WARN("Invalid terrain definition file.");
            SAFE_DELETE(p);
            return NULL;
        }

        // Read heightmap info
        Properties* pHeightmap = pTerrain->getNamespace("heightmap", true);
        if (pHeightmap)
        {
            // Read heightmap path
            const char* heightmap = pHeightmap->getString("path");
            if (strlen(heightmap) == 0)
            {
                GP_WARN("No 'path' property supplied in heightmap section of terrain definition file: %s", terrainFile);
                SAFE_DELETE(p);
                return NULL;
            }

            ext = getExtension(heightmap);
            if (ext == "PNG")
            {
                // Read normalized height values from heightmap image
                heightfield = HeightField::create(heightmap, 0, 0, 0, 1);
            }
            else if (ext == "RAW")
            {
                // Require additional properties to be specified for RAW files
                Vector2 imageSize;
                if (!pHeightmap->getVector2("size", &imageSize))
                {
                    GP_WARN("Invalid or missing 'size' attribute in heightmap defintion of terrain file: %s", terrainFile);
                    SAFE_DELETE(p);
                    return NULL;
                }

                // Read normalized height values from RAW file
                heightfield = HeightField::create(heightmap, (unsigned int)imageSize.x, (unsigned int)imageSize.y, 0, 1);
            }
            else
            {
                // Unsupported heightmap format
                GP_WARN("Unsupported heightmap format ('%s') in terrain definition file: %s", heightmap, terrainFile);
                SAFE_DELETE(p);
                return NULL;
            }
        }
        else
        {
            // Try to read 'heightmap' as a simple string property
            const char* heightmap = pTerrain->getString("heightmap");
            if (heightmap == NULL || strlen(heightmap) == 0)
            {
                GP_WARN("No 'heightmap' property supplied in terrain definition file: %s", terrainFile);
                SAFE_DELETE(p);
                return NULL;
            }

            ext = getExtension(heightmap);
            if (ext == "PNG")
            {
                // Read normalized height values from heightmap image
                heightfield = HeightField::create(heightmap, 0, 0, 0, 1);
            }
            else if (ext == "RAW")
            {
                GP_WARN("RAW heightmaps must be specified inside a heightmap block with width and height properties.");
                SAFE_DELETE(p);
                return NULL;
            }
            else
            {
                GP_WARN("Unsupported 'heightmap' format ('%s') in terrain definition file: %s.", heightmap, terrainFile);
                SAFE_DELETE(p);
                return NULL;
            }
        }

        // Read terrain 'size'
        if (pTerrain->exists("size"))
        {
            if (!pTerrain->getVector3("size", &terrainSize))
            {
                GP_WARN("Invalid 'size' value ('%s') in terrain definition file: %s", pTerrain->getString("size"), terrainFile);
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
        GP_WARN("Failed to read heightfield heights for terrain: %s", terrainFile);
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
        while (lp = properties->getNextNamespace())
        {
            if (strcmp(lp->getNamespace(), "layer") == 0)
            {
                // If there is no explicitly specified index for this layer, assume it's the 'next' layer
                if (lp->exists("index"))
                    index = lp->getInt("index");
                else
                    ++index;

                const char* textureMap = NULL;
                const char* blendMap = NULL;
                Vector2 textureRepeat;
                int blendChannel = 0;
                int row = -1, column = -1;
                Vector4 temp;

                // Read layer textures
                Properties* t = lp->getNamespace("texture", true);
                if (t)
                {
                    textureMap = t->getString("path");
                    if (!t->getVector2("repeat", &textureRepeat))
                        textureRepeat.set(1,1);
                }

                Properties* b = lp->getNamespace("blend", true);
                if (b)
                {
                    blendMap = b->getString("path");
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

                if (!terrain->setLayer(index, textureMap, textureRepeat, blendMap, blendChannel, row, column))
                {
                    GP_WARN("Failed to load terrain layer: %s", textureMap);
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

        _worldMatrixDirty = true;
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

        if ((row == -1 || patch->_row == row) && (column == -1 || patch->_column == column))
        {
            if (!patch->setLayer(index, texturePath, textureRepeat, blendPath, blendChannel))
                result = false;
        }
    }

    return result;
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

void Terrain::draw(bool wireframe)
{
    for (size_t i = 0, count = _patches.size(); i < count; ++i)
    {
        _patches[i]->draw(wireframe);
    }
}

void Terrain::transformChanged(Transform* transform, long cookie)
{
    _worldMatrixDirty = true;
}

const Matrix& Terrain::getWorldMatrix() const
{
    if (_worldMatrixDirty)
    {
        _worldMatrixDirty = false;

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

const Matrix& Terrain::getWorldViewProjectionMatrix() const
{
    static Matrix worldViewProj;

    if (_node)
        Matrix::multiply(_node->getViewProjectionMatrix(), getWorldMatrix(), &worldViewProj);
    else
        worldViewProj = getWorldMatrix(); // no node, so nothing to get viewProjection from

    return worldViewProj;
}

// Returns the uppercase extension of a file
std::string getExtension(const char* filename)
{
    const char* str = strrchr(filename, '.');
    if (str == NULL)
        return NULL;

    std::string ext;
    size_t len = strlen(str);
    for (size_t i = 1; i < len; ++i)
        ext += std::toupper(str[i]);

    return ext;
}

float getDefaultHeight(unsigned int width, unsigned int height)
{
    // When terrain height is not specified, we'll use a default height of ~ 0.3 of the image dimensions
    return ((width + height) * 0.5f) * DEFAULT_TERRAIN_HEIGHT_RATIO;
}

Terrain::HeightField::HeightField(unsigned int columns, unsigned int rows)
    : _array(NULL), _cols(columns), _rows(rows)
{
    _array = new float[columns * rows];
}

Terrain::HeightField::~HeightField()
{
    SAFE_DELETE_ARRAY(_array);
}

Terrain::HeightField* Terrain::HeightField::create(unsigned int columns, unsigned int rows)
{
    return new HeightField(columns, rows);
}

/**
 * @script{ignore}
 */
float normalizedHeightPacked(float r, float g, float b)
{
    // This formula is intended for 24-bit packed heightmap images (that are generated
    // with gameplay-encoder. However, it is also compatible with normal grayscale 
    // heightmap images, with an error of approximately 0.4%. This can be seen by
    // setting r=g=b=x and comparing the grayscale height expression to the packed
    // height expression: the error is 2^-8 + 2^-16 which is just under 0.4%.
    return (256.0f*r + g + 0.00390625f*b) / 65536.0f;
}

Terrain::HeightField* Terrain::HeightField::create(const char* imagePath, unsigned int width, unsigned int height, float minHeight, float maxHeight)
{
    GP_ASSERT(imagePath);
    GP_ASSERT(maxHeight >= minHeight);

    // Validate input parameters
    size_t pathLength = strlen(imagePath);
    if (pathLength <= 4)
    {
        GP_WARN("Unrecognized file extension for heightfield image: %s.", imagePath);
        return NULL;
    }

    float heightScale = maxHeight - minHeight;

    HeightField* heightfield = NULL;

    // Load height data from image
    const char* ext = imagePath + (pathLength - 4);
    if (ext[0] == '.' && toupper(ext[1]) == 'P' && toupper(ext[2]) == 'N' && toupper(ext[3]) == 'G')
    {
        // Normal image
        Image* image = Image::create(imagePath);
        if (!image)
            return NULL;

        unsigned int pixelSize = 0;
        switch (image->getFormat())
        {
            case Image::RGB:
                pixelSize = 3;
                break;
            case Image::RGBA:
                pixelSize = 4;
                break;
            default:
                SAFE_RELEASE(image);
                GP_WARN("Unsupported pixel format for heightfield image: %s.", imagePath);
                return NULL;
        }

        // Calculate the heights for each pixel.
        heightfield = HeightField::create(image->getWidth(), image->getHeight());
        float* heights = heightfield->getArray();
        unsigned char* data = image->getData();
        int idx;
        for (int y = image->getHeight()-1, i = 0; y >= 0; --y)
        {
            for (unsigned int x = 0, w = image->getWidth(); x < w; ++x)
            {
                idx = (y*w + x) * pixelSize;
                heights[i++] = minHeight + normalizedHeightPacked(data[idx], data[idx + 1], data[idx + 2]) * heightScale;
            }
        }

        SAFE_RELEASE(image);
    }
    else if (ext[0] == '.' && toupper(ext[1]) == 'R' && toupper(ext[2]) == 'A' && toupper(ext[3]) == 'W')
    {
        // RAW image (headerless)
        if (width < 2 || height < 2 || maxHeight < 0)
        {
            GP_WARN("Invalid 'width', 'height' or 'maxHeight' parameter for RAW heightfield image: %s.", imagePath);
            return NULL;
        }

        // Load raw bytes
        int fileSize = 0;
        unsigned char* bytes = (unsigned char*)FileSystem::readAll(imagePath, &fileSize);
        if (bytes == NULL)
        {
            GP_WARN("Falied to read bytes from RAW heightfield image: %s.", imagePath);
            return NULL;
        }

        // Determine if the RAW file is 8-bit or 16-bit based on file size.
        int bits = (fileSize / (width * height)) * 8;
        if (bits != 8 && bits != 16)
        {
            GP_WARN("Invalid RAW file - must be 8-bit or 16-bit, but found neither: %s.", imagePath);
            SAFE_DELETE_ARRAY(bytes);
            return NULL;
        }

        heightfield = HeightField::create(width, height);
        float* heights = heightfield->getArray();

        // RAW files have an origin of bottom left, whereas our height array needs an origin of
        // top left, so we need to flip the Y as we write height values out.
        if (bits == 16)
        {
            // 16-bit (0-65535)
            int idx;
            for (int y = height-1, i = 0; y >= 0; --y)
            {
                for (unsigned int x = 0; x < width; ++x, ++i)
                {
                    idx = (y * width + x) << 1;
                    heights[i] = minHeight + ((bytes[idx] | (int)bytes[idx+1] << 8) / 65535.0f) * heightScale;
                }
            }
        }
        else
        {
            // 8-bit (0-255)
            for (int y = height-1, i = 0; y >= 0; --y)
            {
                for (unsigned int x = 0; x < width; ++x, ++i)
                {
                    heights[i] = minHeight + (bytes[y * width + x] / 255.0f) * heightScale;
                }
            }
        }

        SAFE_DELETE_ARRAY(bytes);
    }
    else
    {
        GP_WARN("Unsupported heightfield image format: %s.", imagePath);
    }

    return heightfield;
}

float* Terrain::HeightField::getArray() const
{
    return _array;
}

unsigned int Terrain::HeightField::getColumnCount() const
{
    return _cols;
}

unsigned int Terrain::HeightField::getRowCount() const
{
    return _rows;
}

}
