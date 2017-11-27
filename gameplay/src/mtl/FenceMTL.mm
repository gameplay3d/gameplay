#include "Base.h"
#include "FenceMTL.h"

namespace gameplay
{

FenceMTL::FenceMTL() :
	Fence(),
	_device(nullptr),
    _fence(nullptr)
{
}

FenceMTL::FenceMTL(id<MTLDevice> device, id<MTLFence> fence) :
	Fence(),
	_device(device),
	_fence(fence)
{
}

FenceMTL::~FenceMTL()
{
}

}
