#include "Base.h"
#include "VertexFormat.h"

namespace gameplay
{

VertexFormat::VertexFormat()
{
}

VertexFormat::VertexFormat(const Element* elements, size_t elementCount) : 
	_stride(0)
{
    GP_ASSERT(elements);

    // Copy elements and compute vertex size
    for (size_t i = 0; i < elementCount; ++i)
    {
        Element element;
        memcpy(&element, &elements[i], sizeof(Element));
        _elements.push_back(element);
        _stride += element.size * sizeof(float);
    }
}

VertexFormat::~VertexFormat()
{
}

const VertexFormat::Element& VertexFormat::getElement(size_t index) const
{
    GP_ASSERT(index < _elements.size());
    return _elements[index];
}

size_t VertexFormat::getElementCount() const
{
    return _elements.size();
}

size_t VertexFormat::getStride() const
{
    return _stride;
}

bool VertexFormat::operator == (const VertexFormat& format) const
{
    if (_elements.size() != format._elements.size())
        return false;

    for (size_t i = 0, count = _elements.size(); i < count; ++i)
    {
        if (_elements[i] != format._elements[i])
            return false;
    }
    return true;
}

bool VertexFormat::operator != (const VertexFormat& format) const
{
    return !(*this == format);
}

VertexFormat::Element::Element() : 
	usage(USAGE_POSITION), 
	size(0)
{
}

VertexFormat::Element::Element(Usage usage, size_t size) 
	: usage(usage), size(size)
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

std::string VertexFormat::toString(Usage usage)
{
    switch (usage)
    {
    case USAGE_POSITION:
        return "POSITION";
    case USAGE_NORMAL:
        return "NORMAL";
    case USAGE_COLOR:
        return "COLOR";
    case USAGE_TANGENT:
        return "TANGENT";
    case USAGE_BINORMAL:
        return "BINORMAL";
    case USAGE_BLENDWEIGHTS:
        return "BLENDWEIGHTS";
    case USAGE_BLENDINDICES:
        return "BLENDINDICES";
    case USAGE_TEXCOORD0:
        return "TEXCOORD0";
    case USAGE_TEXCOORD1:
        return "TEXCOORD1";
    case USAGE_TEXCOORD2:
        return "TEXCOORD2";
    case USAGE_TEXCOORD3:
        return "TEXCOORD3";
    case USAGE_TEXCOORD4:
        return "TEXCOORD4";
    case USAGE_TEXCOORD5:
        return "TEXCOORD5";
    case USAGE_TEXCOORD6:
        return "TEXCOORD6";
    case USAGE_TEXCOORD7:
        return "TEXCOORD7";
    default:
        return "POSITION";
    }
}

}
