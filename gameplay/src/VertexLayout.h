#pragma once

namespace gameplay
{
	enum class Format;

/**
 * Defines a vertex layout.
 */
class VertexLayout
{
public:

    /**
     * Defines a set of usages for vertex attributes.
     */
    enum Semantic
    {
        SEMANTIC_POSITION,
        SEMANTIC_NORMAL,
        SEMANTIC_COLOR,
        SEMANTIC_TANGENT,
        SEMANTIC_BINORMAL,
        SEMANTIC_TEXCOORD0,
        SEMANTIC_TEXCOORD1,
        SEMANTIC_TEXCOORD2,
        SEMANTIC_TEXCOORD3,
        SEMANTIC_TEXCOORD4,
        SEMANTIC_TEXCOORD5,
        SEMANTIC_TEXCOORD6,
        SEMANTIC_TEXCOORD7
    };

    /**
     * Defines a single vertex attributes a within a vertex layout.     
     */
    class VertexAttribute
    {
	public:

        Semantic semantic;
        std::string name;
		Format format;
		uint32_t binding;
		uint32_t location;
		uint32_t offset;

        /**
         * Constructor.
         */
        VertexAttribute();

        /**
         * Constructor.
         *
         * @param semantic The semantic.
         * @param name The name attribute.
		 * @param format The format of the attribute
         */
        VertexAttribute(Semantic semantic, 
						const std::string& name, 
						Format format,
						uint32_t binding,
						uint32_t location,
						uint32_t offset);

        /**
         * Compares two vertex attributes for equality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute matches the specified one, false otherwise.
         */
        bool operator == (const VertexAttribute& attr) const;

        /**
         * Compares to vertex attributes for inequality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute does not match the specified one, false otherwise.
         */
        bool operator != (const VertexAttribute& attr) const;
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
    VertexLayout(const VertexAttribute* attributes, size_t count);

    /**
     * Destructor.
     */
    ~VertexLayout();

    /**
     * Gets the vertex attribute at the specified index.
     *
     * @param index The index of the attribute to retrieve.
     */
    const VertexAttribute& getAttribute(size_t index) const;

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

    /**
     * Gets the string representation of a Semantic enumeration value.
     */
    static std::string toString(Semantic semantic);

private:

    std::vector<VertexAttribute> _attributes;
    size_t _stride;
};

}
