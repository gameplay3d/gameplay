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
    BufferMTL(Usage usage, size_t size, size_t stride, bool hostVisible,
              id<MTLDevice> device,
              id<MTLBuffer> buffer);

    /**
     * Destructor.
     */
    ~BufferMTL();

    /**
     * @see Buffer::getHostMemory
     */
    void* getHostMemory() const;


    id<MTLDevice> _device;
    id<MTLBuffer> _buffer;
    void* _hostMemory;

};

}
