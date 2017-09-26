#pragma once

#include "Mesh.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Mesh.
 */
class MeshVK : public Mesh
{
public:

	/**
	 * Vulkan impl of a Mesh::Part.
	 */
	class PartVK : public Part
	{
	public:

		/**
		 * Constructor.
		 */
		PartVK();

		/**
		 * Constructor.
		 */
		PartVK(std::shared_ptr<Mesh> mesh,
			   Graphics::PrimitiveTopology primitiveTopology, 
			   Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic, 
			   VkBuffer indexBuffer);

		/**
		 * Destructor.
		 */
		~PartVK();

		/**
		 * @see Mesh::Part::mapIndexBuffer
		 */
		void* mapIndexBuffer(size_t rangeStart, size_t rangeSize);

		/**
		 * @see Mesh::Part::setIndexData
		 */
		void setIndexData(void* indexData);

		/**
	     * @see Mesh::Part::unmapIndexBuffer
		 */
		bool unmapIndexBuffer();

		VkBuffer _indexBuffer;
	};

	/**
	 * Constructor.
	 */
	MeshVK();

	/**
	 * Constructor.
	 */
	MeshVK(const VertexFormat& vertexFormat, size_t vertexCount, bool dynamic,
		   VkDevice device, 
		   VkBuffer vertexBuffer);
	/**
	 * Destructor.
	 */
	~MeshVK();

	/**
	 * @see Mesh::setVertexData
	 */
	void* mapVertexBuffer(size_t rangeStart, size_t rangeSize);

	/**
	 * @see Mesh::setVertexData
	 */
	void setVertexData(void* indexData);

	/**
	 * @see Mesh::unmapVertexBuffer
	 */
	bool unmapVertexBuffer();

	/**
	 * @see Mesh::addPart
	 */
	Mesh::Part* addPart(Graphics::PrimitiveTopology primitiveTopology, Graphics::IndexFormat indexFormat, size_t indexCount, bool dynamic);

	VkDevice _device;
	VkBuffer _vertexBuffer;
};

}