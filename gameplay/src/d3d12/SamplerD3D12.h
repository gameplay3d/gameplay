#pragma once

#include "Sampler.h"
#include <dxgi1_5.h>
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Sampler.
 */
class SamplerD3D12 : public Sampler
{
public:

    /**
     * Constructor.
     */
	SamplerD3D12();

    /**
     * Constructor.
     */
    SamplerD3D12(Filter filterMag,
				 Filter filterMin,
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
				 D3D12_SAMPLER_DESC samplerDesc);

    /**
	 * Destructor.
     */
    ~SamplerD3D12();

	ID3D12Device* _device;
	D3D12_SAMPLER_DESC _samplerDesc;
};

}