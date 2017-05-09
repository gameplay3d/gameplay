#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract physics collider.
 */
class PhysicsCollider : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    PhysicsCollider();

    /**
     * Destructor
     */
    virtual ~PhysicsCollider();

};

}
