#pragma once

namespace gameplay
{

	class CommandBuffer;

/**
 * Defines a command pool for creating command buffers.
 */ 
class CommandPool
{
public:	

	/**
	 * Constructor.
	 */
	CommandPool();

	/**
	 * Destructor.
	 */
	~CommandPool();

	/**
	 * Creates a command buffer.
	 *
	 * @return The command buffer created.
	 */
	virtual std::shared_ptr<CommandBuffer> createCommandBuffer() = 0;

	/**
	 * Destroys a command buffer.
	 *
	 * @param commandBuffer The command buffer to be destroyed.
	 */
	virtual void destroyCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer) = 0;
};

}