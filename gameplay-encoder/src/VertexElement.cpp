#include "Base.h"
#include "VertexElement.h"

namespace gameplay
{

VertexElement::VertexElement(unsigned int t, unsigned int c) :
    usage(t),
    size(c)
{
}

VertexElement::~VertexElement(void)
{
}
const char* VertexElement::getElementName(void) const
{
    return "VertexElement";
}

void VertexElement::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(usage, file);
    write(size, file);
}
void VertexElement::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "usage", usageStr(usage));
    fprintfElement(file, "size", size);
    fprintElementEnd(file);
}

const char* VertexElement::usageStr(unsigned int usage)
{
    switch (usage)
    {
        case UNKNOWN:
            return "UNKNOWN";
        case POSITION:
            return "POSITION";
        case NORMAL:
            return "NORMAL";
        case COLOR:
            return "COLOR";
        case TANGENT:
            return "TANGENT";
        case BINORMAL:
            return "BINORMAL";
        case BLENDWEIGHTS:
            return "BLENDWEIGHTS";
        case BLENDINDICES:
            return "BLENDINDICES";
        case TEXCOORD0:
            return "TEXCOORD0";
        case TEXCOORD1:
            return "TEXCOORD1";
        case TEXCOORD2:
            return "TEXCOORD2";
        case TEXCOORD3:
            return "TEXCOORD3";
        case TEXCOORD4:
            return "TEXCOORD4";
        case TEXCOORD5:
            return "TEXCOORD5";
        case TEXCOORD6:
            return "TEXCOORD6";
        case TEXCOORD7:
            return "TEXCOORD7";
        default:
            return "";
    }
}

}
