#include "Base.h"
#include "Glyph.h"

namespace gameplay
{

Glyph::Glyph(void) :
    index(0),
    width(0)
{
    fillArray(uvCoords, 0.0f, 4);
}

    Glyph::~Glyph(void)
{
}

const char* Glyph::getElementName(void) const
{
    return "Glyph";
}

void Glyph::writeBinary(FILE* file)
{
    Object::writeBinary(file);

    write(index, file);
    write(width, file);
    write(uvCoords, 4, file);

}

void Glyph::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "index", index);
    fprintfElement(file, "width", width);
    fprintfElement(file, "uvCoords", uvCoords, 4);
    fprintElementEnd(file);
}

}
