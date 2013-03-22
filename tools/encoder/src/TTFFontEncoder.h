#include <ft2build.h>
#include FT_FREETYPE_H

#define START_INDEX     32
#define END_INDEX       127
#define GLYPH_PADDING   4

namespace gameplay
{

// Structure of Glyph.
class Glyph
{
public:
    unsigned int index;
    unsigned int width;
    float uvCoords[4];
};

/**
 * Writes the font gpb file.
 * 
 * @param inFilePath Input file path to the tiff file.
 * @param outFilePath Output file path to write the gpb to.
 * @param fontSize Size of the font.
 * @param id ID string of the font in the ref table.
 * @param fontpreview True if the pgm font preview file should be written. (For debugging)
 * 
 * @return 0 if successful, -1 if error.
 */
int writeFont(const char* inFilePath, const char* outFilePath, unsigned int fontSize, const char* id, bool fontpreview);

}
