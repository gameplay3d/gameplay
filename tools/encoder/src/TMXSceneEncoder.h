#ifndef TMXSCENEEENCODER_H_
#define TMXSCENEEENCODER_H_

#include <fstream>
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
    void write(const std::string& filepath, const gameplay::EncoderArguments& arguments);

private:
    static std::vector<unsigned int> loadDataElement(const tinyxml2::XMLElement* data);

    bool parseTmx(const tinyxml2::XMLDocument& xmlDoc, gameplay::TMXMap& map, const std::string& outputDirectory) const;
    void writeScene(const gameplay::TMXMap& map, const std::string& outputFilepath, const std::string& sceneName);

    void writeTileset(const gameplay::TMXMap& map, unsigned int tileSetIndex, std::ofstream& file);
    void writeSoloTileset(const gameplay::TMXMap& map, const gameplay::TMXTileSet& tmxTileset, const gameplay::TMXLayer& tileset, std::ofstream& file, unsigned int resultOnlyForTileset = TMX_INVALID_ID);

    void writeLine(std::ofstream& file, const std::string& line) const;

    unsigned int _tabCount;
};

#endif
