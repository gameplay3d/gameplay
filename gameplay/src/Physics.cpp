#include "Base.h"
#include "Physics.h"
#include "PhysicsCollisionShape.h"

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

std::shared_ptr<PhysicsCollisionShape> Physics::createCollisionBox(const Vector3& center, const Vector3& extents)
{
    std::shared_ptr<PhysicsCollisionShape> shape = std::make_shared<PhysicsCollisionShape>();
    std::shared_ptr<PhysicsCollisionShape::Box> geometry = std::make_shared<PhysicsCollisionShape::Box>();    
    geometry->setCenter(center);
    geometry->setExtents(extents);
    shape->_type = PhysicsCollisionShape::TYPE_BOX;
    shape->_box = geometry;
    return shape;
}

std::shared_ptr<PhysicsCollisionShape> Physics::createCollisionSphere(const Vector3& center, float radius)
{
    std::shared_ptr<PhysicsCollisionShape> shape = std::make_shared<PhysicsCollisionShape>();
    std::shared_ptr<PhysicsCollisionShape::Sphere> geometry = std::make_shared<PhysicsCollisionShape::Sphere>();
    geometry->setCenter(center);
    geometry->setRadius(radius);
    shape->_type = PhysicsCollisionShape::TYPE_SPHERE;
    shape->_sphere = geometry;
    return shape;
}

std::shared_ptr<PhysicsCollisionShape> Physics::createCollisionCapsule(const Vector3& center, float radius, float height,
                                                                       PhysicsCollisionShape::Capsule::Direction direction)
{
    std::shared_ptr<PhysicsCollisionShape> shape = std::make_shared<PhysicsCollisionShape>();
    std::shared_ptr<PhysicsCollisionShape::Capsule> geometry = std::make_shared<PhysicsCollisionShape::Capsule>();
    geometry->setCenter(center);
    geometry->setRadius(radius);
    geometry->setHeight(height);
    geometry->setDirection(direction);
    shape->_type = PhysicsCollisionShape::TYPE_CAPSULE;
    shape->_capsule = geometry;
    return shape;
}

std::shared_ptr<PhysicsCollisionShape> Physics::createCollisionHeightfield(std::shared_ptr<Heightfield> heightfield)
{
    std::shared_ptr<PhysicsCollisionShape> shape = std::make_shared<PhysicsCollisionShape>();
    shape->_type = PhysicsCollisionShape::TYPE_HEIGHTFIELD;
    shape->_heightfield = heightfield;
    return shape;
}

std::shared_ptr<PhysicsCollisionShape> Physics::createCollisionMesh(std::shared_ptr<Mesh> mesh)
{
    std::shared_ptr<PhysicsCollisionShape> shape = std::make_shared<PhysicsCollisionShape>();
    shape->_type = PhysicsCollisionShape::TYPE_HEIGHTFIELD;
    shape->_mesh = mesh;
    return shape;
}

}