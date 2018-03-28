#pragma once

#include "Renderer.h"

namespace gameplay
{

/**
 * Defines a model renderer.
 */
class Mesh : public Renderer
{
    friend class Graphics;
    friend class Activator;
    friend class SceneObject;

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
     * @see Component::getClassType
     */
    Component::ClassType getClassType();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();
    
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
