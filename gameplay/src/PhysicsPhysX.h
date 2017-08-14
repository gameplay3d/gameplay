#pragma once

#include "Physics.h"

namespace gameplay
{

/**
 * Defines an abstract physics system.
 */
class PhysicsPhysX : public Physics
{
public:

    /**
     * Constructor.
     */
    PhysicsPhysX();

    /**
     * Destructor.
     */
    ~PhysicsPhysX();

    /**
     * @see Physics::initialize
     */
    void initialize(unsigned long window, unsigned long connection = 0);
};

}
