#pragma once

#include "Component.h"
#include "Vector3.h"

namespace gameplay
{
    class Heightfield;
    class Mesh;
    class PhysicsMaterial;

/**
 * Defines the shape for a physics body allowing it to
 * participate in collisions.P
 */
class PhysicsCollider : public Component
{
    friend class Physics;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Defines the type of geometry.
     */
    enum class Type
    {
        eBox,
        eSphere,
        eCapsule,
        eMesh,
        eHeightfield
    };

    /**
     * Constructor.
     */
    PhysicsCollider();

    /**
     * Destructor.
     */
    ~PhysicsCollider();

    /**
     * Defines box geometry for a physics box.
     */
    struct Box
    {
        /**
         * The center of the box.
         *
         */
        Vector3 center = Vector3::zero();

        /**
         * The extents of the box.
         */
        Vector3 extents = Vector3::zero();
    };

    /**
     * Defines sphere geometry for a physics sphere.
     */
    struct Sphere
    {
        /**
         * The center of the physics sphere.
         */
        Vector3 center = Vector3::zero();

        /**
         * The radius of the sphere.
         */
        float radius = 0.0f;
    };

    /**
     * Defines capsule geometry for a collision shape.
     */
    struct Capsule
    {
        /**
         * Defines the direction of a capsule.
         */
        enum class Direction
        {
            eX,
            eY,
            eZ
        };

        /**
         * The center of the capsule.
         */
        Vector3 center = Vector3::zero();

        /**
         * The radius of the capsule.
         */
        float radius = 0.0f;

        /**
         * The height of the capsule.
         */
        float height = 0.0f;

        /**
         * The direction of the capsule.
         */
        Direction direction = Direction::eY;
    };

    /**
     * Gets the type of geometry used to represent the collision shape.
     *
     * @return The type of geometry used to represent the collision shape.
     */
    PhysicsCollider::Type getType() const;

    /**
     * Sets a physics material for this shape.
     *
     * This determines how this shape interacts with others.
     *
     * @param material The physics material for this shape.
     */
    void setMaterial(std::shared_ptr<PhysicsMaterial> material);

    /**
     * Gets the physics material for this shape.
     *
     * This determines how this shape interacts with others.
     *
     * @return The physics material for this shape.
     */
    std::shared_ptr<PhysicsMaterial> getMaterial() const;

    /**
     * Sets if this box acts only in only triggering
     * collision events and ignores the physics system.
     * 
     * @param trigger true if this is a trigger, false if not.
     */
    void setTrigger(bool trigger);

    /**
     * Determines if this box acts only in only triggering
     * collision events and ignores the physics system.
     *
     * @return true if this is a trigger, false if not.
     */
    bool isTrigger() const;

protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();
    
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);     

private:

    PhysicsCollider::Type _type;

};

}
