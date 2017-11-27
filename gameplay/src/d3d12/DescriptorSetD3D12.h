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
	 * Additional descriptor heap and root signature binding info.
	 */
	class Binding
	{
	public:
		unsigned int rootParameterIndex;
		unsigned int heapOffset;
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
                       ID3D12DescriptorHeap* cbvSrvUavHeap,
                       ID3D12DescriptorHeap* samplerHeap);

    /**
     * Destructor.
     */
    ~DescriptorSetD3D12();

    ID3D12Device* _device;
    ID3D12DescriptorHeap* _cbvSrvUavHeap;
    ID3D12DescriptorHeap* _samplerHeap;
	std::vector<Binding> _bindings;
};

}