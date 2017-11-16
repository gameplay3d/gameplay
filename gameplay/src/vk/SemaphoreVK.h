#pragma once

#include "Semaphore.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Semaphore.
 */
class SemaphoreVK : public Semaphore
{
public:

    /**
     * Constructor.
     */
	SemaphoreVK();

    /**
     * Constructor.
     */
    SemaphoreVK(VkDevice device, VkSemaphore semaphore);

    /**
	 * Destructor.
     */
    ~SemaphoreVK();

	VkDevice _device;
	VkSemaphore _semaphore;
};

}