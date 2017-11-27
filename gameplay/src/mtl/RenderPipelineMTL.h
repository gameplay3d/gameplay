#pragma once

#include "RenderPipeline.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a RenderPipeline.
 */
class RenderPipelineMTL : public RenderPipeline
{
public:

    /**
     * Constructor.
     */
	RenderPipelineMTL();

    /**
     * Constructor.
     */
    RenderPipelineMTL(PrimitiveTopology primitiveTopology,
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
                      id<MTLRenderPipelineState> pipelineState);

    /**
	 * Destructor.
     */
    ~RenderPipelineMTL();

	id<MTLDevice>  _device;
    id<MTLRenderPipelineState> _pipelineState;
};

}
