#include "Base.h"
#include "CommandPoolMTL.h"

namespace gameplay
{

CommandPoolMTL::CommandPoolMTL() :
	CommandPool(),
	_device(nullptr)
{
}

CommandPoolMTL::CommandPoolMTL(id<MTLDevice> device) :
	CommandPool(),
	_device(device)
{
}

CommandPoolMTL::~CommandPoolMTL()
{
}

}
