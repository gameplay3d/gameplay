#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract physics system.
 */
class Physics
{
public:
    /**
     * Gets the physics system.
     *
     * @return The physics system.
     */
    static Physics* getPhysics();

    /**
     * Called by platform or editor to initialize the physics system.
     *
     * Exits application if fails.
     *
     * @param window The native window object/handle.
     * @param connection The native connection to windowing system and application.
     */
    virtual void initialize(unsigned long window, unsigned long connection = 0) = 0;


    /**
     * Defines an physics joint connstrain between 2 rigid bodies.
     */
    class Joint
    {
    };

    /**
     * Defines the physical collision behaviour of a surface.
     */
    class Material
    {
    };

    /**
     * Defines the physical geometry.
     */
    class Geometry
    {
    };

    /**
     * Defines the physical collision behaviour of a surface.
     */
    class Shape
    {
    };

    /**
     * Defines an physics rigid body used for dynamics
     */
    class RigidBody : public Component
    {
        friend class SceneObject;
        //friend class Serializer::Activator;
    };

private:

    static Physics* _physics;
};

}
