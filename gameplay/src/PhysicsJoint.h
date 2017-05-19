#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract physics joint.
 */
class PhysicsJoint : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    PhysicsJoint();

    /**
     * Destructor
     */
    ~PhysicsJoint();
};

}
