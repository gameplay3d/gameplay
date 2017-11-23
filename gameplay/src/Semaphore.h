#pragma once

namespace gameplay
{

/**
 * Defines a semaphore synchronization primitive.
 *
 * A semaphore is used to insert a dependency on 
 * batches of commands submitted.
 */
class Semaphore
{
public:

	/**
	 * Constructor.
	 */
	Semaphore();

	/**
	 * Destructor.
	 */
	~Semaphore();
};

}