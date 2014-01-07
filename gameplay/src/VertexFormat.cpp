#include "Base.h"
#include "VertexFormat.h"

namespace gameplay
{

VertexFormat::VertexFormat(const Element* elements, unsigned int elementCount)
    : _vertexSize(0)
{
    GP_ASSERT(elements);

    // Copy elements and compute vertex size
    for (unsigned int i = 0; i < elementCount; ++i)
    {
        // Copy element
        Element element;
        memcpy(&element, &elements[i], sizeof(Element));
        _elements.push_back(element);

        _vertexSize += element.size * sizeof(float);
    }
}

VertexFormat::~VertexFormat()
{
}

const VertexFormat::Element& VertexFormat::getElement(unsigned int index) const
{
    GP_ASSERT(index < _elements.size());
    return _elements[index];
}

unsigned int VertexFormat::getElementCount() const
{
    return (unsigned int)_elements.size();
}

unsigned int VertexFormat::getVertexSize() const
{
    return _vertexSize;
}

bool VertexFormat::operator == (const VertexFormat& f) const
{
    if (_elements.size() != f._elements.size())
        return false;

    for (size_t i = 0, count = _elements.size(); i < count; ++i)
    {
        if (_elements[i] != f._elements[i])
            return false;
    }

    return true;
}


bool VertexFormat::operator != (const VertexFormat& f) const
{
    return !(*this == f);
}

VertexFormat::Element::Element() :
    usage(POSITION), size(0)
{
}

VertexFormat::Element::Element(Usage usage, unsigned int size) :
    usage(usage), size(size)
{
}

bool VertexFormat::Element::operator == (const VertexFormat::Element& e) const
{
    return (size == e.size && usage == e.usage);
}

bool VertexFormat::Element::operator != (const VertexFormat::Element& e) const
{
    return !(*this == e);
}

const char* VertexFormat::toString(Usage usage)
{
    switch (usage)
    {
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
        return "UNKNOWN";
    }
}

}
