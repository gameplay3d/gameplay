#include "Base.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsController.h"
#include "Game.h"
#include "Node.h"
#include "ScriptController.h"
#include "PhysicsRigidBody.h"
#include "PhysicsCharacter.h"
#include "PhysicsGhostObject.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"

namespace gameplay
{

/**
 * Internal class used to implement the collidesWith(PhysicsCollisionObject*) function.
 * @script{ignore}
 */
struct CollidesWithCallback : public btCollisionWorld::ContactResultCallback
{
    /**
     * Called with each contact. Needed to implement collidesWith(PhysicsCollisionObject*).
     */
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* a, int partIdA, int indexA, const btCollisionObjectWrapper* b, int partIdB, int indexB)
    {
        result = true;
        return 0.0f;
    }

    /**
     * The result of the callback.
     */
    bool result;
};

PhysicsCollisionObject::PhysicsCollisionObject(Node* node)
    : _node(node), _collisionShape(NULL), _enabled(true), _scriptListeners(NULL), _motionState(NULL)
{
}

PhysicsCollisionObject::~PhysicsCollisionObject()
{
    SAFE_DELETE(_motionState);

    if (_scriptListeners)
    {
        for (unsigned int i = 0; i < _scriptListeners->size(); i++)
        {
            SAFE_DELETE((*_scriptListeners)[i]);
        }
        SAFE_DELETE(_scriptListeners);
    }

    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->destroyShape(_collisionShape);
}

PhysicsCollisionShape::Type PhysicsCollisionObject::getShapeType() const
{
    GP_ASSERT(getCollisionShape());
    return getCollisionShape()->getType();
}

Node* PhysicsCollisionObject::getNode() const
{
    return _node;
}

PhysicsCollisionShape* PhysicsCollisionObject::getCollisionShape() const
{
    return _collisionShape;
}

bool PhysicsCollisionObject::isKinematic() const
{
    switch (getType())
    {
    case GHOST_OBJECT:
    case CHARACTER:
        return true;
    default:
        GP_ASSERT(getCollisionObject());
        return getCollisionObject()->isKinematicObject();
    }
}

bool PhysicsCollisionObject::isStatic() const
{
    switch (getType())
    {
    case GHOST_OBJECT:
    case CHARACTER:
        return false;
    default:
        GP_ASSERT(getCollisionObject());
        return getCollisionObject()->isStaticObject();
    }
}

bool PhysicsCollisionObject::isDynamic() const
{
    GP_ASSERT(getCollisionObject());
    return !getCollisionObject()->isStaticOrKinematicObject();
}

bool PhysicsCollisionObject::isEnabled() const
{
    return _enabled;
}

void PhysicsCollisionObject::setEnabled(bool enable)
{
    if (enable)
    {  
        if (!_enabled)
        {
            Game::getInstance()->getPhysicsController()->addCollisionObject(this);
            _motionState->updateTransformFromNode();
            _enabled = true;
        }
    }
    else
    {
        if (_enabled)
        {
            Game::getInstance()->getPhysicsController()->removeCollisionObject(this, false);
            _enabled = false;
        }
    }
}

void PhysicsCollisionObject::addCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->addCollisionListener(listener, this, object);
}

void PhysicsCollisionObject::removeCollisionListener(CollisionListener* listener, PhysicsCollisionObject* object)
{
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->removeCollisionListener(listener, this, object);
}

void PhysicsCollisionObject::addCollisionListener(const char* function, PhysicsCollisionObject* object)
{
    if (!_scriptListeners)
        _scriptListeners = new std::vector<ScriptListener*>();

    ScriptListener* listener = new ScriptListener(function);
    _scriptListeners->push_back(listener);
    addCollisionListener(listener, object);
}

void PhysicsCollisionObject::removeCollisionListener(const char* function, PhysicsCollisionObject* object)
{
    if (!_scriptListeners)
        return;

    std::string url = function;
    for (unsigned int i = 0; i < _scriptListeners->size(); i++)
    {
        if ((*_scriptListeners)[i]->url == url)
        {
            removeCollisionListener((*_scriptListeners)[i], object);
            SAFE_DELETE((*_scriptListeners)[i]);
            _scriptListeners->erase(_scriptListeners->begin() + i);
            return;
        }
    }
}

bool PhysicsCollisionObject::collidesWith(PhysicsCollisionObject* object) const
{
    GP_ASSERT(Game::getInstance()->getPhysicsController() && Game::getInstance()->getPhysicsController()->_world);
    GP_ASSERT(object && object->getCollisionObject());
    GP_ASSERT(getCollisionObject());

    static CollidesWithCallback callback;

    callback.result = false;
    Game::getInstance()->getPhysicsController()->_world->contactPairTest(getCollisionObject(), object->getCollisionObject(), callback);
    return callback.result;
}

PhysicsRigidBody* PhysicsCollisionObject::asRigidBody()
{
    return getType() == RIGID_BODY ? static_cast<PhysicsRigidBody*>(this) : NULL;
}

