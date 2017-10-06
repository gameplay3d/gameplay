#pragma once

namespace gameplay
{

/**
 * Defines an allocation of unformatted memory 
 * that can contain any type of data. 
 *
 * Buffers are often used for vertex, shader 
 * and compute state data.
 */
class Buffer
{
public:

	/**
	 * Defines the usage of Buffer.
	 */
	enum Usage
	{
		USAGE_VERTEX,
		USAGE_INDEX,
		USAGE_UNIFORM
	};

	/**
	 * Constructor.
	 */
	Buffer();

	/**
	 * Constructor.
	 */
	Buffer(Usage usage, size_t size, size_t stride, bool hostVisible);

	/**
	 * Destructor.
	 */
	~Buffer();

	/**
	 * Gets the usage of buffer.
	 *
	 * @return The usage of buffer.
	 */
	Usage getUsage() const;

	/**
	 * Gets the buffer size (in bytes).
	 *
	 * @return the buffer size (in bytes).
	 */
	size_t getSize() const;

	/**
	 * Gets the stride of one element in the buffer.
	 */
	size_t getStride() const;

	/**
	 * Deterines if the buffer has visible access to host memory.
	 *
	 * @return true if the buffer has visible access to host memory, false if static
	 */
	bool isHostVisible() const;

	/**
	 * Gets the pointer to the mapped host memory.
	 *
	 * @return The pointer to the mapped host memory.
	 */
	virtual void* getHostMemory() const = 0;

protected:

	Usage _usage;
	size_t _size;
	size_t _stride;
	bool _hostVisible;
};

}