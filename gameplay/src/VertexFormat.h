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
         * The vertex element usage semantic.
         */
        Usage usage;

        /**
         * The number of values in the vertex element.
         */
        unsigned int size;

        /**
         * Constructor.
         */
        Element();

        /**
         * Constructor.
         *
         * @param usage The vertex element usage semantic.
         * @param size The number of float values in the vertex element.
         */
        Element(Usage usage, unsigned int size);
    };

    /**
     * Returns a unique VertexFormat for the request vertex element layout.
     *
     * @param elements The list of vertex elements defining the vertex format.
     * @param elementCount The number of items in the elements array.
     * 
     * @return A unique VertexFormat object.
     */
    static VertexFormat* create(const Element* elements, unsigned int elementCount);

    /**
     * Returns the vertex element at the specified index.
     *
     * index The index of the element to retreive.
     */
    const Element& getElement(unsigned int index) const;

    /**
     * Returns the number of elements in this VertexFormat.
     *
     * @return The number of items in the elements array.
     */
    unsigned int getElementCount() const;

    /**
     * Returns the size (in bytes) of a single vertex using this format.
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
