#include "Base.h"
#include "VertexFormat.h"

namespace gameplay
{

VertexFormat::VertexFormat(const Element* elements, unsigned int elementCount)
    : _vertexSize(0)
{
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
    assert(index < _elements.size());

    return _elements[index];
}

unsigned int VertexFormat::getElementCount() const
{
    return _elements.size();
}

unsigned int VertexFormat::getVertexSize() const
{
    return _vertexSize;
}

bool VertexFormat::operator == (const VertexFormat& f) const
{
    if (_elements.size() != f._elements.size())
        return false;

    for (unsigned int i = 0, count = _elements.size(); i < count; ++i)
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

}
