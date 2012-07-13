#include "Base.h"
#include "ScriptController.h"
#include "ScriptListener.h"

namespace gameplay
{

static const char* SL_ANIMATION_ARGS = "<AnimationClip>[AnimationClip::Listener::EventType]";
static const char* SL_PHYSICS_COLLISION_ARGS = "[PhysicsCollisionObject::CollisionListener::EventType]<PhysicsCollisionObject::CollisionPair><Vector3><Vector3>";
static const char* SL_TRANSFORM_ARGS = "<Transform>l";
static const char* SL_PHYSICS_STATUS_ARGS = "[PhysicsController::Listener::EventType]";
static const char* SL_CONTROL_ARGS = "<Control>[Control::Listener::EventType]";
static const char* SL_TIME_ARGS = "l";

void ScriptListener::animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type)
{
    _sc->executeFunction<void>(_function.c_str(), SL_ANIMATION_ARGS, clip, type);
}

void ScriptListener::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
    const PhysicsCollisionObject::CollisionPair& collisionPair, const Vector3& contactPointA, const Vector3& contactPointB)
{
    _sc->executeFunction<void>(_function.c_str(), SL_PHYSICS_COLLISION_ARGS, type, &collisionPair, &contactPointA, &contactPointB);
}

void ScriptListener::transformChanged(Transform* transform, long cookie)
{
    _sc->executeFunction<void>(_function.c_str(), SL_TRANSFORM_ARGS, transform, cookie);
}

void ScriptListener::statusEvent(PhysicsController::Listener::EventType type)
{
    _sc->executeFunction<void>(_function.c_str(), SL_PHYSICS_STATUS_ARGS, type);
}

void ScriptListener::controlEvent(Control* control, Control::Listener::EventType evt)
{
    _sc->executeFunction<void>(_function.c_str(), SL_CONTROL_ARGS, control, evt);
}

void ScriptListener::timeEvent(long timeDiff, void* cookie)
{
    _sc->executeFunction<void>(_function.c_str(), SL_TIME_ARGS, timeDiff);
}

ScriptListener::ScriptListener(const char* function)
{
    _function = function ? function : "";
    _sc = Game::getInstance()->getScriptController();

    GP_ASSERT(_function.size() > 0);
    GP_ASSERT(_sc);
}


}
