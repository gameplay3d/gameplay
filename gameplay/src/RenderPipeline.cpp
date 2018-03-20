#include "Base.h"
#include "RenderPipeline.h"

namespace gameplay
{

RenderPipeline::RenderPipeline() :
    _renderPass(nullptr),
    _descriptorSet(nullptr),
    _vertShader(nullptr),
    _tescShader(nullptr),
    _geomShader(nullptr),
    _fragShader(nullptr),
    _pipelineLayout(VK_NULL_HANDLE),
    _pipeline(VK_NULL_HANDLE)
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