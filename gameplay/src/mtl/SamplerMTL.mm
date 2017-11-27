#include "Base.h"
#include "SamplerMTL.h"

namespace gameplay
{

SamplerMTL::SamplerMTL() :
	Sampler(),
	_device(nullptr),
    _samplerState(nullptr)
{
}

SamplerMTL::SamplerMTL(id<MTLDevice> device, id<MTLSamplerState> samplerState) :
	Sampler(),
	_device(device),
	_samplerState(samplerState)
{
}

SamplerMTL::~SamplerMTL()
{
}

}
