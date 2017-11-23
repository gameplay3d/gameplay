#include "Base.h"
#include "SemaphoreVK.h"

namespace gameplay
{

SemaphoreVK::SemaphoreVK() :
	Semaphore(),
	_device(nullptr),
	_semaphore(nullptr)
{
}

SemaphoreVK::SemaphoreVK(VkDevice device, 
						 VkSemaphore semaphore) :
	Semaphore(),
	_device(device),
	_semaphore(semaphore)
{
}

SemaphoreVK::~SemaphoreVK()
{
}

}