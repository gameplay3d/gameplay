#include "Base.h"
#include "RenderPipelineVK.h"

namespace gameplay
{

RenderPipelineVK::RenderPipelineVK() :
	RenderPipeline(),
	_device(nullptr),
    _pipelineLayout(nullptr),
    _pipeline(nullptr)
{
}

RenderPipelineVK::RenderPipelineVK(PrimitiveTopology primitiveTopology,
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
								   VkPipeline pipeline) :
	RenderPipeline(primitiveTopology,
				   vertexLayout,
				   rasterizerState,
				   colorBlendState,
				   depthStencilState,
				   renderPass,
				   descriptorSet,
				   vertShader,
				   tescShader,
				   teseShader,
				   geomShader,
				   fragShader),
	_device(device),
    _pipelineLayout(pipelineLayout),
    _pipeline(pipeline)
{
}

RenderPipelineVK::~RenderPipelineVK()
{
}

}