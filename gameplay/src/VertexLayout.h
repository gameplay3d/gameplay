#pragma once

#include "Format.h"

namespace gameplay
{

/**
 * Defines a layout of vertex attributes.
 */
class VertexLayout
{
public:

    /**
     * Defines semantics for vertex attributes.
     */
    enum class Semantic
    {
        ePosition,
        eNormal,
        eColor,
        eColor0,
        eColor1,
        eColor2,
        eColor3,
        eColor4,
        eTangent,
        eBitangent,
        eTexCoord,
        eTexCoord0,
        eTexCoord1,
        eTexCoord2,
        eTexCoord3,
        eTexCoord4,
        eTexCoord5,
        eTexCoord6,
        eTexCoord7
    };

    /**
     * Defines a single vertex attribute a within a vertex layout.     
     */
    class Attribute
    {
    public:

        Semantic semantic = Semantic::ePosition;
        Format format = Format::eUndefined;
        uint32_t binding = 0;
        uint32_t location = 0;
        uint32_t offset = 0;

        /**
         * Constructor.
         */
        Attribute();

        /**
         * Constructor.
         *
         * @param semantic The semantic.
         * @param format The format of the attribute
         * @param binding The attribute binding
         * @param location The attibute location
         * @param offset The attribute offset
         */
        Attribute(Semantic semantic,
                  Format format,
                  uint32_t binding,
                  uint32_t location,
                  uint32_t offset);

        /**
         * Destructor.
         */
        ~Attribute();

        /**
         * Compares two vertex attributes for equality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute matches the specified one, false otherwise.
         */
        bool operator == (const Attribute& attr) const;

        /**
         * Compares to vertex attributes for inequality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute does not match the specified one, false otherwise.
         */
        bool operator != (const Attribute& attr) const;
    };

    /**
     * Constructor.
     */
    VertexLayout();

    /**
     * Constructor.
     *
     * The passed in vertex attribute array is copied into the new VertexLayout.
     *
     * @param attributes The array of vertex attributes defining the vertex layout.
     * @param count The number of items in the attributes array.
     */
    VertexLayout(const Attribute* attributes, size_t count);

    /**
     * Destructor.
     */
    ~VertexLayout();

    /**
     * Gets the vertex attribute at the specified index.
     *
     * @param index The index of the attribute to retrieve.
     */
    const Attribute& getAttribute(size_t index) const;

    /**
     * Gets the number of vertex attributes in this VertexLayout.
     *
     * @return The number of attribute in the array.
     */
    size_t getAttributeCount() const;

    /**
     * Gets the stride (size in bytes) of the layout destribing a vertex.
     */
    size_t getStride() const;

    /**
     * Gets the vertex stride for a specified format.
     *
     * @param format The format to get the stride for.
     * @return The vertex stride for a specified format.
     */
    static size_t toStride(Format format);

    /**
     * Compares two vertex layouts for equality.
     *
     * @param layout The vertex layout to compare.
     * @return true if the attributes in this VertexLayout matches the specified one, false otherwise.
     */
    bool operator == (const VertexLayout& layout) const;

    /**
     * Compares to vertex layouts for inequality.
     *
     * @param layout The vertex layout to compare.
     * @return true if the attributes in this VertexLayout are not equal to the specified one, false otherwise.
     */
    bool operator != (const VertexLayout& layout) const;

private:

    std::vector<Attribute> _attributes;
    size_t _stride;
};

}
