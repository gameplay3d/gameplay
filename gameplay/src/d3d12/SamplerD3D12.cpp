#include "Base.h"
#include "SamplerD3D12.h"

namespace gameplay
{

SamplerD3D12::SamplerD3D12() :
	Sampler(),
	_device(nullptr)
{
}

SamplerD3D12::SamplerD3D12(Filter filterMin,
						   Filter filterMag,
						   Filter filterMip,
						   AddressMode addressModeU,
						   AddressMode addressModeV,
						   AddressMode addressModeW,
						   BorderColor borderColor,
						   bool compareEnabled,
						   CompareFunc compareFunc,
						   bool anisotropyEnabled,
						   float anisotropyMax,
						   float lodMin,
						   float lodMax,
						   float lodMipBias,
						   ID3D12Device* device, 
						   D3D12_SAMPLER_DESC samplerDesc) :
	Sampler(filterMin,
		    filterMag,
			filterMip,
			addressModeU,
			addressModeV,
			addressModeW,
			borderColor,
			compareEnabled,
			compareFunc,
			anisotropyEnabled,
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