#pragma once

#include "Semaphore.h"
#include <d3d12.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Semaphore.
 */
class SemaphoreD3D12 : public Semaphore
{
public:

    /**
     * Constructor.
     */
	SemaphoreD3D12();

    /**
     * Constructor.
     */
    SemaphoreD3D12(ID3D12Device* device, void* semaphore);

    /**
	 * Destructor.
     */
    ~SemaphoreD3D12();

	ID3D12Device* _device;
	void* _semaphore;
};

}