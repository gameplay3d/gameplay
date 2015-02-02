#include <string>

#include <zlib.h>

#include "Base64.h"
#include "TMXSceneEncoder.h"

using namespace gameplay;
using namespace tinyxml2;
using std::string;

#ifdef GP_4_SPACE_TABS
#define TAB_STRING(count) string((count) * 4, ' ')
#else
#define TAB_STRING(count) string((count), '\t')
#endif

#define BUFFER_SIZE 256

#ifdef WIN32
#define snprintf(s, n, fmt, ...) sprintf((s), (fmt), __VA_ARGS__)
#endif

TMXSceneEncoder::TMXSceneEncoder() :
    _tabCount(0)
{
}

TMXSceneEncoder::~TMXSceneEncoder()
{
}

void TMXSceneEncoder::write(const string& filepath, const EncoderArguments& arguments)
{
    XMLDocument xmlDoc;
    XMLError err;
    if ((err = xmlDoc.LoadFile(filepath.c_str())) != XML_NO_ERROR)
    {
        LOG(1, "Call to XMLDocument::LoadFile() failed.\n");
        LOG(1, "Error returned: %d\n\n", err);
        return;
    }
    
    // Parse the Tiled map
    string outputFilePath = arguments.getOutputFilePath();
    int pos = outputFilePath.find_last_of('/');
    string outputDirectory = (pos == -1 ? outputFilePath : outputFilePath.substr(0, pos));

    TMXMap map;
    if (!parseTmx(xmlDoc, map, outputDirectory))
    {
        return;
    }

    //XXX arguments.generateTextureGutter()

    // Write the tile map
    string fileName = arguments.getFileName();
    pos = fileName.find_last_of('.');

    writeScene(map, outputFilePath, (pos == -1 ? fileName : fileName.substr(0, pos)));
}

