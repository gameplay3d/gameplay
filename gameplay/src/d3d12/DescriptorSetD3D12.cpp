#include "Base.h"
#include "DescriptorSetD3D12.h"

namespace gameplay
{

DescriptorSetD3D12::DescriptorSetD3D12() :
	DescriptorSet(),
	_cbvSrvUavHeap(nullptr),
	_samplerHeap(nullptr)
{
}

DescriptorSetD3D12::DescriptorSetD3D12(const Descriptor* descriptors, 
									   size_t descriptorCount,
									   ID3D12Device* device,
                                       ID3D12DescriptorHeap* cbvSrvUavHeap,
                                       ID3D12DescriptorHeap* samplerHeap) :
	DescriptorSet(descriptors, descriptorCount),
	_cbvSrvUavHeap(cbvSrvUavHeap),
	_samplerHeap(samplerHeap)
{
	_bindings.resize(descriptorCount);
}

DescriptorSetD3D12::~DescriptorSetD3D12()
{
}

}