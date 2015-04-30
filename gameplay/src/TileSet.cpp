#include "Base.h"
#include "TileSet.h"
#include "Matrix.h"
#include "Scene.h"

namespace gameplay
{
  
TileSet::TileSet() : Drawable(),
    _tiles(NULL), _tileWidth(0), _tileHeight(0),
    _rowCount(0), _columnCount(0), _width(0), _height(0),
    _opacity(1.0f), _color(Vector4::one()), _batch(NULL)
{
}

TileSet::~TileSet()
{
    SAFE_DELETE_ARRAY(_tiles);
    SAFE_DELETE(_batch);
}
    
TileSet& TileSet::operator=(const TileSet& set)
{
    return *this;
}
    
TileSet* TileSet::create(const char* imagePath,
                         float tileWidth, float tileHeight,
                         unsigned int rowCount, unsigned int columnCount)
{
    GP_ASSERT(imagePath);
    GP_ASSERT(tileWidth > 0 && tileHeight > 0);
    GP_ASSERT(rowCount > 0 && columnCount > 0);
    
    SpriteBatch* batch = SpriteBatch::create(imagePath);
    batch->getSampler()->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    batch->getSampler()->setFilterMode(Texture::Filter::NEAREST, Texture::Filter::NEAREST);
    batch->getStateBlock()->setDepthWrite(false);
    batch->getStateBlock()->setDepthTest(true);
    
    TileSet* tileset = new TileSet();
    tileset->_batch = batch;
    tileset->_tiles = new Vector2[rowCount * columnCount];
    memset(tileset->_tiles, -1, sizeof(float) * rowCount * columnCount * 2);
    tileset->_tileWidth = tileWidth;
    tileset->_tileHeight = tileHeight;
    tileset->_rowCount = rowCount;
    tileset->_columnCount = columnCount;
    tileset->_width = tileWidth * columnCount;
    tileset->_height = tileHeight * rowCount;
    return tileset;
}
    
TileSet* TileSet::create(Properties* properties)
{
    // Check if the Properties is valid and has a valid namespace.
    if (!properties || strcmp(properties->getNamespace(), "tileset") != 0)
    {
        GP_ERROR("Properties object must be non-null and have namespace equal to 'tileset'.");
        return NULL;
    }

    // Get image path.
    const char* imagePath = properties->getString("path");
    if (imagePath == NULL || strlen(imagePath) == 0)
    {
        GP_ERROR("TileSet is missing required image file path.");
        return NULL;
    }

    // Get set size
    int rows = properties->getInt("rows");
    if (rows <= 0)
    {
        GP_ERROR("TileSet row count must be greater then zero.");
        return NULL;
    }
    int columns = properties->getInt("columns");
    if (columns <= 0)
    {
        GP_ERROR("TileSet column count must be greater then zero.");
        return NULL;
    }

    // Get tile size
    float tileWidth = properties->getFloat("tileWidth");
    if (tileWidth <= 0)
    {
        GP_ERROR("TileSet tile width must be greater then zero.");
        return NULL;
    }
    float tileHeight = properties->getFloat("tileHeight");
    if (tileHeight <= 0)
    {
        GP_ERROR("TileSet tile height must be greater then zero.");
        return NULL;
    }

    // Create tile set
    TileSet* set = TileSet::create(imagePath, tileWidth, tileHeight, rows, columns);

    // Get color
    if (properties->exists("color"))
    {
        Vector4 color;
        switch (properties->getType("color"))
        {
        case Properties::VECTOR3:
            color.w = 1.0f;
            properties->getVector3("color", (Vector3*)&color);
            break;
        case Properties::VECTOR4:
            properties->getVector4("color", &color);
            break;
        case Properties::STRING:
        default:
            properties->getColor("color", &color);
            break;
        }
        set->setColor(color);
    }

    // Get opacity
    if (properties->exists("opacity"))
    {
        set->setOpacity(properties->getFloat("opacity"));
    }

    // Get tile sources
    properties->rewind();
    Properties* tileProperties = NULL;
    while ((tileProperties = properties->getNextNamespace()))
    {
        if (strcmp(tileProperties->getNamespace(), "tile") == 0)
        {
            Vector2 cell;
            Vector2 source;
            if (tileProperties->getVector2("cell", &cell) && tileProperties->getVector2("source", &source) &&
                (cell.x >= 0 && cell.y >= 0 && cell.x < set->_columnCount && cell.y < set->_rowCount))
            {
                set->_tiles[(int)cell.y * set->_columnCount + (int)cell.x] = source;
            }
        }
    }

    return set;
}

void TileSet::setTileSource(unsigned int column, unsigned int row, const Vector2& source)
{
    GP_ASSERT(column < _columnCount);
    GP_ASSERT(row < _rowCount);
    
    _tiles[row * _columnCount + column] = source;
}

void TileSet::getTileSource(unsigned int column, unsigned int row, Vector2* source)
{
    GP_ASSERT(column < _columnCount);
    GP_ASSERT(row < _rowCount);
    GP_ASSERT(source);
    
    source->x = _tiles[row * _columnCount + column].x;
    source->y = _tiles[row * _columnCount + column].y;
}

float TileSet::getTileWidth() const
{
    return _tileWidth;
}

float TileSet::getTileHeight() const
{
    return _tileHeight;
}

unsigned int TileSet::getRowCount() const
{
    return _rowCount;
}
    
unsigned int TileSet::getColumnCount() const
{
    return _columnCount;
}

float TileSet::getWidth() const
{
    return _width;
}

float TileSet::getHeight() const
{
    return _height;
}
    
void TileSet::setOpacity(float opacity)
{
    _opacity = opacity;
}

float TileSet::getOpacity() const
{
    return _opacity;
}

void TileSet::setColor(const Vector4& color)
{
    _color = color;
}

const Vector4& TileSet::getColor() const
{
    return _color;
}

unsigned int TileSet::draw(bool wireframe)
{
    // Apply scene camera projection and translation offsets
    Vector3 position = Vector3::zero();
    if (_node && _node->getScene())
    {
        Camera* activeCamera = _node->getScene()->getActiveCamera();
        if (activeCamera)
        {
            Node* cameraNode = _node->getScene()->getActiveCamera()->getNode();
            if (cameraNode)
            {
                // Scene projection
                Matrix projectionMatrix;
                projectionMatrix = _node->getProjectionMatrix();
                _batch->setProjectionMatrix(projectionMatrix);

                position.x -= cameraNode->getTranslationWorld().x;
                position.y -= cameraNode->getTranslationWorld().y;
            }
        }
        
        // Apply node translation offsets
        Vector3 translation = _node->getTranslationWorld();
        position.x += translation.x;
        position.y += translation.y;
        position.z += translation.z;
    }
    
    // Draw each cell in the tile set
    position.y += _tileHeight * (_rowCount - 1);
    float xStart = position.x;
    _batch->start();
    for (unsigned int row = 0; row < _rowCount; row++)
    {
        for (unsigned int col = 0; col < _columnCount; col++)
        {
            Vector2 scale = Vector2(_tileWidth, _tileHeight);
            
            // Negative values are skipped to allow blank tiles
            if (_tiles[row * _columnCount + col].x >= 0 &&
                _tiles[row * _columnCount + col].y >= 0)
            {
                Rectangle source = Rectangle(_tiles[row * _columnCount + col].x,
                                             _tiles[row * _columnCount + col].y, _tileWidth, _tileHeight);
                _batch->draw(position, source, scale, Vector4(_color.x, _color.y, _color.z, _color.w * _opacity),
                             Vector2(0.5f, 0.5f), 0);
            }
            
            position.x += _tileWidth;
        }
        position.x = xStart;
        position.y -= _tileHeight;
    }
    _batch->finish();
    return 1;
}

Drawable* TileSet::clone(NodeCloneContext& context)
{
    TileSet* tilesetClone = new TileSet();

    // Clone properties
    tilesetClone->_tiles = new Vector2[tilesetClone->_rowCount * tilesetClone->_columnCount];
    memset(tilesetClone->_tiles, -1, sizeof(float) * tilesetClone->_rowCount * tilesetClone->_columnCount * 2);
    memcpy(tilesetClone->_tiles, _tiles, sizeof(Vector2) * tilesetClone->_rowCount * tilesetClone->_columnCount);
    tilesetClone->_tileWidth = _tileWidth;
    tilesetClone->_tileHeight = _tileHeight;
    tilesetClone->_rowCount = _rowCount;
    tilesetClone->_columnCount = _columnCount;
    tilesetClone->_width = _tileWidth * _columnCount;
    tilesetClone->_height = _tileHeight * _rowCount;
    tilesetClone->_opacity = _opacity;
    tilesetClone->_color = _color;
    tilesetClone->_batch = _batch;

    return tilesetClone;
}

}