bool TMXSceneEncoder::parseTmx(const XMLDocument& xmlDoc, TMXMap& map, const std::string& outputDirectory) const
{
    auto xmlMap = xmlDoc.FirstChildElement("map");
    if (!xmlMap)
    {
        LOG(1, "Missing root <map> element.\n");
        return false;
    }

    // Read in the map values //XXX should compact this so XML attribute parsing is a little nicer
    unsigned int uiValue;
    int iValue;
    auto attValue = xmlMap->Attribute("width");
    sscanf(attValue, "%u", &uiValue);
    map.setWidth(uiValue);

    attValue = xmlMap->Attribute("height");
    sscanf(attValue, "%u", &uiValue);
    map.setHeight(uiValue);

    float fValue;
    attValue = xmlMap->Attribute("tilewidth");
    sscanf(attValue, "%f", &fValue);
    map.setTileWidth(fValue);

    attValue = xmlMap->Attribute("tileheight");
    sscanf(attValue, "%f", &fValue);
    map.setTileHeight(fValue);

    // Now we load all tilesets
    auto xmlTileSet = xmlMap->FirstChildElement("tileset");
    while (xmlTileSet)
    {
        TMXTileSet tileSet;

        attValue = xmlTileSet->Attribute("firstgid");
        sscanf(attValue, "%u", &uiValue);
        tileSet.setFirstGid(uiValue);

        XMLDocument sourceXmlDoc;
        const XMLElement* xmlTileSetToLoad;
        attValue = xmlTileSet->Attribute("source");
        if (attValue)
        {
            string tsxLocation = outputDirectory + "/" + attValue;
            tsxLocation = EncoderArguments::getRealPath(tsxLocation);

            XMLError err;
            if ((err = sourceXmlDoc.LoadFile(tsxLocation.c_str())) != XML_NO_ERROR)
            {
                LOG(1, "Could not load tileset's source TSX.\n");
                return false;
            }
            xmlTileSetToLoad = sourceXmlDoc.RootElement();
        }
        else
        {
            xmlTileSetToLoad = xmlTileSet;
        }

        // Maximum tile size
        attValue = xmlTileSetToLoad->Attribute("tilewidth");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            tileSet.setMaxTileWidth(uiValue);
        }
        else
        {
            tileSet.setMaxTileWidth(map.getTileWidth());
        }
        attValue = xmlTileSetToLoad->Attribute("tileheight");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            tileSet.setMaxTileHeight(uiValue);
        }
        else
        {
            tileSet.setMaxTileHeight(map.getTileHeight());
        }

        // Spacing and margin
        attValue = xmlTileSetToLoad->Attribute("spacing");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            tileSet.setSpacing(uiValue);
        }
        attValue = xmlTileSetToLoad->Attribute("margin");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            tileSet.setMargin(uiValue);
        }

        // Tile offset
        auto xmlTileOffset = xmlTileSetToLoad->FirstChildElement("tileoffset");
        if (xmlTileOffset)
        {
            Vector2 offset;

            attValue = xmlTileOffset->Attribute("x");
            sscanf(attValue, "%d", &iValue);
            offset.x = iValue;
            attValue = xmlTileOffset->Attribute("y");
            sscanf(attValue, "%d", &iValue);
            offset.y = iValue;

            tileSet.setOffset(offset);
        }

        // Load image source. Don't worry about <data>, trans, or height
        auto xmlTileSetImage = xmlTileSetToLoad->FirstChildElement("image");
        if (!xmlTileSetImage)
        {
            return false;
        }
        tileSet.setImagePath(xmlTileSetImage->Attribute("source"));

        // We only care about image width...
        attValue = xmlTileSetImage->Attribute("width");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            tileSet.setImageWidth(uiValue);
        }
        else
        {
            // Load the image itself to get the width
            string imageLocation = outputDirectory + "/" + tileSet.getImagePath();
            imageLocation = EncoderArguments::getRealPath(imageLocation);

            int pos = imageLocation.find_last_of('.');
            if (imageLocation.substr(pos).compare(".png") != 0)
            {
                LOG(1, "TileSet image must be a PNG. %s\n", imageLocation.c_str());
                return false;
            }

            Image* img = Image::create(imageLocation.c_str());
            if (!img)
            {
                LOG(1, "Could not load TileSet image. %s\n", imageLocation.c_str());
                return false;
            }
            tileSet.setImageWidth(img->getWidth());
            SAFE_DELETE(img);
        }

        //TODO: tiles (specifically, tile animations) if possible. May require marking tiles as special tiles, and spinning them off to Sprites
        //<tile id="relId"><animation><frame tileid="relId" duration="numInMS"></...></...></...>

        // Save the tileset
        map.addTileSet(tileSet);

        xmlTileSet = xmlTileSet->NextSiblingElement("tileset");
    }

    // Load the layers
    auto xmlLayer = xmlMap->FirstChildElement("layer");
    while (xmlLayer)
    {
        auto layer = new TMXLayer();

        layer->setName(xmlLayer->Attribute("name"));

        // Load properties
        attValue = xmlLayer->Attribute("width");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            layer->setWidth(uiValue);
        }
        else
        {
            layer->setWidth(map.getWidth());
        }

        attValue = xmlLayer->Attribute("height");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            layer->setHeight(uiValue);
        }
        else
        {
            layer->setHeight(map.getHeight());
        }

        attValue = xmlLayer->Attribute("opacity");
        if (attValue)
        {
            sscanf(attValue, "%f", &fValue);
            layer->setOpacity(fValue);
        }

        attValue = xmlLayer->Attribute("visible");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            layer->setVisible(uiValue == 1);
        }

        parseProperties(xmlLayer->FirstChildElement("properties"), layer->getProperties());

        // Load tiles
        layer->setupTiles();
        auto data = loadDataElement(xmlLayer->FirstChildElement("data"));
        auto data_size = data.size();
        for (int i = 0; i < data_size; i++)
        {
            //XXX this might depend on map's renderorder
            auto x = i % layer->getWidth();
            auto y = i / layer->getWidth();
            layer->setTile(x, y, data[i]);
        }

        // Save layer
        map.addLayer(layer);

        xmlLayer = xmlLayer->NextSiblingElement("layer");
    }

    // Load image layers
    auto xmlImgLayer = xmlMap->FirstChildElement("imagelayer");
    while (xmlImgLayer)
    {
        auto imgLayer = new TMXImageLayer();

        imgLayer->setName(xmlImgLayer->Attribute("name"));

        // Load properties
        attValue = xmlImgLayer->Attribute("x");
        if (attValue)
        {
            sscanf(attValue, "%d", &iValue);
            imgLayer->setX(iValue);
        }
        attValue = xmlImgLayer->Attribute("y");
        if (attValue)
        {
            sscanf(attValue, "%d", &iValue);
            imgLayer->setY(iValue);
        }

        attValue = xmlImgLayer->Attribute("opacity");
        if (attValue)
        {
            sscanf(attValue, "%f", &fValue);
            imgLayer->setOpacity(fValue);
        }

        attValue = xmlImgLayer->Attribute("visible");
        if (attValue)
        {
            sscanf(attValue, "%u", &uiValue);
            imgLayer->setVisible(uiValue == 1);
        }

        parseProperties(xmlImgLayer->FirstChildElement("properties"), imgLayer->getProperties());

        // Load image source. Don't worry about <data>, trans, width, height
        auto xmlImage = xmlImgLayer->FirstChildElement("image");
        if (!xmlImage)
        {
            return false;
        }
        imgLayer->setImagePath(xmlImage->Attribute("source"));

        // Save image layer
        map.addLayer(imgLayer);

        xmlImgLayer = xmlImgLayer->NextSiblingElement("imagelayer");
    }

    return true;
}

