#pragma once

#include "Fence.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Fence.
 */
class FenceVK : public Fence
{
public:

    /**
     * Constructor.
     */
	FenceVK();

    /**
     * Constructor.
     */
    FenceVK(VkDevice device, VkFence fence);

    /**
	 * Destructor.
     */
    ~FenceVK();

	VkDevice _device;
    VkFence _fence;
};

}