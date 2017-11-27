#include "Base.h"
#include "RenderPipelineMTL.h"

namespace gameplay
{

RenderPipelineMTL::RenderPipelineMTL() :
	RenderPipeline(),
	_device(nullptr),
    _pipelineState(nullptr)
{
}

RenderPipelineMTL::RenderPipelineMTL(PrimitiveTopology primitiveTopology,
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
                                     id<MTLDevice> device,
                                     id<MTLRenderPipelineState> pipelineState) :
	RenderPipeline(primitiveTopology, vertexLayout,
                   rasterizerState, colorBlendState, depthStencilState,
                   renderPass, descriptorSet,
                   vertShader, tescShader, teseShader, geomShader, fragShader),
	_device(device),
	_pipelineState(pipelineState)
{
}

RenderPipelineMTL::~RenderPipelineMTL()
{
}

}