void TMXSceneEncoder::parseProperties(const tinyxml2::XMLElement* xmlProperties, gameplay::TMXProperties& properties) const
{
    if (!xmlProperties)
    {
        return;
    }

    auto xmlProperty = xmlProperties->FirstChildElement("property");
    while (xmlProperty)
    {
        properties[xmlProperty->Attribute("name")] = xmlProperty->Attribute("value");

        xmlProperty = xmlProperty->NextSiblingElement("property");
    }
}

//XXX could probably seperate the writing process to a seperate class (PropertyWriter...)
#define WRITE_PROPERTY_BLOCK_START(str) writeLine(file, (str)); \
    writeLine(file, "{"); \
    _tabCount++
#define WRITE_PROPERTY_BLOCK_END() _tabCount--; \
    writeLine(file, "}")
#define WRITE_PROPERTY_BLOCK_VALUE(name, value) writeLine(file, string(name) + " = " + (value))
#define WRITE_PROPERTY_DIRECT(value) writeLine(file, (value))
#define WRITE_PROPERTY_NEWLINE() file << std::endl

void TMXSceneEncoder::writeScene(const TMXMap& map, const string& outputFilepath, const string& sceneName)
{
    // Prepare for writing the scene
    std::ofstream file(outputFilepath.c_str(), std::ofstream::out | std::ofstream::trunc);

    auto layerCount = map.getLayerCount();

    // Write initial scene
    WRITE_PROPERTY_BLOCK_START("scene " + sceneName);

    // Write all layers
    for (auto i = 0u; i < layerCount; i++)
    {
        auto layer = map.getLayer(i);
        auto type = layer->getType();
        if (type == TMXLayerType::NormalLayer)
        {
            writeTileset(map, dynamic_cast<const TMXLayer*>(layer), file);
            WRITE_PROPERTY_NEWLINE();
        }
        else if (type == TMXLayerType::ImageLayer)
        {
            writeSprite(dynamic_cast<const TMXImageLayer*>(layer), file);
            WRITE_PROPERTY_NEWLINE();
        }
    }

    WRITE_PROPERTY_BLOCK_END();

    // Cleanup
    file.flush();
    file.close();
}

