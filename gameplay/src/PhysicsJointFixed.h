#pragma once

#include "PhysicsJoint.h"

namespace gameplay
{

/**
 * Defines a joint where two rigid bodies 
 * (or one rigid body and the world) making them stick 
 * together in their bound position.
 *
 * This is similar in concept to parenting one object to another,
 * but can be used in specific situations for a more appropriate effect,
 * Ex. for implementing sticky projectiles, etc.
 */
class PhysicsJointFixed : public PhysicsJoint
{
    friend class Physics;
    friend class Activator;
    friend class SceneObject;

public:
  
    /**
     * Constructor.
     */
    PhysicsJointFixed();
    
    /**
     * Destructor.
     */
    ~PhysicsJointFixed();

    /**
     * @see PhysicsJoint::getType
     */
    PhysicsJoint::Type getType() const;

protected:

    /**
     * @see Serializer::Activator::createObject
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
