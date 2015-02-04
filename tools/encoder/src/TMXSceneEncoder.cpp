#include <string>

#include <zlib.h>

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

void TMXSceneEncoder::write(const EncoderArguments& arguments)
{
    XMLDocument xmlDoc;
    XMLError err;
    if ((err = xmlDoc.LoadFile(arguments.getFilePath().c_str())) != XML_NO_ERROR)
    {
        LOG(1, "Call to XMLDocument::LoadFile() failed.\n");
        LOG(1, "Error returned: %d\n\n", err);
        return;
    }
    
    // Parse the Tiled map
    TMXMap map;
    string inputDirectory = arguments.getFileDirPath();

    LOG(2, "Parsing .tmx file.\n");
    if (!parseTmx(xmlDoc, map, inputDirectory))
    {
        return;
    }

    // Apply a gutter, or skirt, around the tiles to prevent gaps
    if (arguments.generateTextureGutter())
    {
        LOG(2, "Bulding gutter tilesets.\n");
        buildTileGutter(map, inputDirectory, arguments.getOutputDirPath());
    }

    // Write the tile map
    string fileName = arguments.getFileName();
    int pos = fileName.find_last_of('.');

    LOG(2, "Writing .scene file.\n");
    writeScene(map, arguments.getOutputFilePath(), (pos == -1 ? fileName : fileName.substr(0, pos)));
}

bool TMXSceneEncoder::parseTmx(const XMLDocument& xmlDoc, TMXMap& map, const string& inputDirectory) const
{
    const XMLElement* xmlMap = xmlDoc.FirstChildElement("map");
    if (!xmlMap)
    {
        LOG(1, "Missing root <map> element.\n");
        return false;
    }

    // Read in the map values //XXX should compact this so XML attribute parsing is a little nicer
    unsigned int uiValue;
    int iValue;
    const char* attValue = xmlMap->Attribute("width");
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
    const XMLElement* xmlTileSet = xmlMap->FirstChildElement("tileset");
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
            XMLError err;
            string tsxLocation = buildFilePath(inputDirectory, attValue);
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
        const XMLElement* xmlTileOffset = xmlTileSetToLoad->FirstChildElement("tileoffset");
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

        // Load image source. Don't worry about <data> or trans
        const XMLElement* xmlTileSetImage = xmlTileSetToLoad->FirstChildElement("image");
        if (!xmlTileSetImage)
        {
            LOG(1, "Could not find <image> element for tileset.\n");
            return false;
        }
        tileSet.setImagePath(xmlTileSetImage->Attribute("source"));

        if (xmlTileSetImage->Attribute("width") && xmlTileSetImage->Attribute("height"))
        {
            attValue = xmlTileSetImage->Attribute("width");
            sscanf(attValue, "%u", &uiValue);
            tileSet.setImageWidth(uiValue);

            attValue = xmlTileSetImage->Attribute("height");
            sscanf(attValue, "%u", &uiValue);
            tileSet.setImageHeight(uiValue);
        }
        else
        {
            // Load the image itself to get the width
            string imageLocation = buildFilePath(inputDirectory, tileSet.getImagePath());
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
            tileSet.setImageHeight(img->getHeight());
            SAFE_DELETE(img);
        }

        //TODO: tiles (specifically, tile animations) if possible. May require marking tiles as special tiles, and spinning them off to Sprites
        //<tile id="relId"><animation><frame tileid="relId" duration="numInMS"></...></...></...>

        // Save the tileset
        map.addTileSet(tileSet);

        xmlTileSet = xmlTileSet->NextSiblingElement("tileset");
    }

    // Load the layers
    const XMLElement* xmlLayer = xmlMap->FirstChildElement("layer");
    while (xmlLayer)
    {
        TMXLayer* layer = new TMXLayer();

        parseBaseLayerProperties(xmlLayer, layer);

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

        // Load tiles
        layer->setupTiles();
        auto data = loadDataElement(xmlLayer->FirstChildElement("data"));
        size_t dataSize = data.size();
        for (int i = 0; i < dataSize; i++)
        {
            //XXX this might depend on map's renderorder... not sure
            unsigned int x = i % layer->getWidth();
            unsigned int y = i / layer->getWidth();
            layer->setTile(x, y, data[i]);
        }

        // Save layer
        map.addLayer(layer);

        xmlLayer = xmlLayer->NextSiblingElement("layer");
    }

    // Load image layers
    const XMLElement* xmlImgLayer = xmlMap->FirstChildElement("imagelayer");
    while (xmlImgLayer)
    {
        TMXImageLayer* imgLayer = new TMXImageLayer();

        parseBaseLayerProperties(xmlImgLayer, imgLayer);

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

        // Load image source. Don't worry about <data>, trans, width, or height
        const XMLElement* xmlImage = xmlImgLayer->FirstChildElement("image");
        if (!xmlImage)
        {
            LOG(1, "Could not find <image> element for the image layer.\n");
            return false;
        }
        imgLayer->setImagePath(xmlImage->Attribute("source"));

        // Save image layer
        map.addLayer(imgLayer);

        xmlImgLayer = xmlImgLayer->NextSiblingElement("imagelayer");
    }

    return true;
}