// This is actually a misnomer. What is a Layer in Tiled/TMX is a TileSet for GamePlay3d. TileSet in Tiled/TMX is something different.
void TMXSceneEncoder::writeTileset(const TMXMap& map, const TMXLayer* tileset, std::ofstream& file)
{
    if (!tileset || !tileset->hasTiles())
    {
        return;
    }

    auto tilesets = tileset->getTilesetsUsed(map);
    if (tilesets.size() == 0)
    {
        return;
    }

    char buffer[BUFFER_SIZE];
    WRITE_PROPERTY_BLOCK_START("node " + tileset->getName());

    if (tilesets.size() > 1)
    {
        auto i = 0u;
        for (auto it = tilesets.begin(); it != tilesets.end(); it++, i++)
        {
            snprintf(buffer, BUFFER_SIZE, "node tileset_%d", i);
            WRITE_PROPERTY_BLOCK_START(buffer);

            auto tmxTileset = map.getTileSet(*it);
            writeSoloTileset(map, tmxTileset, *tileset, file, *it);

            auto tileOffset = tmxTileset.getOffset();
            if (!(tileOffset == Vector2::zero()))
            {
                // Tile offset moves the tiles, not the origin of each tile
                snprintf(buffer, BUFFER_SIZE, "translate = %d, %d, 0", static_cast<int>(tileOffset.x), static_cast<int>(tileOffset.y));
                WRITE_PROPERTY_NEWLINE();
                WRITE_PROPERTY_DIRECT(buffer);
            }

            WRITE_PROPERTY_BLOCK_END();
            if ((i + 1) != tilesets.size())
            {
                WRITE_PROPERTY_NEWLINE();
            }
        }
    }
    else
    {
        auto tmxTileset = map.getTileSet(*(tilesets.begin()));
        writeSoloTileset(map, tmxTileset, *tileset, file);

        auto tileOffset = tmxTileset.getOffset();
        if (!(tileOffset == Vector2::zero()))
        {
            // Tile offset moves the tiles, not the origin of each tile
            snprintf(buffer, BUFFER_SIZE, "translate = %d, %d, 0", static_cast<int>(tileOffset.x), static_cast<int>(tileOffset.y));
            WRITE_PROPERTY_NEWLINE();
            WRITE_PROPERTY_DIRECT(buffer);
        }
    }

    writeNodeProperties(tileset->getVisible(), tileset->getProperties(), file);
    WRITE_PROPERTY_BLOCK_END();
}

void TMXSceneEncoder::writeSoloTileset(const TMXMap& map, const gameplay::TMXTileSet& tmxTileset, const TMXLayer& tileset, std::ofstream& file, unsigned int resultOnlyForTileset)
{
    WRITE_PROPERTY_BLOCK_START("tileset");

    // Write tile path
    WRITE_PROPERTY_BLOCK_VALUE("path", tmxTileset.getImagePath());
    WRITE_PROPERTY_NEWLINE();

    // Write tile size
    //XXX if tile sizes are incorrect, make sure to update TMXLayer::getTileStart too
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "tileWidth = %u", tmxTileset.getMaxTileWidth());
    WRITE_PROPERTY_DIRECT(buffer);
    snprintf(buffer, BUFFER_SIZE, "tileHeight = %u", tmxTileset.getMaxTileHeight());
    WRITE_PROPERTY_DIRECT(buffer);

    // Write tileset size
    snprintf(buffer, BUFFER_SIZE, "columns = %u", tileset.getWidth());
    WRITE_PROPERTY_DIRECT(buffer);
    snprintf(buffer, BUFFER_SIZE, "rows = %u", tileset.getHeight());
    WRITE_PROPERTY_DIRECT(buffer);
    WRITE_PROPERTY_NEWLINE();

    // Write opacity
    if (tileset.getOpacity() < 1.0f)
    {
        snprintf(buffer, BUFFER_SIZE, "opacity = %f", tileset.getOpacity());
        WRITE_PROPERTY_DIRECT(buffer);
        WRITE_PROPERTY_NEWLINE();
    }

    // Write tiles
    auto tileset_height = tileset.getHeight();
    auto tileset_width = tileset.getWidth();
    for (auto y = 0u; y < tileset_height; y++)
    {
        bool tilesWritten = false;
        for (auto x = 0u; x < tileset_width; x++)
        {
            Vector2 startPos = tileset.getTileStart(x, y, map, resultOnlyForTileset);
            if (startPos.x < 0 || startPos.y < 0)
            {
                continue;
            }

            tilesWritten = true;
            WRITE_PROPERTY_BLOCK_START("tile");
            snprintf(buffer, BUFFER_SIZE, "cell = %u, %u", x, y);
            WRITE_PROPERTY_DIRECT(buffer);
            snprintf(buffer, BUFFER_SIZE, "source = %u, %u", static_cast<unsigned int>(startPos.x), static_cast<unsigned int>(startPos.y));
            WRITE_PROPERTY_DIRECT(buffer);
            WRITE_PROPERTY_BLOCK_END();
        }
        if (tilesWritten && ((y + 1) != tileset_height))
        {
            WRITE_PROPERTY_NEWLINE();
        }
    }

    WRITE_PROPERTY_BLOCK_END();
}

