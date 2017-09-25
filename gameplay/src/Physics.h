#pragma once

#include "Component.h"
#include "PhysicsCollisionShape.h"

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

    std::shared_ptr<PhysicsCollisionShape> createCollisionBox(const Vector3& center, const Vector3& extents);

    std::shared_ptr<PhysicsCollisionShape> createCollisionSphere(const Vector3& center, float radius);

    std::shared_ptr<PhysicsCollisionShape> createCollisionCapsule(const Vector3& center, float radius, float height,
                                                                  PhysicsCollisionShape::Capsule::Direction direction);

    std::shared_ptr<PhysicsCollisionShape> createCollisionHeightfield(std::shared_ptr<Heightfield> heightfield);

    std::shared_ptr<PhysicsCollisionShape> createCollisionMesh(std::shared_ptr<Mesh> mesh);

    

private:

    void onInitialize();
    void onUpdate(float elapsedTime);
    void onPause();
    void onResume();

    static Physics* _physics;
};

}