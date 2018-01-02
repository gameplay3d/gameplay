#include "Base.h"
#include "VertexLayout.h"
#include "Format.h"

namespace gameplay
{

VertexLayout::VertexLayout()
{
}

VertexLayout::VertexLayout(const Attribute* attributes, size_t attributeCount) : 
	_stride(0)
{
    GP_ASSERT(attributeCount);
	GP_ASSERT(attributeCount < GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);

    for (size_t i = 0; i < attributeCount; ++i)
    {
        Attribute attribute;
        memcpy(&attribute, &attributes[i], sizeof(Attribute));
        _attributes.push_back(attribute);
		_stride += toStride(_attributes[i].format);
    }
}

VertexLayout::~VertexLayout()
{
}

const VertexLayout::Attribute& VertexLayout::getAttribute(size_t index) const
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

VertexLayout::Attribute::Attribute() : 
	semantic(SEMANTIC_POSITION),
	binding(0), 
	location(0), 
	offset(0)
{
}

VertexLayout::Attribute::Attribute(Semantic semantic,
								   Format format,
								   uint32_t binding,
								   uint32_t location,
								   uint32_t offset) : 
	semantic(semantic), 
	format(format), 
	binding(binding), 
	location(location), 
	offset(offset)
{
}

VertexLayout::Attribute::~Attribute()
{
}

bool VertexLayout::Attribute::operator == (const VertexLayout::Attribute& attr) const
{
    return (semantic == attr.semantic) && 
		   (format == attr.format) &&
		   (binding == attr.binding) &&
		   (location == attr.location) &&
		   (offset == attr.offset);
}

bool VertexLayout::Attribute::operator != (const VertexLayout::Attribute& attr) const
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
    case SEMANTIC_BITANGENT:
        return "BITANGENT";
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
        return "";
    }
}

size_t VertexLayout::toStride(Format format)
{
    switch (format) 
	{
	case Format::FORMAT_R8_UNORM: 
		return 1;
	case Format::FORMAT_R16_UNORM: 
		return 2;
	case Format::FORMAT_R16_FLOAT: 
		return 2;
	case Format::FORMAT_R32_UINT: 
		return 4;
	case Format::FORMAT_R32_FLOAT: 
		return 4;
	case Format::FORMAT_R8G8_UNORM: 
		return 2;
	case Format::FORMAT_R16G16_UNORM: 
		return 2;
	case Format::FORMAT_R16G16_FLOAT:
		return 4;
	case Format::FORMAT_R32G32_UINT: 
		return 8;
	case Format::FORMAT_R32G32_FLOAT: 
		return 8;
	case Format::FORMAT_R32G32B32_UINT: 
		return 12;
	case Format::FORMAT_R32G32B32_FLOAT: 
		return 12;
	case Format::FORMAT_B8G8R8A8_UNORM: 
		return 4;
	case Format::FORMAT_R8G8B8A8_UNORM: 
		return 4;
	case Format::FORMAT_R16G16B16A16_UNORM: 
		return 8;
	case Format::FORMAT_R16G16B16A16_FLOAT: 
		return 8;
	case Format::FORMAT_R32G32B32A32_UINT: 
		return 16;
	case Format::FORMAT_R32G32B32A32_FLOAT: 
		return 16;
    case Format::FORMAT_UNDEFINED:
	case Format::FORMAT_D16_UNORM: 
	case Format::FORMAT_X8_D24_UNORM_PACK32:
	case Format::FORMAT_D32_FLOAT:
	case Format::FORMAT_S8_UINT:
	case Format::FORMAT_D24_UNORM_S8_UINT:
	case Format::FORMAT_D32_FLOAT_S8_UINT:
		return 0;
    }
    return 0;
}

}
