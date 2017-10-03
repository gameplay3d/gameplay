#pragma once

#include "Buffer.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Buffer.
 */
class BufferMTL : public Buffer
{
public:

	/**
	 * Constructor.
	 */
    BufferMTL();

	/**
	 * Constructor.
	 */
	BufferMTL(Usage usage, size_t size, size_t stride, bool hostVisible);

	/**
	 * Destructor.
	 */
	~BufferMTL();

	/**
	 * @see Buffer::getHostMemory
	 */
	void* getHostMemory() const;


	void* _hostMemory;

};

}
