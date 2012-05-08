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

int writeFont(const char* filename, unsigned int fontSize, const char* id, bool fontpreview);

}
