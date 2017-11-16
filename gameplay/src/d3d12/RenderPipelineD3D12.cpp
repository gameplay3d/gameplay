#include "Base.h"
#include "RenderPipelineD3D12.h"

namespace gameplay
{

RenderPipelineD3D12::RenderPipelineD3D12() :
	RenderPipeline(),
	_device(nullptr),
	_rootSignature(nullptr),
    _pipelineState(nullptr)
{
}

RenderPipelineD3D12::RenderPipelineD3D12(PrimitiveTopology primitiveTopology,
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
										 ID3D12Device* device, 
										 ID3D12RootSignature* rootSignature, 
										 ID3D12PipelineState* pipelineState) :
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
	_rootSignature(rootSignature),
	_pipelineState(pipelineState)
{
}

RenderPipelineD3D12::~RenderPipelineD3D12()
{
}

}