void TMXSceneEncoder::parseBaseLayerProperties(const tinyxml2::XMLElement* xmlBaseLayer, gameplay::TMXBaseLayer* layer) const
{
    layer->setName(xmlBaseLayer->Attribute("name"));

    float fValue;
    unsigned int uiValue;

    const char* attValue = xmlBaseLayer->Attribute("opacity");
    if (attValue)
    {
        sscanf(attValue, "%f", &fValue);
        layer->setOpacity(fValue);
    }

    attValue = xmlBaseLayer->Attribute("visible");
    if (attValue)
    {
        sscanf(attValue, "%u", &uiValue);
        layer->setVisible(uiValue == 1);
    }

    const XMLElement* xmlProperties = xmlBaseLayer->FirstChildElement("properties");
    if (xmlProperties)
    {
        TMXProperties& properties = layer->getProperties();
        const XMLElement* xmlProperty = xmlProperties->FirstChildElement("property");
        while (xmlProperty)
        {
            properties[xmlProperty->Attribute("name")] = xmlProperty->Attribute("value");

            xmlProperty = xmlProperty->NextSiblingElement("property");
        }
    }
}

void TMXSceneEncoder::buildTileGutter(TMXMap& map, const string& inputDirectory, const string& outputDirectory)
{
#define ACTUAL_ADJUST_TILESET(imgPath, spacing, margin) tileset.setImagePath((imgPath)); \
    tileset.setSpacing(tileset.getSpacing() + (spacing), false); \
    tileset.setMargin(tileset.getMargin() + (margin), false); \
    tileset.setImageWidth(TMXTileSet::calculateImageDimension(tileset.getHorizontalTileCount(), tileset.getMaxTileWidth(), tileset.getSpacing(), tileset.getMargin())); \
    tileset.setImageHeight(TMXTileSet::calculateImageDimension(tileset.getVerticalTileCount(), tileset.getMaxTileHeight(), tileset.getSpacing(), tileset.getMargin()))
#define ADJUST_TILESET(imgPath) ACTUAL_ADJUST_TILESET(imgPath, 2, 1)
#define UNDO_ADJUST_TILESET(imgPath) ACTUAL_ADJUST_TILESET(imgPath, -2, -1)

    std::unordered_map<std::string, std::string> processedFiles;
    std::unordered_map<std::string, std::string> reverseLookupProcessedFiles;

    unsigned int tilesetCount = map.getTileSetCount();
    for (unsigned int i = 0; i < tilesetCount; i++)
    {
        TMXTileSet& tileset = map.getTileSet(i);

        // See if the tileset was already processed. Then we can skip it
        auto potentialEasyProcess = processedFiles.find(tileset.getImagePath());
        if (potentialEasyProcess != processedFiles.end())
        {
            ADJUST_TILESET(potentialEasyProcess->second);
            continue;
        }

        // Process tileset
        string orgImgPath = tileset.getImagePath();
        string imgPath = orgImgPath;
        string inputFile = buildFilePath(inputDirectory, imgPath);
        int pos = imgPath.find_last_of('.');
        if (pos == -1)
        {
            imgPath = imgPath + "_guttered";
        }
        else
        {
            imgPath = imgPath.substr(0, pos) + "_guttered" + imgPath.substr(pos);
        }
        string outputFile = buildFilePath(outputDirectory, imgPath);

        if (buildTileGutterTileset(tileset, inputFile, outputFile))
        {
            // Update tileset
            ADJUST_TILESET(imgPath);

            // Remember image paths
            processedFiles[orgImgPath] = imgPath;
            reverseLookupProcessedFiles[imgPath] = orgImgPath;
        }
        else
        {
            // Revert all processed tilemaps
            for (unsigned int k = 0; k < i; k++)
            {
                tileset = map.getTileSet(k);

                // Revert to original image and settings
                orgImgPath = reverseLookupProcessedFiles[tileset.getImagePath()];
                UNDO_ADJUST_TILESET(orgImgPath);

                // Delete the processed image, if they exist
                auto it = processedFiles.find(orgImgPath);
                if (it != processedFiles.end())
                {
                    outputFile = buildFilePath(outputDirectory, it->second);
                    if (remove(outputFile.c_str()) != 0)
                    {
                        LOG(3, "Could not remove '%s' during tileset revert.\n", outputFile.c_str());
                    }
                    processedFiles.erase(it);
                }
            }
            LOG(1, "Failed to process '%s'. Reverting all tileset gutters.\n", orgImgPath.c_str());
            return;
        }
    }

#undef UNDO_ADJUST_TILESET
#undef ADJUST_TILESET
#undef ACTUAL_ADJUST_TILESET
}

