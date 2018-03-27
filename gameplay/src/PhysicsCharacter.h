#pragma once

#include "Component.h"
#include "Physics.h"

namespace gameplay
{

/**
 * Defines a physics character which can be controlled.
 */
class PhysicsCharacter : public Component
{
    friend class Physics;

public:

    /**
     * Constructor.
     */
    PhysicsCharacter();
    
    /**
     * Destructor.
     */
    ~PhysicsCharacter();

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
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();
};

}