PhysicsCharacter* PhysicsCollisionObject::asCharacter()
{
    return getType() == CHARACTER ? static_cast<PhysicsCharacter*>(this) : NULL;
}

PhysicsGhostObject* PhysicsCollisionObject::asGhostObject()
{
    return getType() == GHOST_OBJECT ? static_cast<PhysicsGhostObject*>(this) : NULL;
}

PhysicsVehicle* PhysicsCollisionObject::asVehicle()
{
    return getType() == VEHICLE ? static_cast<PhysicsVehicle*>(this) : NULL;
}

PhysicsVehicleWheel* PhysicsCollisionObject::asVehicleWheel()
{
    return getType() == VEHICLE_WHEEL ? static_cast<PhysicsVehicleWheel*>(this) : NULL;
}

PhysicsCollisionObject::CollisionPair::CollisionPair(PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
    : objectA(objectA), objectB(objectB)
{
    // unused
}

bool PhysicsCollisionObject::CollisionPair::operator < (const CollisionPair& collisionPair) const
{
    // If the pairs are equal, then return false.
    if ((objectA == collisionPair.objectA && objectB == collisionPair.objectB) || (objectA == collisionPair.objectB && objectB == collisionPair.objectA))
        return false;

    // We choose to compare based on objectA arbitrarily.
    if (objectA < collisionPair.objectA)
        return true;

    if (objectA == collisionPair.objectA)
        return objectB < collisionPair.objectB;

    return false;
}

PhysicsCollisionObject::PhysicsMotionState::PhysicsMotionState(Node* node, PhysicsCollisionObject* collisionObject, const Vector3* centerOfMassOffset) :
    _node(node), _collisionObject(collisionObject), _centerOfMassOffset(btTransform::getIdentity())
{
    if (centerOfMassOffset)
    {
        // Store the center of mass offset.
        _centerOfMassOffset.setOrigin(BV(*centerOfMassOffset));
    }

    updateTransformFromNode();
}

PhysicsCollisionObject::PhysicsMotionState::~PhysicsMotionState()
{
}

void PhysicsCollisionObject::PhysicsMotionState::getWorldTransform(btTransform &transform) const
{
    GP_ASSERT(_node);
    GP_ASSERT(_collisionObject);

    if (_collisionObject->isKinematic())
        updateTransformFromNode();

    transform = _centerOfMassOffset.inverse() * _worldTransform;
}

void PhysicsCollisionObject::PhysicsMotionState::setWorldTransform(const btTransform &transform)
{
    GP_ASSERT(_node);

    _worldTransform = transform * _centerOfMassOffset;
        
    const btQuaternion& rot = _worldTransform.getRotation();
    const btVector3& pos = _worldTransform.getOrigin();

    _node->setRotation(rot.x(), rot.y(), rot.z(), rot.w());
    _node->setTranslation(pos.x(), pos.y(), pos.z());
}

void PhysicsCollisionObject::PhysicsMotionState::updateTransformFromNode() const
{
    GP_ASSERT(_node);

    // Store the initial world transform (minus the scale) for use by Bullet later on.
    Quaternion rotation;
    const Matrix& m = _node->getWorldMatrix();
    m.getRotation(&rotation);

    if (!_centerOfMassOffset.getOrigin().isZero())
    {
        // When there is a center of mass offset, we modify the initial world transformation
        // so that when physics is initially applied, the object is in the correct location.
        btTransform offset = btTransform(BQ(rotation), btVector3(0.0f, 0.0f, 0.0f)) * _centerOfMassOffset.inverse();

        btVector3 origin(m.m[12] + _centerOfMassOffset.getOrigin().getX() + offset.getOrigin().getX(), 
                         m.m[13] + _centerOfMassOffset.getOrigin().getY() + offset.getOrigin().getY(), 
                         m.m[14] + _centerOfMassOffset.getOrigin().getZ() + offset.getOrigin().getZ());
        _worldTransform = btTransform(BQ(rotation), origin);
    }
    else
    {
        _worldTransform = btTransform(BQ(rotation), btVector3(m.m[12], m.m[13], m.m[14]));
    }
}

void PhysicsCollisionObject::PhysicsMotionState::setCenterOfMassOffset(const Vector3& centerOfMassOffset)
{
    _centerOfMassOffset.setOrigin(BV(centerOfMassOffset));
}

PhysicsCollisionObject::ScriptListener::ScriptListener(const char* url)
    : url(url)
{
    function = Game::getInstance()->getScriptController()->loadUrl(url);
}

void PhysicsCollisionObject::ScriptListener::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
    const PhysicsCollisionObject::CollisionPair& collisionPair, const Vector3& contactPointA, const Vector3& contactPointB)
{
    Game::getInstance()->getScriptController()->executeFunction<void>(function.c_str(), 
        "[PhysicsCollisionObject::CollisionListener::EventType]<PhysicsCollisionObject::CollisionPair><Vector3><Vector3>",
        type, &collisionPair, &contactPointA, &contactPointB);
}

}
