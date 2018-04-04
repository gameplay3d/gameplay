#pragma once

#include "Renderer.h"

namespace gameplay
{

/**
 * Defines a model instance.
 */
class MeshInstance : public Renderer
{
    friend class Graphics;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Constructor.
     */
    MeshInstance();
    
    /**
     * Destructor.
     */
    ~MeshInstance();

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
