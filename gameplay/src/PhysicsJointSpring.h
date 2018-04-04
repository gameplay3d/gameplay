#pragma once

#include "PhysicsJoint.h"

namespace gameplay
{

/**
 * Defines a generic spring joint between two
 * rigid bodies (or one rigid body and the world)
 * where the spring strength and dampening can be set
 * to control the distance two object are from one
 * anther.
 */
class PhysicsJointSpring : public PhysicsJoint
{
    friend class Physics;
    friend class SceneObject;
    friend class Activator;

public:
  
    /**
     * Constructor.
     */
    PhysicsJointSpring();
    
    /**
     * Destructor.
     */
    ~PhysicsJointSpring();

    /**
     * @see PhysicsJoint::getType
     */
    PhysicsJoint::Type getType() const;

    /**
     * Gets the current distance of the joint. 
     *
     * @return The current distance of the joint.  
     */
    float getDistance() const;

    /**
     * Sets the allowed minimum distance for the joint.
     *
     * @param distanceMin The allowed minimum distance for the joint. 
     */
    void setDistanceMin(float distanceMin);

    /**
     * Gets the allowed minimum distance for the joint.
     *
     * @return The allowed minimum distance for the joint. 
     */
    float getDistanceMin() const; 

    /**
     * Sets the allowed maximum distance for the joint.
     *
     * @param distanceMax The allowed maximum distance for the joint. 
     */
    void setDistanceMax(float distanceMax);

    /**
     * Gets the allowed maximum distance for the joint.
     *
     * @return The allowed maximum distance for the joint. 
     */
    float getDistanceMax() const;   

    /**
     * Sets the error tolerance for the joint.
     *
     * @param tolerance The error tolerance for the joint. 
     */
    void setTolerance(float tolerance);

    /**
     * Gets the error tolerance for the joint.
     *
     * @return The error tolerance for the joint. 
     */
    float getTolerance() const;

    /**
     * Sets the stiffening for the spring joint.
     *
     * @param stiffness The stiffening for the spring joint. 
     */
    void setStiffness(float stiffness);

    /**
     * Gets the stiffening for the spring joint.
     *
     * @return The stiffening for the spring joint. 
     */
    float getStiffness() const;

    /**
     * Sets the dampening for the spring joint.
     *
     * @param dampening The dampening for the spring joint. 
     */
    void setDampening(float dampening);

    /**
     * Gets the dampening for the spring joint.
     *
     * @return The dampening for the spring joint. 
     */
    float getDampening() const;

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
};

}
