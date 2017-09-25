#include "Base.h"
#include "PhysicsCollisionShape.h"
#include "Vector3.h"

namespace gameplay
{

PhysicsCollisionShape::PhysicsCollisionShape()
{
}

PhysicsCollisionShape::~PhysicsCollisionShape()
{
}

void PhysicsCollisionShape::Box::setCenter(const Vector3& center)
{
}

Vector3 PhysicsCollisionShape::Box::getCenter() const
{
    return Vector3::zero();
}

void PhysicsCollisionShape::Box::setExtents(const Vector3& extents)
{
}

Vector3 PhysicsCollisionShape::Box::getExtents() const
{
    return Vector3::zero(); 
}

void PhysicsCollisionShape::Sphere::setCenter(const Vector3& center)
{
}

Vector3 PhysicsCollisionShape::Sphere::getCenter() const
{
    return Vector3::zero();
}

void PhysicsCollisionShape::Sphere::setRadius(float radius)
{
}

float PhysicsCollisionShape::Sphere::getRadius() const
{
    return 0.0f;
}

void PhysicsCollisionShape::Capsule::setCenter(const Vector3& center)
{
}

Vector3 PhysicsCollisionShape::Capsule::getCenter() const
{
    return Vector3::zero();
}

void PhysicsCollisionShape::Capsule::setRadius(float radius)
{
}

float PhysicsCollisionShape::Capsule::getRadius() const
{
    return 0.0f;
}

void PhysicsCollisionShape::Capsule::setHeight(float height)
{
}

float PhysicsCollisionShape::Capsule::getHeight() const
{
    return 0.0f;
}

void PhysicsCollisionShape::Capsule::setDirection(PhysicsCollisionShape::Capsule::Direction direction)
{
}

PhysicsCollisionShape::Capsule::Direction PhysicsCollisionShape::Capsule::getDirection() const
{
    return PhysicsCollisionShape::Capsule::Direction::DIRECTION_Y;
}

PhysicsCollisionShape::Type PhysicsCollisionShape::getType() const
{
    return _type;
}

std::shared_ptr<PhysicsCollisionShape::Box> PhysicsCollisionShape::getBoxGeometry() const
{
    return _box;
}

std::shared_ptr<PhysicsCollisionShape::Sphere> PhysicsCollisionShape::getSphereGeometry() const
{
    return _sphere;
}

std::shared_ptr<PhysicsCollisionShape::Capsule> PhysicsCollisionShape::getCapsuleGeometry() const
{
    return _capsule;
}

std::shared_ptr<Heightfield> PhysicsCollisionShape::getHeightfieldGeometry() const
{
    return _heightfield;
}

std::shared_ptr<Mesh> PhysicsCollisionShape::getMeshGeometry() const
{
    return _mesh;
}

void PhysicsCollisionShape::setMaterial(std::shared_ptr<PhysicsMaterial> material)
{
}

std::shared_ptr<PhysicsMaterial> PhysicsCollisionShape::getMaterial() const
{
    return nullptr;
}

void PhysicsCollisionShape::setTrigger(bool trigger)
{
}

bool PhysicsCollisionShape::isTrigger() const
{
    return false;
}

Component::TypeId PhysicsCollisionShape::getTypeId()
{
	return Component::TYPEID_PHYSICS_COLLISION_SHAPE;
}

std::string PhysicsCollisionShape::getClassName()
{
	return "gameplay::PhysicsCollisionShape";
}

void PhysicsCollisionShape::onSerialize(Serializer * serializer)
{
}

void PhysicsCollisionShape::onDeserialize(Serializer * serializer)
{
}

std::shared_ptr<Serializable> PhysicsCollisionShape::createObject()
{
	return std::shared_ptr<PhysicsCollisionShape>();
}

}