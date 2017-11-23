#pragma once

#include "Semaphore.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Semaphore.
 */
class SemaphoreMTL : public Semaphore
{
public:

    /**
     * Constructor.
     */
	SemaphoreMTL();

    /**
     * Constructor.
     */
    SemaphoreMTL(dispatch_semaphore_t semaphore);

    /**
	 * Destructor.
     */
    ~SemaphoreMTL();

	dispatch_semaphore_t _semaphore;
};

}