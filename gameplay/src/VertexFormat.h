/*
 * VertexFormat.h
 */

#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

#include "Ref.h"

namespace gameplay
{

/**
 * Defines the format of a vertex layout used by a mesh.
 *
 * A VertexFormat is immutable and cannot be changed
 * once created.
 */
class VertexFormat : public Ref
{
public:

    /**
     * Defines a set of usages for vertex elements.
     */
    enum Usage
    {
        POSITION = 1,
        NORMAL = 2,
        COLOR = 3,
        TANGENT = 4,
        BINORMAL = 5,
        BLENDWEIGHTS = 6,
        BLENDINDICES = 7,
        TEXCOORD0 = 8,
        TEXCOORD1 = 9,
        TEXCOORD2 = 10,
        TEXCOORD3 = 11,
        TEXCOORD4 = 12,
        TEXCOORD5 = 13,
        TEXCOORD6 = 14,
        TEXCOORD7 = 15
    };

    /**
     * Defines a single element within a vertex format.
     *
     * All vertex elements are assumed to be of type float, but can
     * have a varying number of float values (1-4), which is represented
     * by the size attribute. Additionally, vertex elements are assumed
     * to be tightly packed.
     */
    class Element
    {
    public:
        /**
         * The vertex elemnet usage semantic.
         */
        Usage usage;

        /**
         * The number of values in the vertex element.
         */
        unsigned int size;

        Element();

        Element(Usage usage, unsigned int size);
    };

    /**
     * Returns a unique VertexFormat for the request vertex element layout.
     *
     * @param elements The list of vertex elements defining the vertex format.
     * @param count The number of items in the elements array.
     * 
     * @return A unique VertexFormat object.
     */
    static VertexFormat* create(const Element* elements, unsigned int count);

    /**
     * Returns the vertex element at the specified index.
     */
    const Element& getElement(unsigned int i) const;

    /**
     * Returns the number of elements in this VertexFormat.
     */
    unsigned int getElementCount() const;

    /**
     * Returns the size of a single vertex using this format (in bytes).
     */
    unsigned int getVertexSize() const;

private:

    /**
     * Constructor.
     */
    VertexFormat();

    /**
     * Destructor.
     */
    ~VertexFormat();

    Element* _elements;
    unsigned int _elementCount;
    unsigned int _vertexSize;
};

}

#endif
