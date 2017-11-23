#pragma once

namespace gameplay
{

/**
 * Defines a fence synchronization primitive.
 *
 * A fence is to insert a dependency on the 
 * processing queue and the host.
 */
class Fence
{
public:

	/**
	 * Constructor.
	 */
	Fence();

	/**
	 * Destructor.
	 */
	~Fence();
};

}