#pragma once

#include "Component.h"
#include "Physics.h"

namespace gameplay
{

/**
 * Defines a physics simulation 
 */
class PhysicsVehicle : public Component
{
public:

    /**
     * Constructor.
     */
    PhysicsVehicle();
    
    /**
     * Destructor. 
     */
    ~PhysicsVehicle();

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Component::getClassType
     */
    Component::ClassType getClassType();

protected:

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