void TMXSceneEncoder::writeSprite(const gameplay::TMXImageLayer* imageLayer, std::ofstream& file)
{
    if (!imageLayer)
    {
        return;
    }

    WRITE_PROPERTY_BLOCK_START("node " + imageLayer->getName());

    // Sprite
    {
        WRITE_PROPERTY_BLOCK_START("sprite");

        WRITE_PROPERTY_BLOCK_VALUE("path", imageLayer->getImagePath());
        WRITE_PROPERTY_NEWLINE();

        WRITE_PROPERTY_BLOCK_END();
    }

    writeNodeProperties(imageLayer->getVisible(), imageLayer->getPosition(), imageLayer->getProperties(), file);
    WRITE_PROPERTY_BLOCK_END();
}

void TMXSceneEncoder::writeNodeProperties(bool enabled, const Vector2& pos, const TMXProperties& properties, std::ofstream& file, bool seperatorLineWritten)
{
    char buffer[BUFFER_SIZE];
    if (!enabled)
    {
        // Default is true, so only write it false
        WRITE_PROPERTY_NEWLINE();
        seperatorLineWritten = true;
        WRITE_PROPERTY_DIRECT("enabled = false");
    }
    if (!(pos == Vector2::zero()))
    {
        if (!seperatorLineWritten)
        {
            WRITE_PROPERTY_NEWLINE();
            seperatorLineWritten = true;
        }
        snprintf(buffer, BUFFER_SIZE, "translate = %d, %d, 0", static_cast<int>(pos.x), static_cast<int>(pos.y));
        WRITE_PROPERTY_DIRECT(buffer);
    }

    if (properties.size() > 0)
    {
        WRITE_PROPERTY_NEWLINE();
        WRITE_PROPERTY_BLOCK_START("tags");
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            WRITE_PROPERTY_BLOCK_VALUE(it->first, it->second);
        }
        WRITE_PROPERTY_BLOCK_END();
    }
}

#undef WRITE_PROPERTY_NEWLINE
#undef WRITE_PROPERTY_DIRECT
#undef WRITE_PROPERTY_BLOCK_VALUE
#undef WRITE_PROPERTY_BLOCK_END
#undef WRITE_PROPERTY_BLOCK_START

void TMXSceneEncoder::writeLine(std::ofstream& file, const string& line) const
{
    file << TAB_STRING(_tabCount) << line << std::endl;
}

bool isBase64(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        (c == '=');
}

