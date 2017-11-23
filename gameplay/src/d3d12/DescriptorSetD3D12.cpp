#include "Base.h"
#include "DescriptorSetD3D12.h"

namespace gameplay
{

DescriptorSetD3D12::DescriptorSetD3D12() :
	DescriptorSet(),
	_cbvSrvUav(nullptr),
	_samplerHeap(nullptr)
{
}

DescriptorSetD3D12::DescriptorSetD3D12(const Descriptor* descriptors, 
									   size_t descriptorCount,
									   ID3D12Device* device,
                                       ID3D12DescriptorHeap* cbvSrvUav,
                                       ID3D12DescriptorHeap* samplerHeap) :
	DescriptorSet(descriptors, descriptorCount),
	_cbvSrvUav(cbvSrvUav),
	_samplerHeap(samplerHeap)
{
}

DescriptorSetD3D12::~DescriptorSetD3D12()
{
}

}