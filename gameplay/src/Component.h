#pragma once

#include "Activator.h"

namespace gameplay
{
    class SceneObject;

/**
 * Defines a component
 */
class Component : public Serializable, public std::enable_shared_from_this<Component>
{
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Defines the component class type.
     */
    enum class ClassType
    {
        eCamera,
        eLight,
        eScript,
        eAnimation,
        eAudioSource,
        eMesh,
        eSprite,
        eText,
        eTileset,
        ePath,
        eParticleEmitter,
        eTerrain,
        eWater,
        eTree,
        eDecal,
        eVideo,
        eUI,
        ePhysicsCollider,
        ePhysicsRigidBody,
        ePhysicsCharacter,
        ePhysicsVehicle,
        ePhysicsCloth,
        ePhysicsJointFixed,
        ePhysicsJointGeneric,
        ePhysicsJointHinge,
        ePhysicsJointSocket,
        ePhysicsJointSpring,
        eUser
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
     * Determines if a component is enabled.
     *
     * @return  true if the component is active, false if not active.
     */
    bool isEnabled() const;

    /**
     * Sets if the component as enabled.
     *
     * @param enabled true if the component is active, false if not active.
     */
    void setEnabled(bool enabled);

    /**
     * This gets the class type for a component.
     *
     * @return the class type for a component.
     */
    virtual Component::ClassType getClassType() = 0;

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
