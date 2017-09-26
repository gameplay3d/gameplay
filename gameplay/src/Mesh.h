#pragma once

#include "Graphics.h"
#include "VertexFormat.h"

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
		Part(std::shared_ptr<Mesh> mesh,
			 Graphics::PrimitiveTopology primitiveTopology, 
			 Graphics::IndexFormat indexFormat, size_t _indexCount, bool dynamic);

		/**
		 * Constructor.
		 */
		~Part();

		/**
		 * Gets the mesh used for this part.
		 *
		 * @return The mesh used for this part.
		 */
		std::shared_ptr<Mesh> getMesh() const;

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
		 * Maps the gpu index buffer to host accessible memory.
		 *
		 * @param rangeStart The memory range to start to map.
		 * @param rangeSize The memory range size to map. 
		 */
		virtual void* mapIndexBuffer(size_t rangeStart, size_t rangeSize) = 0;

		/**
		 * Sets the specified index data into the mapped index buffer.
		 *
		 * @param indexData The index data to be set.
		 * @param indexStart The index to start from.
		 * @param indexCount The number of indices to be set.
		 */
		virtual void setIndexData(void* indexData) = 0;

		/**
		 * Unmaps the previously mapped range of memory.
		 *
		 * @return true if the memory was unmapped, false if failed to unmap.
		 */
		virtual bool unmapIndexBuffer() = 0;

	protected:

		std::shared_ptr<Mesh> _mesh;
		Graphics::PrimitiveTopology _primitiveTopology;
		Graphics::IndexFormat _indexFormat;
		size_t _indexCount;
		bool _dynamic;
	};

	/**
	 * Constructor.
	 */
    Mesh();

	/**
	 * Constructor.
	 */
    Mesh(const VertexFormat& vertexFormat, size_t vertexCount, bool _dynamic);
    
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
	 * Maps the gpu vertex buffer to host accessible memory.
	 *
	 * @param rangeStart The memory range to start to map.
	 * @param rangeSize The memory range size to map. 
	 */
	virtual void* mapVertexBuffer(size_t rangeStart, size_t rangeSize) = 0;

    /**
     * Sets (copies) the specified vertex data into the mapped vertex buffer.
     *
     * @param vertexData The vertex data to be set.
     * @param vertexStart The index of the starting vertex (0 by default).
     * @param vertexCount The number of vertices to be set (default is 0, for all vertices).
     */
	virtual void setVertexData(void* vertexData) = 0;

	/**
	 * Unmaps the previously mapped range of memory
	 *
	 * @return true if the memory was unmapped, false if failed to unmap.
	 */
	virtual bool unmapVertexBuffer() = 0;

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
    virtual Part* addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic = false) = 0;

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
    size_t _vertexCount;
	bool _dynamic;
	std::vector<Part*> _parts;
};

}