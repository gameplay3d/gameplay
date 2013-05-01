#include "Base.h"
#include "TerrainPatch.h"
#include "Terrain.h"
#include "MeshPart.h"
#include "Scene.h"
#include "Game.h"

// Default terrain shaders
#define TERRAIN_VSH "res/shaders/terrain.vert"
#define TERRAIN_FSH "res/shaders/terrain.frag"

namespace gameplay
{

/**
 * @script{ignore}
 */
float calculateHeight(float* heights, unsigned int width, unsigned int height, unsigned int x, unsigned int z);

/**
 * @script{ignore}
 */
template <class T> T clamp(T value, T min, T max) { return value < min ? min : (value > max ? max : value); }

TerrainPatch::TerrainPatch() :
    _terrain(NULL), _row(0), _column(0), _materialDirty(true)
{
}

TerrainPatch::~TerrainPatch()
{
    for (size_t i = 0, count = _levels.size(); i < count; ++i)
    {
        Level* level = _levels[i];

        SAFE_RELEASE(level->model);
        SAFE_DELETE(level);
    }

    while (_layers.size() > 0)
    {
        deleteLayer(*_layers.begin());
    }
}

TerrainPatch* TerrainPatch::create(Terrain* terrain,
    unsigned int row, unsigned int column,
    float* heights, unsigned int width, unsigned int height,
    unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
    float xOffset, float zOffset,
    unsigned int maxStep, float verticalSkirtSize)
{
    // Create patch
    TerrainPatch* patch = new TerrainPatch();
    patch->_terrain = terrain;
    patch->_row = row;
    patch->_column = column;

    // Add patch lods
    for (unsigned int step = 1; step <= maxStep; step *= 2)
    {
        patch->addLOD(heights, width, height, x1, z1, x2, z2, xOffset, zOffset, step, verticalSkirtSize);
    }

    // Set our bounding box using the base LOD mesh
    BoundingBox& bounds = patch->_boundingBox;
    bounds.set(patch->_levels[0]->model->getMesh()->getBoundingBox());

    // Apply the terrain's local scale to our bounds
    const Vector3& localScale = terrain->_localScale;
    if (!localScale.isOne())
    {
        bounds.min.set(bounds.min.x * localScale.x, bounds.min.y * localScale.y, bounds.min.z * localScale.z);
        bounds.max.set(bounds.max.x * localScale.x, bounds.max.y * localScale.y, bounds.max.z * localScale.z);
    }

    return patch;
}

void TerrainPatch::addLOD(float* heights, unsigned int width, unsigned int height,
    unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2,
    float xOffset, float zOffset,
    unsigned int step, float verticalSkirtSize)
{
    // Allocate vertex data for this patch
    unsigned int patchWidth;
    unsigned int patchHeight;
    if (step == 1)
    {
        patchWidth = (x2 - x1) + 1;
        patchHeight = (z2 - z1) + 1;
    }
    else
    {
        patchWidth = (x2 - x1) / step + ((x2 - x1) %step == 0 ? 0 : 1) + 1;
        patchHeight = (z2 - z1) / step + ((z2 - z1) % step == 0 ? 0 : 1) + 1;
    }

    if (patchWidth < 2 || patchHeight < 2)
        return; // ignore this level, not enough geometry

    if (verticalSkirtSize > 0.0f)
    {
        patchWidth += 2;
        patchHeight += 2;
    }

    unsigned int vertexCount = patchHeight * patchWidth;
    unsigned int vertexElements = _terrain->_normalMap ? 5 : 8; //<x,y,z>[i,j,k]<u,v>
    float* vertices = new float[vertexCount * vertexElements];
    unsigned int index = 0;
    Vector3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    bool zskirt = verticalSkirtSize > 0 ? true : false;
    for (unsigned int z = z1; ; )
    {
        bool xskirt = verticalSkirtSize > 0 ? true : false;
        for (unsigned int x = x1; ; )
        {
            GP_ASSERT(index < vertexCount);

            float* v = vertices + (index * vertexElements);
            index++;

            // Compute position
            v[0] = x + xOffset;
            v[1] = calculateHeight(heights, width, height, x, z);
            if (xskirt || zskirt)
                v[1] -= verticalSkirtSize;
            v[2] = z + zOffset;

            // Update bounding box min/max (don't include vertical skirt vertices in bounding box)
            if (!(xskirt || zskirt))
            {
                if (v[0] < min.x)
                    min.x = v[0];
                if (v[1] < min.y)
                    min.y = v[1];
                if (v[2] < min.z)
                    min.z = v[2];
                if (v[0] > max.x)
                    max.x = v[0];
                if (v[1] > max.y)
                    max.y = v[1];
                if (v[2] > max.z)
                    max.z = v[2];
            }
            v += 3;

            // Compute normal
            if (!_terrain->_normalMap)
            {
                Vector3 p(x, calculateHeight(heights, width, height, x, z), z);
                Vector3 w(Vector3(x>=step ? x-step : x, calculateHeight(heights, width, height, x>=step ? x-step : x, z), z), p);
                Vector3 e(Vector3(x<width-step ? x+step : x, calculateHeight(heights, width, height, x<width-step ? x+step : x, z), z), p);
                Vector3 s(Vector3(x, calculateHeight(heights, width, height, x, z>=step ? z-step : z), z>=step ? z-step : z), p);
                Vector3 n(Vector3(x, calculateHeight(heights, width, height, x, z<height-step ? z+step : z), z<height-step ? z+step : z), p);
                Vector3 normals[4];
                Vector3::cross(n, w, &normals[0]);
                Vector3::cross(w, s, &normals[1]);
                Vector3::cross(e, n, &normals[2]);
                Vector3::cross(s, e, &normals[3]);
                Vector3 normal = -(normals[0] + normals[1] + normals[2] + normals[3]);
                normal.normalize();
                v[0] = normal.x;
                v[1] = normal.y;
                v[2] = normal.z;
                v += 3;
            }

            // Compute texture coord
            v[0] = (float)x / width;
            v[1] = 1.0f - (float)z / height;
            if (xskirt)
            {
                float offset = verticalSkirtSize / width;
                v[0] = x == x1 ? v[0]-offset : v[0]+offset;
            }
            else if (zskirt)
            {
                float offset = verticalSkirtSize / height;
                v[1] = z == z1 ? v[1]-offset : v[1]+offset;
            }

            if (x == x2)
            {
                if ((verticalSkirtSize == 0) || xskirt)
                    break;
                else
                    xskirt = true;
            }
            else if (xskirt)
            {
                xskirt = false;
            }
            else
            {
                x = std::min(x + step, x2);
            }
        }

        if (z == z2)
        {
            if ((verticalSkirtSize == 0) || zskirt)
                break;
            else
                zskirt = true;
        }
        else if (zskirt)
        {
            zskirt = false;
        }
        else
        {
            z = std::min(z + step, z2);
        }
    }
    GP_ASSERT(index == vertexCount);

    Vector3 center(min + ((max - min) * 0.5f));

    // Create mesh
    VertexFormat::Element elements[3];
    elements[0] = VertexFormat::Element(VertexFormat::POSITION, 3);
    if (_terrain->_normalMap)
    {
        elements[1] = VertexFormat::Element(VertexFormat::TEXCOORD0, 2);
    }
    else
    {
        elements[1] = VertexFormat::Element(VertexFormat::NORMAL, 3);
        elements[2] = VertexFormat::Element(VertexFormat::TEXCOORD0, 2);
    }
    VertexFormat format(elements, _terrain->_normalMap ? 2 : 3);
    Mesh* mesh = Mesh::createMesh(format, vertexCount);
    mesh->setVertexData(vertices);
    mesh->setBoundingBox(BoundingBox(min, max));
    mesh->setBoundingSphere(BoundingSphere(center, center.distance(max)));

    // Add mesh part for indices
    unsigned int indexCount =
        (patchWidth * 2) *      // # indices per row of tris
        (patchHeight - 1) +     // # rows of tris
        (patchHeight-2) * 2;    // # degenerate tris

    // Support a maximum number of indices of USHRT_MAX. Any more indices we will require breaking up the
    // terrain into smaller patches.
    if (indexCount > USHRT_MAX)
    {
        GP_WARN("Index count of %d for terrain patch exceeds the limit of 65535. Please specifiy a smaller patch size.", indexCount);
        GP_ASSERT(indexCount <= USHRT_MAX);
    }

    MeshPart* part = mesh->addPart(Mesh::TRIANGLE_STRIP, Mesh::INDEX16, indexCount);
    unsigned short* indices = new unsigned short[indexCount];
    index = 0;
    for (unsigned int z = 0; z < patchHeight-1; ++z)
    {
        unsigned int i1 = z * patchWidth;
        unsigned int i2 = (z+1) * patchWidth;

        // Move left to right for even rows and right to left for odd rows.
        // Note that this results in two degenerate triangles between rows
        // for stitching purposes, but actually does not require any extra
        // indices to achieve this.
        if (z % 2 == 0)
        {
            if (z > 0)
            {
                // Add degenerate indices to connect strips
                indices[index] = indices[index-1];
                ++index;
                indices[index++] = i1;
            }

            // Add row strip
            for (unsigned int x = 0; x < patchWidth; ++x)
            {
                indices[index++] = i1 + x;
                indices[index++] = i2 + x;
            }
        }
        else
        {
            // Add degenerate indices to connect strips
            if (z > 0)
            {
                indices[index] = indices[index-1];
                ++index;
                indices[index++] = i2 + ((int)patchWidth-1);
            }

            // Add row strip
            for (int x = (int)patchWidth-1; x >= 0; --x)
            {
                indices[index++] = i2 + x;
                indices[index++] = i1 + x;
            }
        }
    }
    GP_ASSERT(index == indexCount);
    part->setIndexData(indices, 0, indexCount);

    SAFE_DELETE_ARRAY(vertices);
    SAFE_DELETE_ARRAY(indices);

    // Create model
    Model* model = Model::create(mesh);
    mesh->release();

    // Add this level
    Level* level = new Level();
    level->model = model;
    _levels.push_back(level);
}

void TerrainPatch::deleteLayer(Layer* layer)
{
    // Release layer samplers
    if (layer->textureIndex != -1)
    {
        if (_samplers[layer->textureIndex]->getRefCount() == 1)
        {
            SAFE_RELEASE(_samplers[layer->textureIndex]);
        }
        else
        {
            _samplers[layer->textureIndex]->release();
        }
    }

    if (layer->blendIndex != -1)
    {
        if (_samplers[layer->blendIndex]->getRefCount() == 1)
        {
            SAFE_RELEASE(_samplers[layer->blendIndex]);
        }
        else
        {
            _samplers[layer->blendIndex]->release();
        }
    }

    _layers.erase(layer);
    SAFE_DELETE(layer);
}

int TerrainPatch::addSampler(const char* path)
{
    // TODO: Support shared samplers stored in Terrain class for layers that span all patches
    // on the terrain (row == col == -1).

    // Load the texture. If this texture is already loaded, it will return
    // a pointer to the same one, with its ref count incremented.
    Texture* texture = Texture::create(path, true);
    if (!texture)
        return -1;

    int firstAvailableIndex = -1;
    for (size_t i = 0, count = _samplers.size(); i < count; ++i)
    {
        Texture::Sampler* sampler = _samplers[i];

        if (sampler == NULL && firstAvailableIndex == -1)
        {
            firstAvailableIndex = (int)i;
        }
        else if (sampler->getTexture() == texture)
        {
            // A sampler was already added for this texture.
            // Increase the ref count for the sampler to indicate that a new
            // layer will be referencing it.
            texture->release();
            sampler->addRef();
            return (int)i;
        }
    }

    // Add a new sampler to the list
    Texture::Sampler* sampler = Texture::Sampler::create(texture);
    texture->release();
    sampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
    sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    if (firstAvailableIndex != -1)
    {
        _samplers[firstAvailableIndex] = sampler;
        return firstAvailableIndex;
    }

    _samplers.push_back(sampler);
    return (int)(_samplers.size()-1);
}

bool TerrainPatch::setLayer(int index, const char* texturePath, const Vector2& textureRepeat, const char* blendPath, int blendChannel)
{
    // If there is an existing layer at this index, delete it
    for (std::set<Layer*, LayerCompare>::iterator itr = _layers.begin(); itr != _layers.end(); ++itr)
    {
        Layer* layer = *itr;
        if (layer->index == index)
        {
            deleteLayer(layer);
            break;
        }
    }

    // Load texture sampler
    int textureIndex = addSampler(texturePath);
    if (textureIndex == -1)
        return false;

    // Load blend sampler
    int blendIndex = -1;
    if (blendPath)
    {
        blendIndex = addSampler(blendPath);
    }

    // Create the layer
    Layer* layer = new Layer();
    layer->index = index;
    layer->textureIndex = textureIndex;
    layer->textureRepeat = textureRepeat;
    layer->blendIndex = blendIndex;
    layer->blendChannel = blendChannel;

    _layers.insert(layer);

    _materialDirty = true;

    return true;
}

bool TerrainPatch::updateMaterial()
{
    if (!_materialDirty)
        return true;

    _materialDirty = false;

    for (size_t i = 0, count = _levels.size(); i < count; ++i)
    {
        // Build preprocessor string to pass to shader.
        // NOTE: I make heavy use of preprocessor definitions, rather than passing in arrays and doing
        // non-constant array access in the shader. This is due to the fact that non-constant array access
        // in GLES is very slow on some GLES 2.x hardware.
        std::ostringstream defines;
        defines << "LAYER_COUNT " << _layers.size();
        defines << ";SAMPLER_COUNT " << _samplers.size();
        if (_terrain->isFlagSet(Terrain::DEBUG_PATCHES))
            defines << ";DEBUG_PATCHES";
        if (_terrain->_normalMap)
            defines << ";NORMAL_MAP";

        // Append texture and blend index constants to preprocessor definition.
        // We need to do this since older versions of GLSL only allow sampler arrays
        // to be indexed using constant expressions (otherwise we could simply pass an
        // array of indices to use for sampler lookup).
        //
        // Rebuild layer lists while we're at it.
        //
        int layerIndex = 0;
        for (std::set<Layer*, LayerCompare>::iterator itr = _layers.begin(); itr != _layers.end(); ++itr, ++layerIndex)
        {
            Layer* layer = *itr;

            defines << ";TEXTURE_INDEX_" << layerIndex << " " << layer->textureIndex;
            defines << ";TEXTURE_REPEAT_" << layerIndex << " vec2(" << layer->textureRepeat.x << "," << layer->textureRepeat.y << ")";

            if (layerIndex > 0)
            {
                defines << ";BLEND_INDEX_" << layerIndex << " " << layer->blendIndex;
                defines << ";BLEND_CHANNEL_" << layerIndex << " " << layer->blendChannel;
            }
        }

        Material* material = Material::create(TERRAIN_VSH, TERRAIN_FSH, defines.str().c_str());
        if (!material)
            return false;
        material->getStateBlock()->setCullFace(true);
        material->getStateBlock()->setDepthTest(true);

        // Set material parameter bindings
        material->getParameter("u_worldViewProjectionMatrix")->bindValue(_terrain, &Terrain::getWorldViewProjectionMatrix);
        if (_terrain->_normalMap)
            material->getParameter("u_normalMap")->setValue(_terrain->_normalMap);
        else
            material->getParameter("u_normalMatrix")->bindValue(_terrain, &Terrain::getNormalMatrix);
        material->getParameter("u_ambientColor")->bindValue(this, &TerrainPatch::getAmbientColor);
        material->getParameter("u_lightColor")->bindValue(this, &TerrainPatch::getLightColor);
        material->getParameter("u_lightDirection")->bindValue(this, &TerrainPatch::getLightDirection);
        if (_layers.size() > 0)
            material->getParameter("u_samplers")->setValue((const Texture::Sampler**)&_samplers[0], (unsigned int)_samplers.size());

        if (_terrain->isFlagSet(Terrain::DEBUG_PATCHES))
        {
            material->getParameter("u_row")->setValue((float)_row);
            material->getParameter("u_column")->setValue((float)_column);
        }

        // Fire terrain listeners
        for (size_t j = 0, lcount = _terrain->_listeners.size(); j < lcount; ++j)
        {
            _terrain->_listeners[j]->materialUpdated(_terrain, material);
        }

        // Set material on this lod level
        _levels[i]->model->setMaterial(material);

        material->release();
    }

    return true;
}

void TerrainPatch::draw(bool wireframe)
{
    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (!camera)
        return;

    // Get our world-space bounding box
    BoundingBox bounds = getBoundingBox(true);

    // If the box does not intersect the view frustum, cull it
    if (_terrain->isFlagSet(Terrain::FRUSTUM_CULLING) && !camera->getFrustum().intersects(bounds))
        return;

    if (!updateMaterial())
        return;

    // Compute the LOD level from the camera's perspective
    size_t lod = computeLOD(camera, bounds);

    // Draw the model for the current LOD
    _levels[lod]->model->draw(wireframe);
}

bool TerrainPatch::isVisible() const
{
    // If frustum culling is disabled, assume the patch is always visible
    if ((_terrain->_flags & Terrain::FRUSTUM_CULLING) == 0)
        return true;

    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (!camera)
        return false;

    // Does the current camera view frustum intersect our world bounds?
    return camera->getFrustum().intersects(getBoundingBox(true));
}

unsigned int TerrainPatch::getTriangleCount() const
{
    // Patches are made up of a single mesh part using triangle strips
    return _levels[0]->model->getMesh()->getPart(0)->getIndexCount() - 2;
}

unsigned int TerrainPatch::getVisibleTriangleCount() const
{
    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (!camera)
        return 0;

    // Does the current camera intersect this patch at all?
    BoundingBox bounds = getBoundingBox(true);
    if (_terrain->_flags & Terrain::FRUSTUM_CULLING)
    {
        if (!camera->getFrustum().intersects(bounds))
            return 0;
    }

    // Return the triangle count of the LOD level depending on the camera
    size_t lod = computeLOD(camera, bounds);
    return _levels[lod]->model->getMesh()->getPart(0)->getIndexCount() - 2;
}

BoundingBox TerrainPatch::getBoundingBox(bool worldSpace) const
{
    if (!worldSpace)
        return _boundingBox;

    // Apply a world-space transformation to our bounding box
    BoundingBox bounds(_boundingBox);

    // Transform the bounding box by the terrain node's world transform.
    // We don't use Terrain::getWorldMatrix because that returns a matrix
    // that has terrain->_localScale factored in - and our patche's bounding
    // box already has local scale factored in.
    if (_terrain->_node)
        bounds.transform(_terrain->_node->getWorldMatrix());

    return bounds;
}

const Vector3& TerrainPatch::getAmbientColor() const
{
    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    return scene ? scene->getAmbientColor() : Vector3::zero();
}

const Vector3& TerrainPatch::getLightColor() const
{
    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    return scene ? scene->getLightColor() : Vector3::one();
}

const Vector3& TerrainPatch::getLightDirection() const
{
    Scene* scene = _terrain->_node ? _terrain->_node->getScene() : NULL;
    if (!scene)
    {
        static Vector3 down(0, -1, 0);
        return down;
    }

    return scene->getLightDirection();
}

size_t TerrainPatch::computeLOD(Camera* camera, const BoundingBox& worldBounds) const
{
    if (!_terrain->isFlagSet(Terrain::LEVEL_OF_DETAIL) || _levels.size() == 0)
        return 0; // base level

    // Compute LOD to use based on very simple distance metric.
    // TODO: Optimize this.
    Game* game = Game::getInstance();
    Rectangle vp(0, 0, game->getWidth(), game->getHeight());
    Vector3 corners[8];
    Vector2 min(FLT_MAX, FLT_MAX);
    Vector2 max(-FLT_MAX, -FLT_MAX);
    worldBounds.getCorners(corners);
    for (unsigned int i = 0; i < 8; ++i)
    {
        const Vector3& corner = corners[i];
        float x, y;
        camera->project(vp, corners[i], &x, &y);
        if (x < min.x)
            min.x = x;
        if (y < min.y)
            min.y = y;
        if (x > max.x)
            max.x = x;
        if (y > max.y)
            max.y = y;
    }
    float area = (max.x - min.x) * (max.y - min.y);
    float screenArea = game->getWidth() * game->getHeight() / 10.0f;
    float error = screenArea / area;

    // Level LOD based on distance from camera
    size_t maxLod = _levels.size()-1;
    size_t lod = (size_t)error;
    lod = std::max(lod, (size_t)0);
    lod = std::min(lod, maxLod);
    return lod;
}

float calculateHeight(float* heights, unsigned int width, unsigned int height, unsigned int x, unsigned int z)
{
    return heights[z * width + x];
}

TerrainPatch::Layer::Layer() :
    index(0), row(-1), column(-1), textureIndex(-1), blendIndex(-1)
{
}

TerrainPatch::Layer::~Layer()
{
}

TerrainPatch::Level::Level() : model(NULL)
{
}

bool TerrainPatch::LayerCompare::operator() (const Layer* lhs, const Layer* rhs) const
{
    return (lhs->index < rhs->index);
}

}
