#ifndef TMXSCENEEENCODER_H_
#define TMXSCENEEENCODER_H_

#include <fstream>
#include <unordered_map>
#include <tinyxml2.h>

#include "Base.h"
#include "Vector2.h"
#include "TMXTypes.h"
#include "EncoderArguments.h"
#include "Image.h"

/**
 * Class for encoding an TMX file.
 */
class TMXSceneEncoder
{
public:
    /**
     * Constructor.
     */
    TMXSceneEncoder();

    /**
     * Destructor.
     */
    ~TMXSceneEncoder();

    /**
     * Writes out encoded TMX file.
     */
    void write(const gameplay::EncoderArguments& arguments);

private:
    static std::vector<unsigned int> loadDataElement(const tinyxml2::XMLElement* data);
    static inline std::string buildFilePath(const std::string& directory, const std::string& file);
    static void copyImage(unsigned char* dst, const unsigned char* src,
        unsigned int srcWidth, unsigned int dstWidth, unsigned int bpp,
        unsigned int srcx, unsigned int srcy, unsigned int dstx, unsigned int dsty, unsigned int width, unsigned int height);

    // Parsing
    bool parseTmx(const tinyxml2::XMLDocument& xmlDoc, gameplay::TMXMap& map, const std::string& inputDirectory) const;
    void parseBaseLayerProperties(const tinyxml2::XMLElement* xmlBaseLayer, gameplay::TMXBaseLayer* layer) const;

    // Gutter
    void buildTileGutter(gameplay::TMXMap& map, const std::string& inputDirectory, const std::string& outputDirectory);
    bool buildTileGutterTileset(const gameplay::TMXTileSet& tileset, const std::string& inputFile, const std::string& outputFile);

    // Writing
    void writeScene(const gameplay::TMXMap& map, const std::string& outputFilepath, const std::string& sceneName);

    void writeTileset(const gameplay::TMXMap& map, const gameplay::TMXLayer* layer, std::ofstream& file);
    void writeSoloTileset(const gameplay::TMXMap& map, const gameplay::TMXTileSet& tmxTileset, const gameplay::TMXLayer& tileset, std::ofstream& file, unsigned int resultOnlyForTileset = TMX_INVALID_ID);

    void writeSprite(const gameplay::TMXImageLayer* imageLayer, std::ofstream& file);

    void writeNodeProperties(bool enabled, std::ofstream& file, bool seperatorLineWritten = false);
    void writeNodeProperties(bool enabled, const gameplay::TMXProperties& properties, std::ofstream& file, bool seperatorLineWritten = false);
    void writeNodeProperties(bool enabled, const gameplay::Vector2& pos, std::ofstream& file, bool seperatorLineWritten = false);
    void writeNodeProperties(bool enabled, const gameplay::Vector2& pos, const gameplay::TMXProperties& properties, std::ofstream& file, bool seperatorLineWritten = false);

    void writeLine(std::ofstream& file, const std::string& line) const;

    unsigned int _tabCount;
};

inline void TMXSceneEncoder::writeNodeProperties(bool enabled, std::ofstream& file, bool seperatorLineWritten)
{
    writeNodeProperties(enabled, gameplay::Vector2::zero(), file, seperatorLineWritten);
}
inline void TMXSceneEncoder::writeNodeProperties(bool enabled, const gameplay::TMXProperties& properties, std::ofstream& file, bool seperatorLineWritten)
{
    writeNodeProperties(enabled, gameplay::Vector2::zero(), properties, file, seperatorLineWritten);
}
inline void TMXSceneEncoder::writeNodeProperties(bool enabled, const gameplay::Vector2& pos, std::ofstream& file, bool seperatorLineWritten)
{
    gameplay::TMXProperties noOp;
    writeNodeProperties(enabled, pos, noOp, file, seperatorLineWritten);
}

#endif
