#ifndef TMXTYPES_H_
#define TMXTYPES_H_

#include <vector>
#include <set>
#include <map>
#include <string>

#include "Vector2.h"

namespace gameplay
{

typedef std::map<std::string, std::string> TMXProperties;

/**
 * Represents the tiles that make up a map.
 */
class TMXTileSet
{
public:
    /**
     * Constructor.
     */
    TMXTileSet();

    void setImagePath(const std::string& path);
    const std::string& getImagePath() const;

    void setFirstGid(unsigned int gid);
    unsigned int getFirstGid() const;

    void setMaxTileWidth(unsigned int value, bool adjustTiles = true);
    unsigned int getMaxTileWidth() const;
    void setMaxTileHeight(unsigned int value, bool adjustTiles = true);
    unsigned int getMaxTileHeight() const;

    void setSpacing(unsigned int value, bool adjustTiles = true);
    unsigned int getSpacing() const;
    void setMargin(unsigned int value, bool adjustTiles = true);
    unsigned int getMargin() const;

    // This will calculate the number of tiles based on max tile size, spacing, and margin.
    // If any of those change, this needs to be recalled.
    void setImageWidth(unsigned int value);
    void setImageHeight(unsigned int value);
    unsigned int getHorizontalTileCount() const;
    unsigned int getVerticalTileCount() const;

    void setOffset(const Vector2& offset);
    const Vector2& getOffset() const;

    static unsigned int calculateImageDimension(unsigned int tileCount, unsigned int tileSize, unsigned int spacing = 0, unsigned int margin = 0);
    static Vector2 calculateTileOrigin(const Vector2& pos, const Vector2& tileSize, unsigned int spacing = 0, unsigned int margin = 0);

private:
    std::string _path;

    unsigned int _imgWidth;
    unsigned int _imgHeight;
    unsigned int _horzTileCount;
    unsigned int _vertTileCount;

    unsigned int _firstGid;
    unsigned int _maxTileWidth;
    unsigned int _maxTileHeight;
    Vector2 _offset;
    unsigned int _spacing;
    unsigned int _margin;
};

/**
 * Types of layers
 */
enum TMXLayerType
{
    NormalLayer,
    ImageLayer,
};

/**
 * Represents the base type of layer on a map.
 */
class TMXBaseLayer
{
protected:
    /**
     * Constructor.
     */
    TMXBaseLayer(TMXLayerType type);

public:
    /**
     * Destructor.
     */
    virtual ~TMXBaseLayer();

    TMXLayerType getType() const;

    void setName(const std::string& value);
    const std::string& getName() const;

    void setOpacity(float value);
    float getOpacity() const;

    void setVisible(bool value);
    bool getVisible() const;

    TMXProperties& getProperties();
    const TMXProperties& getProperties() const;

private:
    std::string _name;
    TMXLayerType _type;
    float _opacity;
    bool _visible;
    TMXProperties _properties;
};

#define TMX_INVALID_ID 0xFFFFFFFFu

class TMXMap;
/**
 * Represents a single layer on a map.
 */
class TMXLayer : public TMXBaseLayer
{
public:
    /**
     * Constructor.
     */
    TMXLayer();

    /**
     * Destructor.
     */
    virtual ~TMXLayer();

    void setWidth(unsigned int value);
    unsigned int getWidth() const;
    void setHeight(unsigned int value);
    unsigned int getHeight() const;

    void setupTiles();
    void setTile(unsigned int x, unsigned int y, unsigned int gid);
    unsigned int getTile(unsigned int x, unsigned int y) const;
    bool isEmptyTile(unsigned int x, unsigned int y) const;
    Vector2 getTileStart(unsigned int x, unsigned int y, const TMXMap& map, unsigned int resultOnlyForTileset = TMX_INVALID_ID) const;

    bool hasTiles() const;
    std::set<unsigned int> getTilesetsUsed(const TMXMap& map) const;

private:
    struct layer_tile
    {
        unsigned int gid;
        bool horz_flip;
        bool vert_flip;
        bool diag_flip;
    };

    const layer_tile& getTileStruct(unsigned int x, unsigned int y) const;

    unsigned int _width;
    unsigned int _height;

    std::vector<layer_tile> _tiles;
};

/**
 * Represents a image layer on a map.
 */
class TMXImageLayer : public TMXBaseLayer
{
public:
    /**
     * Constructor.
     */
    TMXImageLayer();

    /**
     * Destructor.
     */
    virtual ~TMXImageLayer();

    void setX(int value);
    int getX() const;
    void setY(int value);
    int getY() const;

    const Vector2& getPosition() const;

    void setImagePath(const std::string& path);
    const std::string& getImagePath() const;

private:
    Vector2 _pos;
    std::string _path;
};

/**
 * Represents an entire map of tiles.
 */
class TMXMap
{
public:
    /**
     * Constructor.
     */
    TMXMap();

    /**
     * Destructor.
     */
    ~TMXMap();

    void setWidth(unsigned int value);
    void setHeight(unsigned int value);
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setTileWidth(float value);
    void setTileHeight(float value);
    float getTileWidth() const;
    float getTileHeight() const;

    bool isValidTileId(unsigned int tileId) const;
    unsigned int findTileSet(unsigned int tileId) const;

    void addTileSet(const TMXTileSet& tileset);
    void addLayer(const TMXBaseLayer* layer);

    unsigned int getTileSetCount() const;
    unsigned int getLayerCount() const;

    TMXTileSet& getTileSet(unsigned int index);
    const TMXTileSet& getTileSet(unsigned int index) const;
    const TMXBaseLayer* getLayer(unsigned int index) const;

private:
    unsigned int _width;
    unsigned int _height;
    float _tileWidth;
    float _tileHeight;

    std::vector<TMXTileSet> _tileSets;
    std::vector<const TMXBaseLayer*> _layers;
};

}

#endif
