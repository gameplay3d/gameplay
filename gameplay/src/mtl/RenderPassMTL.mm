#include "Base.h"
#include "RenderPassMTL.h"

namespace gameplay
{

RenderPassMTL::RenderPassMTL() :
	RenderPass(),
	_device(nullptr),
    _renderPassDescriptor(nullptr)
{
}

RenderPassMTL::RenderPassMTL(id<MTLDevice> device, MTLRenderPassDescriptor* renderPassDescriptor) :
	RenderPass(),
	_device(device),
	_renderPassDescriptor(renderPassDescriptor)
{
}

RenderPassMTL::~RenderPassMTL()
{
}

}
