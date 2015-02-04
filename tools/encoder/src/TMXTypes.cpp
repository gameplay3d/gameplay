#include "TMXTypes.h"

using namespace gameplay;

// TMXMap
TMXMap::TMXMap()
    : _width(0), _height(0),
    _tileWidth(0.0f), _tileHeight(0.0f),
    _tileSets(), _layers()
{
}

TMXMap::~TMXMap()
{
    for (auto it = _layers.begin(); it != _layers.end(); it++)
    {
        delete *it;
    }
}

void TMXMap::setWidth(unsigned int value)
{
    _width = value;
}

void TMXMap::setHeight(unsigned int value)
{
    _height = value;
}

unsigned int TMXMap::getWidth() const
{
    return _width;
}

unsigned int TMXMap::getHeight() const
{
    return _height;
}

void TMXMap::setTileWidth(float value)
{
    _tileWidth = value;
}

void TMXMap::setTileHeight(float value)
{
    _tileHeight = value;
}

float TMXMap::getTileWidth() const
{
    return _tileWidth;
}

float TMXMap::getTileHeight() const
{
    return _tileHeight;
}

bool TMXMap::isValidTileId(unsigned int tileId) const
{
    if (tileId != 0)
    {
        unsigned int tileSet = findTileSet(tileId);
        return tileSet < _tileSets.size();
    }

    return false;
}

unsigned int TMXMap::findTileSet(unsigned int tileId) const
{
    if (tileId == 0)
    {
        return (unsigned int)_tileSets.size();
    }

    for (int i = (int)_tileSets.size() - 1; i >= 0; i--)
    {
        if (_tileSets[i].getFirstGid() > tileId)
        {
            continue;
        }
        return i;
    }

    return (unsigned int)_tileSets.size();
}

void TMXMap::addTileSet(const TMXTileSet& tileset)
{
    _tileSets.push_back(tileset);
}

void TMXMap::addLayer(const TMXBaseLayer* layer)
{
   _layers.push_back(layer);
}

unsigned int TMXMap::getTileSetCount() const
{
    return (unsigned int)_tileSets.size();
}

unsigned int TMXMap::getLayerCount() const
{
    return (unsigned int)_layers.size();
}

TMXTileSet& TMXMap::getTileSet(unsigned int index)
{
    return _tileSets[index];
}

const TMXTileSet& TMXMap::getTileSet(unsigned int index) const
{
    return _tileSets[index];
}

const TMXBaseLayer* TMXMap::getLayer(unsigned int index) const
{
    return _layers[index];
}

// TMXTileSet
TMXTileSet::TMXTileSet()
    : _path(""),
    _imgWidth(0), _imgHeight(0), _horzTileCount(0), _vertTileCount(0),
    _firstGid(0), _maxTileWidth(0), _maxTileHeight(0),
    _offset(), _spacing(0), _margin(0)
{
}

void TMXTileSet::setImagePath(const std::string& path)
{
    _path = path;
}

const std::string& TMXTileSet::getImagePath() const
{
    return _path;
}

void TMXTileSet::setFirstGid(unsigned int gid)
{
    _firstGid = gid;
}

unsigned int TMXTileSet::getFirstGid() const
{
    return _firstGid;
}

void TMXTileSet::setMaxTileWidth(unsigned int value, bool adjustTiles)
{
    _maxTileWidth = value;
    if (adjustTiles && _maxTileWidth != 0)
    {
        setImageWidth(_imgWidth);
    }
}

unsigned int TMXTileSet::getMaxTileWidth() const
{
    return _maxTileWidth;
}

void TMXTileSet::setMaxTileHeight(unsigned int value, bool adjustTiles)
{
    _maxTileHeight = value;
    if (adjustTiles && _maxTileHeight != 0)
    {
        setImageWidth(_imgHeight);
    }
}

unsigned int TMXTileSet::getMaxTileHeight() const
{
    return _maxTileHeight;
}

void TMXTileSet::setSpacing(unsigned int value, bool adjustTiles)
{
    _spacing = value;
    if (adjustTiles)
    {
        if (_maxTileWidth != 0)
        {
            setImageWidth(_imgWidth);
        }
        if (_maxTileHeight != 0)
        {
            setImageWidth(_imgHeight);
        }
    }
}

