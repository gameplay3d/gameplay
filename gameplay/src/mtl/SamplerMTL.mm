#include "Base.h"
#include "SamplerStateMTL.h"

namespace gameplay
{

SamplerStateMTL::SamplerStateMTL() :
	Sampler(),
	_device(nullptr),
    _samplerState(nullptr)
{
}

SamplerStateMTL::SamplerStateMTL(id<MTLDevice> device, id<MTLSamplerState> samplerState) :
	Sampler(),
	_device(device),
	_samplerState(samplerState)
{
}

SamplerStateMTL::~SamplerStateMTL()
{
}

}