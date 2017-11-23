#pragma once

#include "RenderPipeline.h"
#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Vulkan impl of a RenderPipeline.
 */
class RenderPipelineVK : public RenderPipeline
{
public:

    /**
     * Constructor.
     */
	RenderPipelineVK();

    /**
     * Constructor.
     */
    RenderPipelineVK(PrimitiveTopology primitiveTopology,
				     VertexLayout vertexLayout,
				     RasterizerState rasterizerState,
				     ColorBlendState colorBlendState,
				     DepthStencilState depthStencilState,
				     std::shared_ptr<RenderPass> renderPass,
				     std::shared_ptr<DescriptorSet> descriptorSet,
				     std::shared_ptr<Shader> vertShader,
				     std::shared_ptr<Shader> tescShader,
				     std::shared_ptr<Shader> teseShader,
				     std::shared_ptr<Shader> geomShader,
				     std::shared_ptr<Shader> fragShader,
					 VkDevice device, 
                     VkPipelineLayout pipelineLayout, 
                     VkPipeline pipeline);

    /**
	 * Destructor.
     */
    ~RenderPipelineVK();

	VkDevice _device;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _pipeline;
};

}