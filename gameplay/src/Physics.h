#pragma once

#include "Component.h"
#include "PhysicsCollider.h"

namespace gameplay
{
	class Heightfield;
	class Mesh;

/**
 * Defines an abstract physics system.
 */
class Physics
{
    friend class Game;

public:

    /**
     * Gets the physics system.
     *
     * @return The physics system.
     */
    static Physics* getPhysics();

    std::shared_ptr<PhysicsCollider> createBoxCollider(const Vector3& center, const Vector3& extents);

    std::shared_ptr<PhysicsCollider> createSphereCollider(const Vector3& center, float radius);

    std::shared_ptr<PhysicsCollider> createCapsuleCollider(const Vector3& center, float radius, float height, PhysicsCollider::Capsule::Direction direction);

    std::shared_ptr<PhysicsCollider> createMeshCollider(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<PhysicsCollider> createHeightfieldCollider(std::shared_ptr<Heightfield> heightfield);

private:

    void onInitialize();
    void onUpdate(float elapsedTime);
    void onPause();
    void onResume();

    static Physics* _physics;
};

}