bool TMXSceneEncoder::buildTileGutterTileset(const TMXTileSet& tileset, const string& inputFile, const string& outputFile)
{
#define COPY_CONTENTS(sx, sy, dx, dy, w, h) copyImage(outputData, static_cast<unsigned char*>(inputImage->getData()), \
    inputImageWidth, outputImageWidth, bpp, \
    (sx), (sy), (dx), (dy), (w), (h))

    // Setup images
    const Image* inputImage = Image::create(inputFile.c_str());
    if (!inputImage)
    {
        return false;
    }

    unsigned int tilesetWidth = tileset.getHorizontalTileCount();
    unsigned int tilesetHeight = tileset.getVerticalTileCount();
    Vector2 tileSize = Vector2(tileset.getMaxTileWidth(), tileset.getMaxTileHeight());
    unsigned int bpp = 0;
    switch (inputImage->getFormat())
    {
        case Image::LUMINANCE:
            bpp = 1;
            break;
        case Image::RGB:
            bpp = 3;
            break;
        case Image::RGBA:
            bpp = 4;
            break;
        default:
            LOG(4, "Unknown image format. Possibly need update by developer.\n")
            return false;
    }

    Image* outputImage = Image::create(inputImage->getFormat(),
        TMXTileSet::calculateImageDimension(tilesetWidth, tileSize.x, tileset.getSpacing() + 2, tileset.getMargin() + 1),
        TMXTileSet::calculateImageDimension(tilesetHeight, tileSize.y, tileset.getSpacing() + 2, tileset.getMargin() + 1));

    // Get a couple variables so we don't constantly call functions (they aren't inline)
    unsigned int inputImageWidth = inputImage->getWidth();
    unsigned int inputImageHeight = inputImage->getHeight();
    unsigned int outputImageWidth = outputImage->getWidth();
    unsigned int outputImageHeight = outputImage->getHeight();
    unsigned char* outputData = static_cast<unsigned char*>(outputImage->getData());
    unsigned int tilesetSpacing = tileset.getSpacing();
    unsigned int tilesetMargin = tileset.getMargin();

    // Copy margin
    if (tilesetMargin != 0)
    {
        // Horizontal
        unsigned int avaliable = inputImageWidth;
        unsigned int remaining = outputImageWidth;
        unsigned int use = min(avaliable, remaining);

        while (remaining > 0)
        {
            // Top
            COPY_CONTENTS(0, 0, outputImageWidth - remaining, 0, use, tilesetMargin);

            // Bottom
            COPY_CONTENTS(0, inputImageHeight - tilesetMargin - 1, outputImageWidth - remaining, outputImageHeight - tilesetMargin - 1, use, tilesetMargin);

            remaining -= use;
            use = min(avaliable, remaining);
        }

        // Vertical
        avaliable = inputImageHeight;
        remaining = outputImageHeight;
        use = min(avaliable, remaining);

        while (remaining > 0)
        {
            // Left
            COPY_CONTENTS(0, 0, 0, outputImageHeight - remaining, tilesetMargin, use);

            // Right
            COPY_CONTENTS(inputImageWidth - tilesetMargin - 1, 0, inputImageWidth - tilesetMargin - 1, outputImageHeight - remaining, tilesetMargin, use);

            remaining -= use;
            use = min(avaliable, remaining);
        }
    }

    // Copy the contents of each tile
    for (unsigned int y = 0; y < tilesetHeight; y++)
    {
        for (unsigned int x = 0; x < tilesetWidth; x++)
        {
            Vector2 src = TMXTileSet::calculateTileOrigin(Vector2(x, y), tileSize, tilesetSpacing, tilesetMargin);
            Vector2 dst = TMXTileSet::calculateTileOrigin(Vector2(x, y), tileSize, tilesetSpacing + 2, tilesetMargin + 1);

            COPY_CONTENTS(src.x, src.y, dst.x, dst.y, tileSize.x, tileSize.y);
        }
    }
    
    // Extend the edges of the tiles to produce the "gutter"
    unsigned int gutterContents = outputImageHeight - ((tilesetMargin + 1) * 2);
    for (unsigned int x = 0; x < tilesetWidth; x++)
    {
        Vector2 pos = TMXTileSet::calculateTileOrigin(Vector2(x, 0), tileSize, tilesetSpacing + 2, tilesetMargin + 1);
        
        // Left
        copyImage(outputData, outputData,
            outputImageWidth, outputImageWidth, bpp,
            pos.x, pos.y, pos.x - 1, pos.y, 1, gutterContents);

        // Right
        copyImage(outputData, outputData,
            outputImageWidth, outputImageWidth, bpp,
            pos.x + tileSize.x - 1, pos.y, pos.x + tileSize.x, pos.y, 1, gutterContents);
    }
    gutterContents = outputImageWidth - ((tilesetMargin + 1) * 2);
    for (unsigned int y = 0; y < tilesetHeight; y++)
    {
        Vector2 pos = TMXTileSet::calculateTileOrigin(Vector2(0, y), tileSize, tilesetSpacing + 2, tilesetMargin + 1);

        // Top
        copyImage(outputData, outputData,
            outputImageWidth, outputImageWidth, bpp,
            pos.x, pos.y, pos.x, pos.y - 1, gutterContents, 1);

        // Bottom
        copyImage(outputData, outputData,
            outputImageWidth, outputImageWidth, bpp,
            pos.x, pos.y + tileSize.y - 1, pos.x, pos.y + tileSize.y, gutterContents, 1);
    }

    // Save and cleanup
    outputImage->save(outputFile.c_str());
    SAFE_DELETE(inputImage);
    SAFE_DELETE(outputImage);

    return true;

