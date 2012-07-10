#ifndef SCRIPTLISTENER_H_
#define SCRIPTLISTENER_H_

#include "AnimationClip.h"
#include "Control.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "Transform.h"
#include "TimeListener.h"

namespace gameplay
{

/**
 * Used to bind Lua script functions as listeners
 * for any listener interface within gameplay.
 * 
 * @script{ignore}
 */
class ScriptListener : public AnimationClip::Listener, public PhysicsCollisionObject::CollisionListener,
    public Transform::Listener, public PhysicsController::Listener, public Control::Listener, public TimeListener
{
    friend class AnimationClip;
    friend class Control;
    friend class Game;
    friend class PhysicsCollisionObject;
    friend class PhysicsController;
    friend class Transform;

public:
    /** @see AnimationClip#Listener */
    void animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type);
        
    /** @see PhysicsCollisionObject::CollisionListener */
    void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
                                const PhysicsCollisionObject::CollisionPair& collisionPair,
                                const Vector3& contactPointA = Vector3::zero(),
                                const Vector3& contactPointB = Vector3::zero());

    /** @see Transform::Listener */
    void transformChanged(Transform* transform, long cookie);

    /** @see PhysicsController::Listener */
    void statusEvent(PhysicsController::Listener::EventType type);

    /** @see Control::Listener */
    void controlEvent(Control* control, Control::Listener::EventType evt);

    /** @see TimeListener */
    void timeEvent(long timeDiff, void* cookie);

private:
    /**
     * Constructor.
     */
    ScriptListener(const char* function);

    /** Holds the name of the Lua function to be called back. */
    std::string _function;
    /** Holds the global script controller. */
    ScriptController* _sc;
};

}

#endif
