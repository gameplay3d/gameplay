#pragma once

namespace gameplay
{

/**
 * Defines a command buffer for storing 
 * recorded gpu commands.
 */ 
class CommandBuffer
{
public:	

	/**
	 * The type of commands.
	 */
	enum Type
	{
		TYPE_RENDER,
		TYPE_COMPUTE,
		TYPE_COPY
	};

	/**
	 * Constructor.
	 */
	CommandBuffer();

	/**
	 * Destructor.
	 */
	~CommandBuffer();

protected:

	Type _type;
};

}