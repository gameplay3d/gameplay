#pragma once

namespace gameplay
{

/**
 * Defines a semaphore synchronization primitive.
 *
 * A semaphore is used to insert a dependency on 
 * batches of commands submitted to a queue.
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