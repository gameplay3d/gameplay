#pragma once

#include "Serializable.h"

namespace gameplay
{

/**
 * Defines a component
 */
class Component : public std::enable_shared_from_this<Component>, public Serializable
{
    friend class SceneObject;
    friend class Serializer::Activator;

public:
    /**
     * Constructor
     */
    Component();

    /**
     * Destructor.
     */
    ~Component();

    /**
     * Gets the scene object this component is attached to.
     *
     * @return The scene object this component is attached to.
     */
    std::shared_ptr<SceneObject> getObject() const;

    /**
     * @see Serializable::getClassName
     */
    virtual std::string getClassName() = 0;

    /**
     * @see Serializable::onSerialize
     */
    virtual void onSerialize(Serializer* serializer) = 0;

    /**
     * @see Serializable::onDeserialize
     */
    virtual void onDeserialize(Serializer* serializer) = 0;

protected:
    /**
     * Sets the scene object this component is attached to.
     *
     * @param object The objet this component is attached to.
     */
    virtual void setObject(std::shared_ptr<SceneObject> object);

    std::shared_ptr<SceneObject> _object;

};

}
