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

RenderPipelineMTL::RenderPipelineMTL(id<MTLDevice> device, id<MTLRenderPipelineState> pipelineState) :
	Pipeline(),
	_device(device),
	_pipelineState(pipelineState)
{
}

RenderPipelineMTL::~RenderPipelineMTL()
{
}

}