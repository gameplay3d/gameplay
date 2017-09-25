#pragma once

namespace gameplay
{

/**
 * Defines a graphics buffer used for rendering.
 *
 * 
 */
class Buffer
{
public:

	/**
	 * Defines the type of Buffer.
	 */
	enum Type
	{
		TYPE_VERTEX,
		TYPE_INDEX,
		TYPE_CONSTANT
	};

	/**
	 * Constructor.
	 */
	Buffer();

	/**
	 * Constructor.
	 */
	Buffer(Type type, size_t size, bool dynamic);

	/**
	 * Destructor.
	 */
	~Buffer();

	/**
	 * Gets the type of buffer.
	 *
	 * @return The type of buffer.
	 */
	Type getType() const;

	/**
	 * Gets the buffer size (in bytes).
	 *
	 * @return the buffer size (in bytes).
	 */
	size_t getSize() const;

	/**
	 * Deterines if the buffer is intended for dynamic updates the data.
	 *
	 * @return true if the buffer is intended for dynamic updates the data, false if static.
	 */
	bool isDynamic() const;

protected:

	Type _type;
	size_t _size;
	bool _dynamic;
};

}