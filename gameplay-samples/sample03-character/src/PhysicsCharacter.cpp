#include "PhysicsCharacter.h"

namespace gameplay
{

PhysicsCharacter::PhysicsCharacter(Node* node)
    : _node(node), _currentMovement(NULL)
{
    node->addRef();
}

PhysicsCharacter::~PhysicsCharacter()
{
    SAFE_RELEASE(_node);
}

void PhysicsCharacter::addMovement(const char* name, AnimationClip* clip, const Vector3& velocity)
{
    Movement m;
    m.clip = clip;
    m.velocity = velocity;
    _movements[name] = m;
}

void PhysicsCharacter::setMovement(const char* name)
{
    std::map<const char*, Movement>::iterator itr = _movements.find(name);
    if (itr != _movements.end())
    {
        _currentMovement = &((*itr).second);
    }
}

void PhysicsCharacter::stop()
{
    _currentMovement = NULL;
}

void PhysicsCharacter::update(long elapsedTime)
{
    Scene* scene = _node->getScene();
    Camera* camera = scene->getActiveCamera();

    Vector3 rayFrom = camera->getNode()->getTranslationWorld();

    Ray ray(camera->getNode()->getTranslationWorld(), camera->getNode()->getForwardVectorWorld());

    Node* walls[4] = {
        scene->findNode("Wall1"),
        scene->findNode("Wall2"),
        scene->findNode("Wall3"),
        scene->findNode("Wall4")
    };

    for (unsigned int i = 0; i < sizeof(walls); ++i)
    {
        // Get local (non-transformed) bounding box
        const BoundingBox& box = walls[i]->getModel()->getMesh()->getBoundingBox();
        /*
        // Move pick ray into the local space of the box
        Matrix t;
        walls[i]->getWorldMatrix().invert(&t);

        Vector3 rayFr
        t.transformPoint(
        */
        PhysicsRigidBody* rb = walls[i]->getPhysicsRigidBody();

        /*
        walls[i]->getModel()->getMesh()->getBoundingBox();

        rb->coll
        btRayAabb(
        rb->collidesWith(
        if (walls[i]->getPhysicsRigidBody()*/
    }
}

}
