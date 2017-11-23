#include "Base.h"
#include "SemaphoreD3D12.h"

namespace gameplay
{

SemaphoreD3D12::SemaphoreD3D12() :
	Semaphore(),
	_device(nullptr),
	_semaphore(nullptr)
{
}

SemaphoreD3D12::SemaphoreD3D12(ID3D12Device* device, void* semaphore) :
	Semaphore(),
	_device(device),
	_semaphore(semaphore)
{
}

SemaphoreD3D12::~SemaphoreD3D12()
{
}

}