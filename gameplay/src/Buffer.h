#pragma once

namespace gameplay
{

/**
 * Defines a graphics buffer used for rendering.
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
	Buffer(Usage usage, size_t size, size_t stride, bool hostVisible, void* hostMemory = nullptr);

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
	 * Gets the pointer to the host memory.
	 *
	 * @return The pointto the host memory.
	 */
	void* getHostMemory() const;

protected:

	Usage _usage;
	size_t _size;
	size_t _stride;
	bool _hostVisible;
	void* _hostMemory;
};

}