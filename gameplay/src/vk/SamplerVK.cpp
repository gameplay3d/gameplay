#include "Base.h"
#include "SamplerVK.h"

namespace gameplay
{

SamplerVK::SamplerVK() :
	Sampler(),
	_device(nullptr),
	_sampler(nullptr)
{
}

SamplerVK::SamplerVK(Filter filterMin,
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
					 VkDevice device, 
					 VkSampler sampler) :
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
	_sampler(sampler)
{
}

SamplerVK::~SamplerVK()
{
}

}