#pragma once

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines a synchronization primitive that can be used 
 * to insert a dependency between batches submitted to queues.  
 */
class Semaphore
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    Semaphore();

    /**
     * Destructor.
     */
    ~Semaphore();

private:

    VkSemaphore _semaphore;
};

}