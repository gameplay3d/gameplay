#pragma once

#include "Renderer.h"

namespace gameplay
{

/**
 * Defines a path renderer.
 */
class Path : public Renderer
{
	friend class Graphics;
	friend class SceneObject;
	friend class Serializer::Activator;

public:

	/**
	 * Constructor.
	 */
    Path();
    
	/**
	 * Destructor.
	 */
    ~Path();

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