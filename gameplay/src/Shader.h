#pragma once

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines a shader (module/function) for use in
 * a shader stage in a Pipeline.
 */
class Shader
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    Shader();

    /**
     * Destructor.
     */
    ~Shader();

private:

    VkShaderModule _shaderModule;
};

}