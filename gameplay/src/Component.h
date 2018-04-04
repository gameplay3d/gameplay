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
     * Event occurs when the component is added to a scene object;
     */
    virtual void onAdded();

    /**
     * Event occurs when the component is removed from a scene object;
     */
    virtual void onRemoved();

    std::weak_ptr<SceneObject> _object;
    bool _enabled;

};

}
