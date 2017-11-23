#pragma once

#include "DescriptorSet.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a DescriptorSet.
 */
class DescriptorSetD3D12 : public DescriptorSet
{
public:

	/**
	 * Additional heap and root signature binding info.
	 */
	struct DescriptorBindingInfo
	{
		unsigned int heapOffset;
		unsigned int rootParameterIndex;
	};

    /**
     * Constructor.
     */
    DescriptorSetD3D12();

    /**
     * Constructor.
     */
    DescriptorSetD3D12(const Descriptor* descriptors, 
					   size_t descriptorCount,
					   ID3D12Device* device,
                       ID3D12DescriptorHeap* cbvSrvUav,
                       ID3D12DescriptorHeap* samplerHeap);

    /**
     * Destructor.
     */
    ~DescriptorSetD3D12();

    ID3D12Device* _device;
    ID3D12DescriptorHeap* _cbvSrvUav;
    ID3D12DescriptorHeap* _samplerHeap;
	std::vector<DescriptorBindingInfo> _bindingInfo;
};

}