std::vector<unsigned int> TMXSceneEncoder::loadDataElement(const XMLElement* data)
{
    if (!data)
    {
        return std::vector<unsigned int>();
    }

    const char* encoding = data->Attribute("encoding");
    const char* compression = data->Attribute("compression");

    const char* attValue = "0";
    unsigned int tileGid;
    std::vector<unsigned int> tileData;

    if (!compression && !encoding)
    {
        auto xmlTile = data->FirstChildElement("tile");
        while (xmlTile)
        {
            attValue = xmlTile->Attribute("gid");
            sscanf(attValue, "%u", &tileGid);
            tileData.push_back(tileGid);

            xmlTile = xmlTile->NextSiblingElement("tile");
        }
    }
    else
    {
        if (strcmp(encoding, "csv") == 0)
        {
            if (compression)
            {
                LOG(1, "Compression is not supported with CSV encoding.\n");
                exit(-1);
            }

            auto rawCsvData = data->GetText();
            int start = 0;
            char* endptr;
            // Skip everything before values
            while (rawCsvData[start] == ' ' || rawCsvData[start] == '\n' || rawCsvData[start] == '\r' || rawCsvData[start] == '\t')
            {
                start++;
            }
            // Iterate through values. Skipping to next value when done
            while (rawCsvData[start])
            {
                tileData.push_back(strtoul(rawCsvData + start, &endptr, 10));
                start = endptr - rawCsvData;
                while (rawCsvData[start] == ' ' || rawCsvData[start] == ',' || rawCsvData[start] == '\n' || rawCsvData[start] == '\r' || rawCsvData[start] == '\t')
                {
                    start++;
                }
            }
        }
        else if (strcmp(encoding, "base64") == 0)
        {
            string base64Data = data->GetText();
            if (base64Data.size() > 0 && (!isBase64(base64Data[0]) || !isBase64(base64Data[base64Data.size() - 1])))
            {
                int start = 0;
                auto end = base64Data.size() - 1;
                while (!isBase64(base64Data[start]))
                {
                    start++;
                }
                while (!isBase64(base64Data[end]))
                {
                    end--;
                }
                base64Data = base64Data.substr(start, end - start + 1);
            }
            auto byteData = base64_decode(base64Data);

            if (compression)
            {
                int err;
                const unsigned int buffer_size = 4096;
                char buffer[buffer_size];
                string decomData;

                if (strcmp(compression, "zlib") == 0 || strcmp(compression, "gzip") == 0)
                {
                    z_stream d_stream;
                    d_stream.zalloc = Z_NULL;
                    d_stream.zfree = Z_NULL;
                    d_stream.opaque = Z_NULL;
                    d_stream.next_in = (Bytef*)byteData.c_str();
                    d_stream.avail_in = byteData.size();

                    if (strcmp(compression, "zlib") == 0)
                    {
                        // zlib
                        if ((err = inflateInit(&d_stream)) != Z_OK)
                        {
                            LOG(1, "ZLIB inflateInit failed. Error: %d.\n", err);
                            exit(-1);
                        }
                    }
                    else
                    {
                        // gzip
                        if ((err = inflateInit2(&d_stream, 16+MAX_WBITS)) != Z_OK)
                        {
                            LOG(1, "ZLIB inflateInit2 failed. Error: %d.\n", err);
                            exit(-1);
                        }
                    }

                    do
                    {
                        d_stream.next_out = (Bytef*)buffer;
                        d_stream.avail_out = buffer_size;
                        err = inflate(&d_stream, Z_NO_FLUSH);
                        switch (err)
                        {
                            case Z_NEED_DICT:
                            case Z_DATA_ERROR:
                            case Z_MEM_ERROR:
                                inflateEnd(&d_stream);
                                LOG(1, "ZLIB inflate failed. Error: %d.\n", err);
                                exit(-1);
                                break;
                        }

                        string decomBlock(buffer, buffer_size - d_stream.avail_out);
                        decomData += decomBlock;
                    } while (err != Z_STREAM_END);

                    inflateEnd(&d_stream);
                }
                else
                {
                    LOG(1, "Unknown compression: %s.\n", compression);
                    exit(-1);
                }

                byteData = decomData;
            }

            auto byteDataSize = byteData.size();
            for (unsigned int i = 0; i < byteDataSize; i += 4)
            {
                unsigned int gid = static_cast<unsigned char>(byteData[i + 0]) |
                    (static_cast<unsigned char>(byteData[i + 1]) << 8u) | 
                    (static_cast<unsigned char>(byteData[i + 2]) << 16u) |
                    (static_cast<unsigned char>(byteData[i + 3]) << 24u);
                tileData.push_back(gid);
            }
        }
        else
        {
            LOG(1, "Unknown encoding: %s.\n", encoding);
            exit(-1);
        }
    }

    return tileData;
}
