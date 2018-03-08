#include "Base.h"
#include "Semaphore.h"

namespace gameplay
{

Semaphore::Semaphore() :
	_semaphore(VK_NULL_HANDLE)
{
}

Semaphore::~Semaphore()
{
}

}