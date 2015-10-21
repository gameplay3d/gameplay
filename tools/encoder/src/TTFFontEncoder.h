#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"

#define GLYPH_INDEX_START		32
#define GLYPH_INDEX_END			255
#define GLYPH_PADDING			4

namespace gameplay
{

// Structure of TTF Glyph.
class TTFGlyph
{
public:
    unsigned int index;
    unsigned int width;
    int bearingX;
    unsigned int advance;
    float uvCoords[4];
};

/**
 * Writes the font gpb file.
 * 
 * @param inFilePath Input file path to the tiff file.
 * @param outFilePath Output file path to write the gpb to.
 * @param fontSizes List of sizes to generate for the font.
 * @param id ID string of the font in the ref table.
 * @param fontpreview True if the pgm font preview file should be written. (For debugging)
 * @param fontFormat font format
 * @param indexStart Start of the range of glyphs to be generated
 * @param indexEnd End of the range of glyphs to be generated
 * 
 * @return 0 if successful, -1 if error.
 */
int writeFont(const char* inFilePath, const char* outFilePath, std::vector<unsigned int>& fontSize, const char* id, bool fontpreview, Font::FontFormat fontFormat, unsigned int indexStart, unsigned int indexEnd);

}
