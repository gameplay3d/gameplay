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
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Constructor.
     */
    Tileset();
    
    /**
     * Destructor.
     */
    ~Tileset();

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
