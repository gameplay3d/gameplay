#include "Base.h"
#include "Font.h"

namespace gameplay
{

Font::Font(void) :
    style(0),
    size(0),
    texMapWidth(0),
    texMapHeight(0)
{
}

Font::~Font(void)
{
}

unsigned int Font::getTypeId(void) const
{
    return FONT_ID;
}
const char* Font::getElementName(void) const
{
    return "Font";
}

void Font::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(family, file);
    write(style, file);
    write(size, file);
    write(charset, file);
    writeBinaryObjects(glyphs, file);
    write(texMapWidth, file);
    write(texMapHeight, file);
    write(texMap, file);
}
void Font::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "family", family);
    fprintfElement(file, "style", style);
    fprintfElement(file, "size", size);
    fprintfElement(file, "alphabet", charset);
    //fprintfElement(file, "glyphs", glyphs);
    fprintfElement(file, "texMapWidth", texMapWidth);
    fprintfElement(file, "texMapHeight", texMapHeight);
    //fprintfElement(file, "texMap", texMap);
    fprintElementEnd(file);
}

}
