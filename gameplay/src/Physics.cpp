#include "Base.h"
#include "Physics.h"
#include "PhysicsPhysX.h"

namespace gameplay
{

Physics* Physics::_physics = nullptr;

Physics* Physics::getPhysics()
{
    if (!_physics)
    {
        _physics = new PhysicsPhysX();
    }
    return _physics;
}

}
