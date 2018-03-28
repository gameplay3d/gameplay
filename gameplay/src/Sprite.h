#pragma once

#include "Renderer.h"

namespace gameplay
{

/**
 * Defines a sprite renderer.
 */
class Sprite : public Renderer
{
    friend class Graphics;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Constructor.
     */
    Sprite();
    
    /**
     * Destructor.
     */
    ~Sprite();

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
