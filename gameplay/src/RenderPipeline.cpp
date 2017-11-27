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

RenderPipeline::PrimitiveTopology RenderPipeline::getPrimitiveTopology() const
{
	return _primitiveTopology;
}

VertexLayout RenderPipeline::getVertexLayout() const
{
	return _vertexLayout;
}

RasterizerState RenderPipeline::getRasterizerState() const
{
	return _rasterizerState;
}

ColorBlendState RenderPipeline::getColorBlendState() const
{
	return _colorBlendState;
}

DepthStencilState RenderPipeline::getDepthStencilState() const
{
	return _depthStencilState;
}

std::shared_ptr<RenderPass> RenderPipeline::getRenderPass() const
{
	return _renderPass;
}

std::shared_ptr<DescriptorSet> RenderPipeline::getDescriptorSet() const
{
	return _descriptorSet;
}

std::shared_ptr<Shader> RenderPipeline::getVertexShader() const
{
	return _vertShader;
}

std::shared_ptr<Shader> RenderPipeline::getTessellationControlShader() const
{
	return _tescShader;
}

std::shared_ptr<Shader> RenderPipeline::getTessellationEvaluationShader() const
{
	return _teseShader;
}

std::shared_ptr<Shader> RenderPipeline::getGeometryShader() const
{
	return _geomShader;
}

std::shared_ptr<Shader> RenderPipeline::getFragmentShader() const
{
	return _fragShader;
}

}