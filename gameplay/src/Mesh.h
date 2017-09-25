#pragma once

#include "Graphics.h"
#include "VertexFormat.h"
#include "Buffer.h"

namespace gameplay
{

/**
 * Defines a mesh supporting various vertex formats and 1 or more
 * mesh parts to define how the vertices are connected.
 */
class Mesh
{
public:

	/**
	 * Defines a part of a mesh describing the way the
	 * mesh's vertices are connected/indexed together.
	 */
	class Part
	{
	public:

		/**
		 * Constructor.
		 */
		Part();

		/**
		 * Constructor.
		 */
		~Part();

        /**
         * Gets the primitive topology to define how the indices are connected.
         *
         * @return The primitive topology to define how the indices are connected.
         */
		Graphics::PrimitiveTopology getPrimitiveTopology() const;

		/**
		 * Gets the format of indices in the part.
		 *
		 * @return The format of indices in the part.
		 */
		Graphics::IndexFormat getIndexFormat() const;

		/**
		 * Gets the number of indices in the part.
		 *
		 * @return The number of indices in the part.
		 */
		size_t getIndexCount() const;

		/**
		 * Determines if the indices are dynamic.
		 *
		 * @return true if the part is dynamic, false otherwise.
		 */
		bool isDynamic() const;

		/**
		 * Sets the specified index data into the mapped index buffer.
		 *
		 * @param indexData The index data to be set.
		 * @param indexStart The index to start from.
		 * @param indexCount The number of indices to be set.
		 */
		void setIndexData(const void* indexData, size_t indexStart, size_t indexCount);

	protected:

		Graphics::PrimitiveTopology _primitiveTopology;
		Graphics::IndexFormat _indexFormat;
		size_t _indexCount;
		bool _dynamic;
		std::shared_ptr<Buffer> _indexBuffer;
	};

	/**
	 * Constructor.
	 */
    Mesh();
    
	/**
	 * Destructor.
	 */
    ~Mesh();

   /**
     * Gets the vertex format for the mesh.
     *
     * @return The vertex format.
     */
    const VertexFormat& getVertexFormat() const;

    /**
     * Gets the number of vertices in the mesh.
     *
     * @return The number of vertices in the mesh.
     */
    size_t getVertexCount() const;

    /**
     * Gets the stride of a single vertex in the mesh.
     *
     * @return The size of 1 vertex in the mesh.
     */
    size_t getVertexStride() const;

    /**
     * Determines if the mesh is dynamic.
     *
     * @return true if the mesh is dynamic, false otherwise.
     */
    bool isDynamic() const;

    /**
     * Gets the primitive topology of the vertices in the mesh.
     *
     * The default primitive topology for a Mesh is PRIMITIVE_TOPOLOGY_TRIANGLES.
     * 
     * @return The primitive topology of the vertices in the mesh.
     */
    Graphics::PrimitiveTopology getPrimitiveTopology() const;

    /**
     * Sets the primitive topology of the vertices in the mesh.
     *
     * The primitive type set for a Mesh is only meaningful for meshes that do not
     * have any mesh parts. When there are no mesh parts associated with a mesh,
     * the Mesh is drawn as non-indexed geometry and the PrimitiveType of the Mesh
     * determines how the vertices are interpreted when drawn.
     *
     * @param primitiveTopology The primitive topology of the vertices in the mesh.
     */
    void setPrimitiveTopology(Graphics::PrimitiveTopology primitiveTopology);

    /**
     * Sets the specified vertex data into the mapped vertex buffer.
     *
     * @param vertexData The vertex data to be set.
     * @param vertexStart The index of the starting vertex (0 by default).
     * @param vertexCount The number of vertices to be set (default is 0, for all vertices).
     */
	void setVertexData(const void* vertexData, size_t vertexStart = 0, size_t vertexCount = 0);

    /**
     * Creates and adds a new part of primitive data defining how the vertices are connected.
     *
     * @param primitiveType The type of primitive data to connect the indices as.
     * @param indexFormat The format of the indices. SHORT or INT.
     * @param indexCount The number of indices to be contained in the part.
     * @param dynamic true if the index data is dynamic; false otherwise.
     * 
     * @return The newly created/added mesh part.
     */
    Part* addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic = false);

    /**
     * Gets the number of mesh parts contained within the mesh.
     *
     * @return The number of mesh parts contained within the mesh.
     */
	size_t getPartCount() const;

    /**
     * Gets a mesh part by index.
     * 
     * @param index The index of the mesh part to get.
     * @return The mesh part at the specified index.
     */
	Part* getPart(size_t index);

protected:

	Graphics::PrimitiveTopology _primitiveTopology;
    VertexFormat _vertexFormat;
    unsigned int _vertexCount;
	bool _dynamic;
	std::shared_ptr<Buffer> _vertexBuffer;
	std::vector<Part*> _parts;
};

}