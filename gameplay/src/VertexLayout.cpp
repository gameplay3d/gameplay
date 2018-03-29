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
    semantic(Semantic::ePosition),
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

size_t VertexLayout::toStride(Format format)
{
    switch (format) 
    {
    case Format::eR8Unorm:
        return 1;
    case Format::eR16Unorm:
        return 2;
    case Format::eR16Float:
        return 2;
    case Format::eR32Uint:
        return 4;
    case Format::eR32Float:
        return 4;
    case Format::eR8G8Unorm:
        return 2;
    case Format::eR16G16Unorm:
        return 2;
    case Format::eR16G16Float:
        return 4;
    case Format::eR32G32Uint:
        return 8;
    case Format::eR32G32Float:
        return 8;
    case Format::eR32G32B32Uint:
        return 12;
    case Format::eR32G32B32Float:
        return 12;
    case Format::eB8G8R8A8Unorm:
        return 4;
    case Format::eR8G8B8A8Unorm:
        return 4;
    case Format::eR16G16B16A16Unorm:
        return 8;
    case Format::eR16G16B16A16Float:
        return 8;
    case Format::eR32G32B32A32Uint:
        return 16;
    case Format::eR32G32B32A32Float:
        return 16;
    case Format::eUndefined:
    case Format::eD16Unorm:
    case Format::eX8D24UnormPack32:
    case Format::eD32Float:
    case Format::eS8Uint:
    case Format::eD24UnormS8Uint:
    case Format::eD32FloatS8Uint:
        return 0;
    }
    return 0;
}

}
