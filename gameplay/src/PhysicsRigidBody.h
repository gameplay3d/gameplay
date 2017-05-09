#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{


/**
 * Defines an physics rigid body.
 */
class PhysicsRigidBody : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    PhysicsRigidBody();

    /**
     * Destructor
     */
    ~PhysicsRigidBody();

};

}
