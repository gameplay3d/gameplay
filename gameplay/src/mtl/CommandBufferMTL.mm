#include "Base.h"
#include "CommandBufferMTL.h"

namespace gameplay
{

CommandBufferMTL::CommandBufferMTL() :
	CommandBuffer(),
	_device(nullptr),
    _commandBuffer(nullptr)
{
}

CommandBufferMTL::CommandBufferMTL(id<MTLDevice> device, id<MTLCommandBuffer> commandBuffer) :
	CommandBuffer(),
	_device(device),
	_commandBuffer(commandBuffer)
{
}

CommandBufferMTL::~CommandBufferMTL()
{
}

}