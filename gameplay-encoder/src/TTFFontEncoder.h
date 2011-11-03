#include <stdio.h>
#include <ft2build.h>
#include "string.h"
#include <fstream>
#include <math.h>

#include FT_FREETYPE_H

#define START_INDEX     32
#define END_INDEX       127

#define GLYPH_PADDING   4


// Structure of Glyph.
class Glyph
{
public:
    unsigned int index;
    unsigned int width;
    float uvCoords[4];
};


void drawBitmap(unsigned char* dstBitmap, int x, int y, int dstWidth, unsigned char* srcBitmap, int srcWidth, int srcHeight);

void writeUint(FILE* fp, unsigned int i);

void writeFloat(FILE* fp, float f);

void writeString(FILE* fp, const char* str);

int writeFont(const char* filename, unsigned int fontSize, const char* id, bool fontpreview);