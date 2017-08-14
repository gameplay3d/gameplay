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
     * Defines the component type identifier.
     */
    enum TypeId
    {
        TYPEID_SCRIPT,
        TYPEID_CAMERA,
        TYPEID_LIGHT,
        TYPEID_ANIMATION,
        TYPEID_AUDIO_SOURCE,
        TYPEID_PHYSICS_COLLIDER,
        TYPEID_PHYSICS_RIGIDBODY,
        TYPEID_PHYSICS_JOINT,
        TYPEID_RENDERER
    };

    /**
     * Constructor
     */
    Component();

    /**
     * Destructor.
     */
    ~Component();

    /**
     * This gets the type identifier for a component.
     *
     * @return the type id for a component.
     */
    virtual Component::TypeId getTypeId() = 0;

    /**
     * Determines if a component is enabled.
     *
     * @return  true if the component is active, false if not active.
     */
    bool isEnabled() const;

    /**
     * Sets if the component as enabled.
     *
     * @param  true if the component is active, false if not active.
     */
    void setEnabled(bool enabled);

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

    /**
     * Gets the scene object this component is attached to.
     *
     * @return The scene object this component is attached to.
     */
    std::shared_ptr<SceneObject> getObject() const;

protected:

    /**
     * Sets the scene object this component is attached to.
     *
     * @param object The objet this component is attached to.
     */
    virtual void setObject(std::shared_ptr<SceneObject> object);

    std::weak_ptr<SceneObject> _object;
    bool _enabled;

};

}
