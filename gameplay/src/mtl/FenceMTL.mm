#include "Base.h"
#include "FenceMTL.h"

namespace gameplay
{

FenceMTL::FenceMTL() :
	Shader(),
	_device(nullptr),
    _fence(nullptr)
{
}

FenceMTL::FenceMTL(id<MTLDevice> device, id<MTLFence> fence) :
	Shader(),
	_device(device),
	_fence(fence)
{
}

FenceMTL::~FenceMTL()
{
}

}