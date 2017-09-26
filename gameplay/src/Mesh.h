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
	friend class SceneObject;
	friend class Serializer::Activator;

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
	 * Loads a mesh using data from any supported common interchange format:
	 * http://assimp.sourceforge.net/main_features_formats.html
	 *
	 * @return Loaded mesh.
	 */
	void load(const std::string& url);
	
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