#include "Base.h"
#include "Physics.h"
#include "PhysicsCollider.h"

namespace gameplay
{

Physics* Physics::_physics = nullptr;

Physics* Physics::getPhysics()
{
    if (!_physics)
        _physics = new Physics();
    return _physics;
}

void Physics::onInitialize()
{
}

void Physics::onUpdate(float elapsedTime)
{
}

void Physics::onPause()
{
}

void Physics::onResume()
{
}

std::shared_ptr<PhysicsCollider> Physics::createBoxCollider(const Vector3& center, const Vector3& extents)
{
    std::shared_ptr<PhysicsCollider> collider;
    return collider;
}

std::shared_ptr<PhysicsCollider> Physics::createSphereCollider(const Vector3& center, float radius)
{
    std::shared_ptr<PhysicsCollider> collider;
    return collider;
}

std::shared_ptr<PhysicsCollider> Physics::createCapsuleCollider(const Vector3& center, float radius, float height, PhysicsCollider::Capsule::Direction direction)
{
    std::shared_ptr<PhysicsCollider> collider;
    return collider;
}

std::shared_ptr<PhysicsCollider> Physics::createMeshCollider(std::shared_ptr<Mesh> mesh)
{
    std::shared_ptr<PhysicsCollider> collider;
    return collider;
}

std::shared_ptr<PhysicsCollider> Physics::createHeightfieldCollider(std::shared_ptr<Heightfield> heightfield)
{
    std::shared_ptr<PhysicsCollider> collider;
    return collider;
}

}
