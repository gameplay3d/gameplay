#pragma once

#include "Component.h"

namespace gameplay
{
	class Vector3;
	class Heightfield;
    class Mesh;
	class PhysicsMaterial;

/**
 * Defines the shape for a rigid body allowing it to 
 * participate in collisions.
 */
class PhysicsCollisionShape : public Component
{
    friend class Physics;
	friend class SceneObject;
	friend class Serializer::Activator;

public:

    /**
     * Defines the type of shape.
     */
    enum Type
    {
        TYPE_BOX,
        TYPE_SPHERE,
        TYPE_CAPSULE,
        TYPE_MESH,
        TYPE_HEIGHTFIELD
    };

    /**
     * Constructor.
     */
    PhysicsCollisionShape();

    /**
     * Destructor.
     */
    ~PhysicsCollisionShape();

    /**
     * Defines box geometry for a collision shape.
     */
    class Box
    {
    public:

        /**
         * Sets the center of the box.
         * 
         * @param center The center of the box.
         */
        void setCenter(const Vector3& center);

        /**
         * Gets the center of the box.
         * 
         * @return The center of the box.
         */
        Vector3 getCenter() const;

        /**
         * Sets the center of the box.
         * 
         * @param center The center of the box.
         */
        void setExtents(const Vector3& extents);

        /**
         * Gets the extents of the box.
         * 
         * @return The extents of the box.
         */
        Vector3 getExtents() const;
    };

    /**
     * Defines sphere geometry for a collision shape.
     */
    class Sphere
    {
    public:

        /**
         * Sets the center of the sphere.
         * 
         * @param center The center of the sphere.
         */
        void setCenter(const Vector3& center);

        /**
         * Gets the center of the sphere.
         * 
         * @return The center of the sphere.
         */
        Vector3 getCenter() const;

        /**
         * Sets the radius of the sphere.
         * 
         * @param radius The radius of the sphere.
         */
        void setRadius(float radius);

        /**
         * Gets the radius of the sphere.
         * 
         * @return The radius of the sphere.
         */
        float getRadius() const;
    };

    /**
     * Defines capsule geometry for a collision shape.
     */
    class Capsule
    {
    public:

        /**
         * Defines the direction of a capsule.
         */
        enum Direction
        {
            DIRECTION_X,
            DIRECTION_Y,
            DIRECTION_Z
        };

        /**
         * Sets the center of the capsule.
         * 
         * @param center The center of the capsule.
         */
        void setCenter(const Vector3& center);

        /**
         * Gets the center of the capsule.
         * 
         * @return The center of the capsule.
         */
        Vector3 getCenter() const;

        /**
         * Sets the radius of the capsule.
         * 
         * @param radius The radius of the capsule.
         */
        void setRadius(float radius);

        /**
         * Gets the radius of the capsule.
         * 
         * @return The radius of the capsule.
         */
        float getRadius() const;

        /**
         * Sets the height of the capsule.
         * 
         * @param height The height of the capsule.
         */
        void setHeight(float height);

        /**
         * Gets the height of the capsule.
         * 
         * @return The height of the capsule.
         */
        float getHeight() const;

        /**
         * Sets the direction of the capsule.
         * 
         * @param direction The direction of the capsule.
         */
        void setDirection(Direction direction);

        /**
         * Gets the direction of the capsule.
         * 
         * @return The direction of the capsule.
         */
        Direction getDirection() const;
    };

    /**
     * Gets the type of geometry used to represent the collision shape.
     *
     * @return The type of geometry used to represent the collision shape.
     */
    PhysicsCollisionShape::Type getType() const;

    /**
     * Gets the box geometry data if this shape is a box.
     *
     * @return The box geometry data if this shape is a box, otherwise nullptr.
     */
    std::shared_ptr<Box> getBoxGeometry() const;

    /**
     * Gets the sphere geometry data if this shape is a sphere.
     *
     * @return The sphere geometry data if this shape is a sphere, otherwise nullptr.
     */
    std::shared_ptr<Sphere> getSphereGeometry() const;

    /**
     * Gets the capsule geometry data if this shape is a sphere.
     *
     * @return The capsule geometry data if this shape is a capsule, otherwise nullptr.
     */
    std::shared_ptr<Capsule> getCapsuleGeometry() const;

    /**
     * Gets the heightfield geometry data if this shape is a heightfield.
     *
     * @return The heightfield geometry data if this shape is a heightfield, otherwise nullptr.
     */
    std::shared_ptr<Heightfield> getHeightfieldGeometry() const;

    /**
     * Gets the mesh geometry data if this shape is a mesh.
     *
     * @return The mesh geometry data if this shape is a mesh, otherwise nullptr.
     */
    std::shared_ptr<Mesh> getMeshGeometry() const;

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

    /**
     * @see Component::getTypeId
     */
    Component::TypeId getTypeId();

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
	
	/**
     * @see Serializer::Activator::createObject
	 */
    static std::shared_ptr<Serializable> createObject();

protected:

    PhysicsCollisionShape::Type _type;
    union
    {
        std::shared_ptr<Box> _box;
        std::shared_ptr<Sphere> _sphere;
        std::shared_ptr<Capsule> _capsule;
        std::shared_ptr<Heightfield> _heightfield;
        std::shared_ptr<Mesh> _mesh;
    };
};

}