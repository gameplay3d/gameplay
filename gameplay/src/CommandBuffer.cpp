#include "Base.h"
#include "CommandBuffer.h"

namespace gameplay
{

CommandBuffer::CommandBuffer() : 
    _commandBuffer(VK_NULL_HANDLE)
{
}
CommandBuffer::~CommandBuffer()
{
}

}