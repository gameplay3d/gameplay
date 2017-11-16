#include "Base.h"
#include "SamplerD3D12.h"

namespace gameplay
{

SamplerD3D12::SamplerD3D12() :
	Sampler(),
	_device(nullptr)
{
}

SamplerD3D12::SamplerD3D12(Filter filterMag,
						   Filter filterMin,
						   Filter filterMip,
						   AddressMode addressModeU,
						   AddressMode addressModeV,
						   AddressMode addressModeW,
						   CompareFunc compareFunc,
						   BorderColor borderColor,
						   float anisotropyMax,
						   float lodMin,
						   float lodMax,
						   float lodMipBias,
						   ID3D12Device* device, 
						   D3D12_SAMPLER_DESC samplerDesc) :
	Sampler(filterMag,
			filterMin,
			filterMip,
			addressModeU,
			addressModeV,
			addressModeW,
			compareFunc,
			borderColor,
			anisotropyMax,
			lodMin,
			lodMax,
			lodMipBias),
	_device(device),
	_samplerDesc(samplerDesc)
{
}

SamplerD3D12::~SamplerD3D12()
{
}

}