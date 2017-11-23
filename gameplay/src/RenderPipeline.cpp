#include "Base.h"
#include "RenderPipeline.h"

namespace gameplay
{

RenderPipeline::RenderPipeline()
{
}

RenderPipeline::RenderPipeline(PrimitiveTopology primitiveTopology,
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
							   std::shared_ptr<Shader> fragShader) :
	_primitiveTopology(primitiveTopology),
	_vertexLayout(vertexLayout),
	_rasterizerState(rasterizerState),
	_colorBlendState(colorBlendState),
	_depthStencilState(depthStencilState),
	_renderPass(renderPass),
	_descriptorSet(descriptorSet),
	_vertShader(vertShader),
	_tescShader(tescShader),
	_geomShader(geomShader),
	_fragShader(fragShader)
{
}

RenderPipeline::~RenderPipeline()
{
}

}