#undef COPY_CONTENTS
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

    unsigned int layerCount = map.getLayerCount();

    // Write initial scene
    WRITE_PROPERTY_BLOCK_START("scene " + sceneName);

    // Write all layers
    for (unsigned int i = 0; i < layerCount; i++)
    {
        const TMXBaseLayer* layer = map.getLayer(i);
        TMXLayerType type = layer->getType();
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

    std::set<unsigned int> tilesets = tileset->getTilesetsUsed(map);
    if (tilesets.size() == 0)
    {
        return;
    }

    char buffer[BUFFER_SIZE];
    WRITE_PROPERTY_BLOCK_START("node " + tileset->getName());

    if (tilesets.size() > 1)
    {
        unsigned int i = 0;
        for (auto it = tilesets.begin(); it != tilesets.end(); it++, i++)
        {
            snprintf(buffer, BUFFER_SIZE, "node tileset_%d", i);
            WRITE_PROPERTY_BLOCK_START(buffer);

            const TMXTileSet& tmxTileset = map.getTileSet(*it);
            writeSoloTileset(map, tmxTileset, *tileset, file, *it);

            const Vector2& tileOffset = tmxTileset.getOffset();
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
        const TMXTileSet& tmxTileset = map.getTileSet(*(tilesets.begin()));
        writeSoloTileset(map, tmxTileset, *tileset, file);

        const Vector2& tileOffset = tmxTileset.getOffset();
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
    unsigned int tilesetHeight = tileset.getHeight();
    unsigned int tilesetWidth = tileset.getWidth();
    for (unsigned int y = 0; y < tilesetHeight; y++)
    {
        bool tilesWritten = false;
        for (unsigned int x = 0; x < tilesetWidth; x++)
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
        if (tilesWritten && ((y + 1) != tilesetHeight))
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
        const XMLElement* xmlTile = data->FirstChildElement("tile");
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

            const char* rawCsvData = data->GetText();
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
                size_t end = base64Data.size() - 1;
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
            string byteData = base64_decode(base64Data);

            if (compression)
            {
                int err;
                const unsigned int buffer_size = 4096;
                char buffer[buffer_size];
                string decomData;

                if (strcmp(compression, "zlib") == 0 || strcmp(compression, "gzip") == 0)
                {
                    z_stream dString;
                    dString.zalloc = Z_NULL;
                    dString.zfree = Z_NULL;
                    dString.opaque = Z_NULL;
                    dString.next_in = (Bytef*)byteData.c_str();
                    dString.avail_in = byteData.size();

                    if (strcmp(compression, "zlib") == 0)
                    {
                        // zlib
                        if ((err = inflateInit(&dString)) != Z_OK)
                        {
                            LOG(1, "ZLIB inflateInit failed. Error: %d.\n", err);
                            exit(-1);
                        }
                    }
                    else
                    {
                        // gzip
                        if ((err = inflateInit2(&dString, 16+MAX_WBITS)) != Z_OK)
                        {
                            LOG(1, "ZLIB inflateInit2 failed. Error: %d.\n", err);
                            exit(-1);
                        }
                    }

                    do
                    {
                        dString.next_out = (Bytef*)buffer;
                        dString.avail_out = buffer_size;
                        err = inflate(&dString, Z_NO_FLUSH);
                        switch (err)
                        {
                            case Z_NEED_DICT:
                            case Z_DATA_ERROR:
                            case Z_MEM_ERROR:
                                inflateEnd(&dString);
                                LOG(1, "ZLIB inflate failed. Error: %d.\n", err);
                                exit(-1);
                                break;
                        }

                        string decomBlock(buffer, buffer_size - dString.avail_out);
                        decomData += decomBlock;
                    } while (err != Z_STREAM_END);

                    inflateEnd(&dString);
                }
                else
                {
                    LOG(1, "Unknown compression: %s.\n", compression);
                    exit(-1);
                }

                byteData = decomData;
            }

            size_t byteDataSize = byteData.size();
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

std::string TMXSceneEncoder::buildFilePath(const std::string& directory, const std::string& file)
{
    return EncoderArguments::getRealPath(directory + "/" + file);
}

void TMXSceneEncoder::copyImage(unsigned char* dst, const unsigned char* src,
    unsigned int srcWidth, unsigned int dstWidth, unsigned int bpp,
    unsigned int srcx, unsigned int srcy, unsigned int dstx, unsigned int dsty, unsigned int width, unsigned int height)
{
    unsigned int sizePerRow = width * bpp;
    for (unsigned int y = 0; y < height; y++)
    {
        const unsigned char* srcPtr = &src[((y + srcy) * srcWidth + srcx) * bpp];
        unsigned char* dstPtr = &dst[((y + dsty) * dstWidth + dstx) * bpp];
        if (src == dst)
        {
            memmove(dstPtr, srcPtr, sizePerRow);
        }
        else
        {
            memcpy(dstPtr, srcPtr, sizePerRow);
        }
    }
}