unsigned int TMXTileSet::getSpacing() const
{
    return _spacing;
}

void TMXTileSet::setMargin(unsigned int value, bool adjustTiles)
{
    _margin = value;
    if (adjustTiles)
    {
        if (_maxTileWidth != 0)
        {
            setImageWidth(_imgWidth);
        }
        if (_maxTileHeight != 0)
        {
            setImageWidth(_imgHeight);
        }
    }
}

unsigned int TMXTileSet::getMargin() const
{
    return _margin;
}

void TMXTileSet::setImageWidth(unsigned int value)
{
    _imgWidth = value;
    _horzTileCount = (value - (_margin * 2) + (_spacing ? _spacing : 0)) / (_maxTileWidth + _spacing);
}

void TMXTileSet::setImageHeight(unsigned int value)
{
    _imgHeight = value;
    _vertTileCount = (value - (_margin * 2) + (_spacing ? _spacing : 0)) / (_maxTileHeight + _spacing);
}

unsigned int TMXTileSet::getHorizontalTileCount() const
{
    return _horzTileCount;
}

unsigned int TMXTileSet::getVerticalTileCount() const
{
    return _vertTileCount;
}

void TMXTileSet::setOffset(const Vector2& off)
{
    _offset = off;
}

const Vector2& TMXTileSet::getOffset() const
{
    return _offset;
}

unsigned int TMXTileSet::calculateImageDimension(unsigned int tileCount, unsigned int tileSize, unsigned int spacing, unsigned int margin)
{
    return tileCount * (tileSize + spacing) + (margin * 2) - spacing;
}

Vector2 TMXTileSet::calculateTileOrigin(const Vector2& pos, const Vector2& tileSize, unsigned int spacing, unsigned int margin)
{
    float xpos = (tileSize.x + spacing) * pos.x;
    float ypos = (tileSize.y + spacing) * pos.y;

    return Vector2(xpos + margin, ypos + margin);
}

// TMXBaseLayer
TMXBaseLayer::TMXBaseLayer(TMXLayerType type)
    : _name(""), _type(type), _opacity(1.0f), _visible(true), _properties()
{
}

TMXBaseLayer::~TMXBaseLayer()
{
}

TMXLayerType TMXBaseLayer::getType() const
{
    return _type;
}

void TMXBaseLayer::setName(const std::string& value)
{
    _name = value;
}

const std::string& TMXBaseLayer::getName() const
{
    return _name;
}

void TMXBaseLayer::setOpacity(float value)
{
    _opacity = value;
}

float TMXBaseLayer::getOpacity() const
{
    return _opacity;
}

void TMXBaseLayer::setVisible(bool value)
{
    _visible = value;
}

bool TMXBaseLayer::getVisible() const
{
    return _visible;
}

TMXProperties& TMXBaseLayer::getProperties()
{
    return _properties;
}

const TMXProperties& TMXBaseLayer::getProperties() const
{
    return _properties;
}

// TMXLayer
#define FLIPPED_HORIZONTALLY_FLAG 0x80000000
#define FLIPPED_VERTICALLY_FLAG 0x40000000
#define FLIPPED_DIAGONALLY_FLAG 0x20000000

TMXLayer::TMXLayer()
    : TMXBaseLayer(TMXLayerType::NormalLayer),
    _width(0), _height(0),
    _tiles()
{
}

TMXLayer::~TMXLayer()
{
}

void TMXLayer::setWidth(unsigned int value)
{
    _width = value;
}

unsigned int TMXLayer::getWidth() const
{
    return _width;
}

void TMXLayer::setHeight(unsigned int value)
{
    _height = value;
}

unsigned int TMXLayer::getHeight() const
{
    return _height;
}

void TMXLayer::setupTiles()
{
    if (_tiles.size() != (_width * _height))
    {
        _tiles.resize(_width * _height);
    }
}

