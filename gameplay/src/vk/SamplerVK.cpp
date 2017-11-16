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

SamplerVK::SamplerVK(Filter filterMag,
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
					 VkDevice device, 
                     VkSampler sampler) :
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
	_sampler(sampler)
{
}

SamplerVK::~SamplerVK()
{
}

}