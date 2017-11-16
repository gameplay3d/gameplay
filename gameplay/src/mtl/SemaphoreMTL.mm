#include "Base.h"
#include "SemaphoreMTL.h"

namespace gameplay
{

SemaphoreMTL::SemaphoreMTL() :
	Semaphore(),
	_semaphore(nullptr)
{
}

SemaphoreMTL::SemaphoreMTL(dispatch_semaphore_t semaphore) :
	Semaphore(),
	_semaphore(semaphore)
{
}

SemaphoreMTL::~SemaphoreMTL()
{
}

}