#pragma once

#include "Renderer.h"

namespace gameplay
{

/**
 * Defines a text renderer.
 */
class Tileset : public Renderer
{
    friend class Graphics;
    friend class SceneObject;
    friend class Activator;

public:

    /**
     * Constructor.
     */
    Tileset();
    
    /**
     * Destructor.
     */
    ~Tileset();

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
