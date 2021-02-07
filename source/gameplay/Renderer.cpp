#include "Renderer.h"
#include "WindowingGLFW.h"

namespace gameplay
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::startup()
{
    /*if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan not supported\n");
        return;
    }
    uint32_t extensionsCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
    */
}

void Renderer::shutdown()
{
}

}
