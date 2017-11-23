#pragma once

#include "RenderPipeline.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a RenderPipeline.
 */
class RenderPipelineD3D12 : public RenderPipeline
{
public:

    /**
     * Constructor.
     */
	RenderPipelineD3D12();

    /**
     * Constructor.
     */
    RenderPipelineD3D12(PrimitiveTopology primitiveTopology,
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
						ID3D12PipelineState* pipelineState);

    /**
	 * Destructor.
     */
    ~RenderPipelineD3D12();

	ID3D12Device* _device;
	ID3D12RootSignature*  _rootSignature;
    ID3D12PipelineState*  _pipelineState;
};

}