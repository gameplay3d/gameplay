#include "Base.h"
#include "VertexLayout.h"

namespace gameplay
{

VertexLayout::VertexLayout()
{
}

VertexLayout::VertexLayout(const VertexAttribute* attributes, size_t count) : 
	_stride(0)
{
    GP_ASSERT(attributes);

    // Copy attributes and compute vertex stride
    for (size_t i = 0; i < count; ++i)
    {
        VertexAttribute attribute;
        memcpy(&attribute, &attributes[i], sizeof(VertexAttribute));
        _attributes.push_back(attribute);
    }
}

VertexLayout::~VertexLayout()
{
}

const VertexLayout::VertexAttribute& VertexLayout::getAttribute(size_t index) const
{
    GP_ASSERT(index < _attributes.size());
    return _attributes[index];
}

size_t VertexLayout::getAttributeCount() const
{
    return _attributes.size();
}

size_t VertexLayout::getStride() const
{
    return _stride;
}

bool VertexLayout::operator == (const VertexLayout& layout) const
{
    if (_attributes.size() != layout._attributes.size())
        return false;

    for (size_t i = 0, count = _attributes.size(); i < count; ++i)
    {
        if (_attributes[i] != layout._attributes[i])
            return false;
    }
    return true;
}

bool VertexLayout::operator != (const VertexLayout& layout) const
{
    return !(*this == layout);
}

VertexLayout::VertexAttribute::VertexAttribute() : 
	semantic(SEMANTIC_POSITION), name(""), binding(0), location(0), offset(0)
{
}

VertexLayout::VertexAttribute::VertexAttribute(Semantic semantic,
											   const std::string& name, 
											   Format format,
											   uint32_t binding,
											   uint32_t location,
											   uint32_t offset) : 
	semantic(semantic), 
	name(name), 
	format(format), 
	binding(binding), 
	location(location), 
	offset(offset)
{
}

bool VertexLayout::VertexAttribute::operator == (const VertexLayout::VertexAttribute& attr) const
{
    return (semantic == attr.semantic) && 
		   (name.size() == attr.name.size()) && 
		   (name.compare(attr.name) == 0) && 
		   (format == attr.format) &&
		   (binding == attr.binding) &&
		   (location == attr.location) &&
		   (offset == attr.offset);
}

bool VertexLayout::VertexAttribute::operator != (const VertexLayout::VertexAttribute& attr) const
{
    return !(*this == attr);
}

std::string VertexLayout::toString(Semantic semantic)
{
    switch (semantic)
    {
    case SEMANTIC_POSITION:
        return "POSITION";
    case SEMANTIC_NORMAL:
        return "NORMAL";
    case SEMANTIC_COLOR:
        return "COLOR";
    case SEMANTIC_TANGENT:
        return "TANGENT";
    case SEMANTIC_BINORMAL:
        return "BINORMAL";
    case SEMANTIC_TEXCOORD0:
        return "TEXCOORD0";
    case SEMANTIC_TEXCOORD1:
        return "TEXCOORD1";
    case SEMANTIC_TEXCOORD2:
        return "TEXCOORD2";
    case SEMANTIC_TEXCOORD3:
        return "TEXCOORD3";
    case SEMANTIC_TEXCOORD4:
        return "TEXCOORD4";
    case SEMANTIC_TEXCOORD5:
        return "TEXCOORD5";
    case SEMANTIC_TEXCOORD6:
        return "TEXCOORD6";
    case SEMANTIC_TEXCOORD7:
        return "TEXCOORD7";
    default:
        return "POSITION";
    }
}

}
