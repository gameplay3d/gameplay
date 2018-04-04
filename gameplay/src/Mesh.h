#pragma once

#include "Serializable.h"

namespace gameplay
{

/**
 * Defines a mesh.
 */
class Mesh : public Serializable
{
    friend class Graphics;
    friend class Activator;

public:
    /**
     * Constructor.
     */
    Mesh();
    
    /**
     * Destructor.
     */
    ~Mesh();

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

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