void TMXLayer::setTile(unsigned int x, unsigned int y, unsigned int gid)
{
    bool flipHorz = (gid & FLIPPED_HORIZONTALLY_FLAG) != 0;
    bool flipVert = (gid & FLIPPED_VERTICALLY_FLAG) != 0;
    bool flipDiag = (gid & FLIPPED_DIAGONALLY_FLAG) != 0;
    unsigned int flaglessGid = gid & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
    _tiles[x + y * _width] = { flaglessGid, flipHorz, flipVert, flipDiag };
}

const TMXLayer::layer_tile& TMXLayer::getTileStruct(unsigned int x, unsigned int y) const
{
    return _tiles[x + y * _width];
}

unsigned int TMXLayer::getTile(unsigned int x, unsigned int y) const
{
    return getTileStruct(x, y).gid;
}

bool TMXLayer::isEmptyTile(unsigned int x, unsigned int y) const
{
    return getTileStruct(x, y).gid == 0;
}

Vector2 TMXLayer::getTileStart(unsigned int x, unsigned int y, const TMXMap& map, unsigned int resultOnlyForTileset) const
{
    // Get the tile
    unsigned int gid = getTileStruct(x, y).gid;
    if (gid == 0)
    {
        return Vector2(-1, -1);
    }

    // Get the tileset for the tile
    unsigned int tileset_id = map.findTileSet(gid);
    if (tileset_id == map.getTileSetCount() || 
        (resultOnlyForTileset != TMX_INVALID_ID && tileset_id != resultOnlyForTileset))
    {
        return Vector2(-1, -1);
    }
    const TMXTileSet& tileset = map.getTileSet(tileset_id);
    if (tileset.getHorizontalTileCount() == 0)
    {
        return Vector2(-1, -1);
    }

    // Calculate the tile position
    unsigned int horzTileCount = tileset.getHorizontalTileCount();
    unsigned int adjusted_gid = gid - tileset.getFirstGid();

    return TMXTileSet::calculateTileOrigin(Vector2(static_cast<float>(adjusted_gid % horzTileCount), static_cast<float>(adjusted_gid / horzTileCount)),
        Vector2(static_cast<float>(tileset.getMaxTileWidth()), static_cast<float>(tileset.getMaxTileHeight())),
        tileset.getSpacing(), 
        tileset.getMargin());
}

bool TMXLayer::hasTiles() const
{
    size_t tile_size = _tiles.size();
    for (unsigned int i = 0; i < tile_size; i++)
    {
        if (_tiles[i].gid != 0)
        {
            return true;
        }
    }
    return false;
}

std::set<unsigned int> TMXLayer::getTilesetsUsed(const TMXMap& map) const
{
    std::set<unsigned int> tilesets;

    unsigned int tileset_size = map.getTileSetCount();
    size_t tile_size = _tiles.size();
    for (unsigned int i = 0; i < tile_size; i++)
    {
        unsigned int gid = _tiles[i].gid;
        if (gid == 0)
        {
            // Empty tile
            continue;
        }
        unsigned int tileset = map.findTileSet(gid);
        if (tileset == tileset_size)
        {
            // Could not find tileset
            continue;
        }
        tilesets.insert(tileset);
        if (tilesets.size() == tileset_size)
        {
            // Don't need to continue checking, we have every possible tileset
            break;
        }
    }

    return tilesets;
}

// TMXImageLayer
TMXImageLayer::TMXImageLayer()
    : TMXBaseLayer(TMXLayerType::ImageLayer),
    _pos(), _path("")
{
}

TMXImageLayer::~TMXImageLayer()
{
}

void TMXImageLayer::setX(int value)
{
    _pos.x = static_cast<float>(value);
}

int TMXImageLayer::getX() const
{
    return static_cast<int>(_pos.x);
}

void TMXImageLayer::setY(int value)
{
    _pos.y = static_cast<float>(value);
}

int TMXImageLayer::getY() const
{
    return static_cast<int>(_pos.y);
}

const Vector2& TMXImageLayer::getPosition() const
{
    return _pos;
}

void TMXImageLayer::setImagePath(const std::string& path)
{
    _path = path;
}

const std::string& TMXImageLayer::getImagePath() const
{
    return _path;
}
