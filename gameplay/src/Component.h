#pragma once

#include "Serializable.h"

namespace gameplay
{
	class SceneObject;

/**
 * Defines a component
 */
class Component : public Serializable, public std::enable_shared_from_this<Component>
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
        TYPEID_MESH,
		TYPEID_TEXT,
		TYPEID_TILESET,
		TYPEID_PATH,
		TYPEID_PARTICLE_EMITTER,
		TYPEID_TERRAIN,
		TYPEID_WATER,
		TYPEID_TREE,
		TYPEID_DECAL,
		TYPEID_VIDEO,
		TYPEID_UI,
        TYPEID_PHYSICS_COLLISION_SHAPE,
        TYPEID_PHYSICS_RIGIDBODY,
		TYPEID_PHYSICS_CHARACTER,
		TYPEID_PHYSICS_VEHICLE,
		TYPEID_PHYSICS_CLOTH,
		TYPEID_PHYSICS_JOINT_FIXED,
		TYPEID_PHYSICS_JOINT_HINGE,
		TYPEID_PHYSICS_JOINT_SPRING,
		TYPEID_PHYSICS_JOINT_SOCKET,
		TYPEID_PHYSICS_JOINT_GENERIC
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

    /**
     * Event occurs when the component is attached to a scene object;
     */
    virtual void onAttached();

	/**
     * Event occurs when the component is detach from a scene object;
     */
    virtual void onDetached();

    std::weak_ptr<SceneObject> _object;
    bool _enabled;

};

}
