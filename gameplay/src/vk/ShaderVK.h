#pragma once

#include "Shader.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a Shader.
 */
class ShaderVK : public Shader
{
public:

    /**
     * Constructor.
     */
	ShaderVK();

    /**
     * Constructor.
     */
    ShaderVK(VkDevice device, VkShaderModule shader);

    /**
	 * Destructor.
     */
    ~ShaderVK();

	VkDevice _device;
	VkShaderModule _shader;
};

}