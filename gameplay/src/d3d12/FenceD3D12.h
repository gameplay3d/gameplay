#pragma once

#include "Fence.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Fence.
 */
class FenceD3D12 : public Fence
{
public:

    /**
     * Constructor.
     */
	FenceD3D12();

    /**
     * Constructor.
     */
    FenceD3D12(ID3D12Device* device, ID3D12Fence* fence, HANDLE fenceEvent);

    /**
	 * Destructor.
     */
    ~FenceD3D12();

	ID3D12Device* _device;
	ID3D12Fence* _fence;
	HANDLE _fenceEvent;
	uint64_t _fenceValue;
};

}