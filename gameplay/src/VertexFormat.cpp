/*
 * VertexFormat.cpp
 */

#include "Base.h"
#include "VertexFormat.h"

namespace gameplay
{

static std::vector<VertexFormat*> __vertexFormatCache;

VertexFormat::VertexFormat() :
    _elements(NULL), _elementCount(0), _vertexSize(0)
{
}

VertexFormat::~VertexFormat()
{
    // Remove from the vertex format cache.
    std::vector<VertexFormat*>::iterator itr = std::find(__vertexFormatCache.begin(), __vertexFormatCache.end(), this);
    if (itr != __vertexFormatCache.end())
    {
        __vertexFormatCache.erase(itr);
    }

    SAFE_DELETE_ARRAY(_elements);
}

VertexFormat* VertexFormat::create(const Element* elements, unsigned int elementCount)
{
    VertexFormat* format;

    // Search for an existing VertexFormat with matching elements.
    for (unsigned int i = 0, count = __vertexFormatCache.size(); i < count; ++i)
    {
        format = __vertexFormatCache[i];
        if (format->_elementCount == elementCount)
        {
            bool match = true;
            for (unsigned int j = 0; j < format->_elementCount; ++j)
            {
                const Element& e1 = format->_elements[j];
                const Element& e2 = elements[j];

                // Compare elements properties.
                if (e1.size != e2.size || e1.usage != e2.usage)
                {
                    // Not a match.
                    match = false;
                    break;
                }
            }
            if (match)
            {
                // Found a match, return it.
                format->addRef();
                return format;
            }
        }
    }

    // Create a new vertex format.
    format = new VertexFormat();
    format->_elementCount = elementCount;
    format->_elements = new Element[elementCount];
    format->_vertexSize = 0;
    bool hasPosition = false;
    for (unsigned int i = 0; i < elementCount; ++i)
    {
        // Validate size (must be between 1-4).
        assert(elements[i].size >= 1 && elements[i].size <= 4);
        if (elements[i].size < 1 || elements[i].size > 4)
        {
            SAFE_DELETE(format);
            return NULL;
        }

        // Validate usage.
        switch (elements[i].usage)
        {
        case POSITION:
            hasPosition = true;
            break;
        case NORMAL:
        case COLOR:
        case TANGENT:
        case BINORMAL:
        case BLENDWEIGHTS:
        case BLENDINDICES:
        case TEXCOORD0:
        case TEXCOORD1:
        case TEXCOORD2:
        case TEXCOORD3:
        case TEXCOORD4:
        case TEXCOORD5:
        case TEXCOORD6:
        case TEXCOORD7:
            break;
        default:
            assert(0); // invalid usage
            SAFE_DELETE(format);
            return NULL;
        }

        // Copy element.
        memcpy(&format->_elements[i], &elements[i], sizeof(Element));
        format->_vertexSize += elements[i].size * sizeof(float);

        if (elements[i].usage == POSITION)
        {
            hasPosition = true;
        }
    }

    // All formats must include 1 POSITION.
    assert(hasPosition);
    if (!hasPosition)
    {
        SAFE_DELETE(format);
        return NULL;
    }

    return format;
}

const VertexFormat::Element& VertexFormat::getElement(unsigned int index) const
{
    assert(index < _elementCount);

    return _elements[index];
}

unsigned int VertexFormat::getElementCount() const
{
    return _elementCount;
}

unsigned int VertexFormat::getVertexSize() const
{
    return _vertexSize;
}

VertexFormat::Element::Element() :
    usage(POSITION), size(0)
{
}

VertexFormat::Element::Element(Usage usage, unsigned int size) :
    usage(usage), size(size)
{
}

}
