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
