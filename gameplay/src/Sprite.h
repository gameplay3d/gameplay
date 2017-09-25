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
	friend class SceneObject;
	friend class Serializer::Activator;

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
     * @see Serializer::Activator::createObject
	 */
    static std::shared_ptr<Serializable> createObject();
};

}