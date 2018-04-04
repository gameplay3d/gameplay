#pragma once

#include "Component.h"
#include "Physics.h"

namespace gameplay
{

/**
 * Defines a physics cloth for the simulation of fabrics.
 */
class PhysicsCloth : public Component
{
    friend class Physics;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Constructor.
     */
    PhysicsCloth();
    
    /**
     * Destructor.
     */
    ~PhysicsCloth();

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
