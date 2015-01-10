#include "Base.h"
#include "PhysicsController.h"
#include "PhysicsRigidBody.h"
#include "PhysicsCharacter.h"
#include "Game.h"
#include "MeshPart.h"
#include "Bundle.h"
#include "Terrain.h"

#ifdef GP_USE_MEM_LEAK_DETECTION
#undef new
#endif
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#ifdef GP_USE_MEM_LEAK_DETECTION
#define new DEBUG_NEW
#endif

// The initial capacity of the Bullet debug drawer's vertex batch.
#define INITIAL_CAPACITY 280

namespace gameplay
{

const int PhysicsController::DIRTY         = 0x01;
const int PhysicsController::COLLISION     = 0x02;
const int PhysicsController::REGISTERED    = 0x04;
const int PhysicsController::REMOVE        = 0x08;

PhysicsController::PhysicsController()
  : _isUpdating(false), _collisionConfiguration(NULL), _dispatcher(NULL),
    _overlappingPairCache(NULL), _solver(NULL), _world(NULL), _ghostPairCallback(NULL),
    _debugDrawer(NULL), _status(PhysicsController::Listener::DEACTIVATED), _listeners(NULL),
    _gravity(btScalar(0.0), btScalar(-9.8), btScalar(0.0)), _collisionCallback(NULL)
{
    GP_REGISTER_SCRIPT_EVENTS();

    // Default gravity is 9.8 along the negative Y axis.
    _collisionCallback = new CollisionCallback(this);
}

PhysicsController::~PhysicsController()
{
    SAFE_DELETE(_collisionCallback);
    SAFE_DELETE(_ghostPairCallback);
    SAFE_DELETE(_debugDrawer);
    SAFE_DELETE(_listeners);
}

const char* PhysicsController::getTypeName() const
{
    return "PhysicsController";
}

void PhysicsController::addStatusListener(Listener* listener)
{
    GP_ASSERT(listener);
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    _listeners->push_back(listener);
}

void PhysicsController::removeStatusListener(Listener* listener)
{
    GP_ASSERT(listener);
    if (!_listeners)
        return;

    for (std::vector<Listener*>::iterator iter = _listeners->begin(); iter != _listeners->end(); iter++)
    {
        if (*iter == listener)
        {
            _listeners->erase(iter);
            return;
        }
    }
}

PhysicsFixedConstraint* PhysicsController::createFixedConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    checkConstraintRigidBodies(a, b);
    PhysicsFixedConstraint* constraint = new PhysicsFixedConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    checkConstraintRigidBodies(a, b);
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b,
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    checkConstraintRigidBodies(a, b);
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, rotationOffsetA, translationOffsetA, b, rotationOffsetB, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsHingeConstraint* PhysicsController::createHingeConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b, 
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    checkConstraintRigidBodies(a, b);
    PhysicsHingeConstraint* constraint = new PhysicsHingeConstraint(a, rotationOffsetA, translationOffsetA, b, rotationOffsetB, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    checkConstraintRigidBodies(a, b);
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a,
    const Vector3& translationOffsetA, PhysicsRigidBody* b, const Vector3& translationOffsetB)
{
    checkConstraintRigidBodies(a, b);
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a,translationOffsetA, b, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    checkConstraintRigidBodies(a, b);
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,           
                                                                   PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    checkConstraintRigidBodies(a, b);
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, rotationOffsetA, translationOffsetA, b, rotationOffsetB, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

const Vector3& PhysicsController::getGravity() const
{
    return _gravity;
}

void PhysicsController::setGravity(const Vector3& gravity)
{
    _gravity = gravity;

    if (_world)
        _world->setGravity(BV(_gravity));
}

void PhysicsController::drawDebug(const Matrix& viewProjection)
{
    GP_ASSERT(_debugDrawer);
    GP_ASSERT(_world);

    _debugDrawer->begin(viewProjection);
    _world->debugDrawWorld();
    _debugDrawer->end();
}

bool PhysicsController::rayTest(const Ray& ray, float distance, PhysicsController::HitResult* result, PhysicsController::HitFilter* filter)
{
    class RayTestCallback : public btCollisionWorld::ClosestRayResultCallback
    {
    private:

        HitFilter* filter;
        HitResult hitResult;

    public:

        RayTestCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld, PhysicsController::HitFilter* filter)
            : btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld), filter(filter)
        {
        }

        virtual bool needsCollision(btBroadphaseProxy* proxy0) const
        {
            if (!btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy0))
                return false;

            btCollisionObject* co = reinterpret_cast<btCollisionObject*>(proxy0->m_clientObject);
            PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(co->getUserPointer());
            if (object == NULL)
                return false;

            return filter ? !filter->filter(object) : true;
        }

        btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
        {
            GP_ASSERT(rayResult.m_collisionObject);
            PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(rayResult.m_collisionObject->getUserPointer());

            if (object == NULL)
                return 1.0f; // ignore

            float result = btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);

            hitResult.object = object;
            hitResult.point.set(m_hitPointWorld.x(), m_hitPointWorld.y(), m_hitPointWorld.z());
            hitResult.fraction = m_closestHitFraction;
            hitResult.normal.set(m_hitNormalWorld.x(), m_hitNormalWorld.y(), m_hitNormalWorld.z());

            if (filter && !filter->hit(hitResult))
                return 1.0f; // process next collision

            return result; // continue normally
        }
    };

    GP_ASSERT(_world);

    btVector3 rayFromWorld(BV(ray.getOrigin()));
    btVector3 rayToWorld(rayFromWorld + BV(ray.getDirection() * distance));

    RayTestCallback callback(rayFromWorld, rayToWorld, filter);
    _world->rayTest(rayFromWorld, rayToWorld, callback);
    if (callback.hasHit())
    {
        if (result)
        {
            result->object = getCollisionObject(callback.m_collisionObject);
            result->point.set(callback.m_hitPointWorld.x(), callback.m_hitPointWorld.y(), callback.m_hitPointWorld.z());
            result->fraction = callback.m_closestHitFraction;
            result->normal.set(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
        }

        return true;
    }

    return false;
}

bool PhysicsController::sweepTest(PhysicsCollisionObject* object, const Vector3& endPosition, PhysicsController::HitResult* result, PhysicsController::HitFilter* filter)
{
    class SweepTestCallback : public btCollisionWorld::ClosestConvexResultCallback
    {
    private:

        PhysicsCollisionObject* me;
        PhysicsController::HitFilter* filter;
        PhysicsController::HitResult hitResult;

    public:

        SweepTestCallback(PhysicsCollisionObject* me, PhysicsController::HitFilter* filter)
            : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), me(me), filter(filter)
        {
        }

        virtual bool needsCollision(btBroadphaseProxy* proxy0) const
        {
            if (!btCollisionWorld::ClosestConvexResultCallback::needsCollision(proxy0))
                return false;

            btCollisionObject* co = reinterpret_cast<btCollisionObject*>(proxy0->m_clientObject);
            PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(co->getUserPointer());
            if (object == NULL || object == me)
                return false;

            return filter ? !filter->filter(object) : true;
        }

        btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
        {
            GP_ASSERT(convexResult.m_hitCollisionObject);
            PhysicsCollisionObject* object = reinterpret_cast<PhysicsCollisionObject*>(convexResult.m_hitCollisionObject->getUserPointer());

            if (object == NULL)
                return 1.0f;

            float result = ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);

            hitResult.object = object;
            hitResult.point.set(m_hitPointWorld.x(), m_hitPointWorld.y(), m_hitPointWorld.z());
            hitResult.fraction = m_closestHitFraction;
            hitResult.normal.set(m_hitNormalWorld.x(), m_hitNormalWorld.y(), m_hitNormalWorld.z());

            if (filter && !filter->hit(hitResult))
                return 1.0f;

            return result;
        }
    };

    GP_ASSERT(object && object->getCollisionShape());
    PhysicsCollisionShape* shape = object->getCollisionShape();
    PhysicsCollisionShape::Type type = shape->getType();
    if (type != PhysicsCollisionShape::SHAPE_BOX && type != PhysicsCollisionShape::SHAPE_SPHERE && type != PhysicsCollisionShape::SHAPE_CAPSULE)
        return false; // unsupported type

    // Define the start transform.
    btTransform start;
    start.setIdentity();
    if (object->getNode())
    {
        Vector3 translation;
        Quaternion rotation;
        const Matrix& m = object->getNode()->getWorldMatrix();
        m.getTranslation(&translation);
        m.getRotation(&rotation);

        start.setIdentity();
        start.setOrigin(BV(translation));
        start.setRotation(BQ(rotation));
    }

    // Define the end transform.
    btTransform end(start);
    end.setOrigin(BV(endPosition));

    // Perform bullet convex sweep test.
    SweepTestCallback callback(object, filter);

    // If the object is represented by a ghost object, use the ghost object's convex sweep test
    // since it is much faster than the world's version.
    // NOTE: Unfortunately the ghost object sweep test does not seem reliable here currently, so using world's version instead.
    /*switch (object->getType())
    {
    case PhysicsCollisionObject::GHOST_OBJECT:
    case PhysicsCollisionObject::CHARACTER:
        static_cast<PhysicsGhostObject*>(object)->_ghostObject->convexSweepTest(static_cast<btConvexShape*>(shape->getShape()), start, end, callback, _world->getDispatchInfo().m_allowedCcdPenetration);
        break;

    default:
        _world->convexSweepTest(static_cast<btConvexShape*>(shape->getShape()), start, end, callback, _world->getDispatchInfo().m_allowedCcdPenetration);
        break;
    }*/

    GP_ASSERT(_world);
    _world->convexSweepTest(static_cast<btConvexShape*>(shape->getShape()), start, end, callback, _world->getDispatchInfo().m_allowedCcdPenetration);

    // Check for hits and store results.
    if (callback.hasHit())
    {
        if (result)
        {
            result->object = getCollisionObject(callback.m_hitCollisionObject);
            result->point.set(callback.m_hitPointWorld.x(), callback.m_hitPointWorld.y(), callback.m_hitPointWorld.z());
            result->fraction = callback.m_closestHitFraction;
            result->normal.set(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
        }

        return true;
    }

    return false;
}

btScalar PhysicsController::CollisionCallback::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* a, int partIdA, int indexA, 
    const btCollisionObjectWrapper* b, int partIdB, int indexB)
{
    GP_ASSERT(_pc);

    // Get pointers to the PhysicsCollisionObject objects.
    PhysicsCollisionObject* objectA = _pc->getCollisionObject(a->m_collisionObject);
    PhysicsCollisionObject* objectB = _pc->getCollisionObject(b->m_collisionObject);

    // If the given collision object pair has collided in the past, then
    // we notify the listeners only if the pair was not colliding
    // during the previous frame. Otherwise, it's a new pair, so add a
    // new entry to the cache with the appropriate listeners and notify them.
    PhysicsCollisionObject::CollisionPair pair(objectA, objectB);

    CollisionInfo* collisionInfo;
    if (_pc->_collisionStatus.count(pair) > 0)
    {
        collisionInfo = &_pc->_collisionStatus[pair];
    }
    else
    {
        // Add a new collision pair for these objects.
        collisionInfo = &_pc->_collisionStatus[pair];

        // Add the appropriate listeners.
        PhysicsCollisionObject::CollisionPair p1(pair.objectA, NULL);
        if (_pc->_collisionStatus.count(p1) > 0)
        {
            const CollisionInfo& ci = _pc->_collisionStatus[p1];
            std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = ci._listeners.begin();
            for (; iter != ci._listeners.end(); iter++)
            {
                GP_ASSERT(*iter);
                collisionInfo->_listeners.push_back(*iter);
            }
        }
        PhysicsCollisionObject::CollisionPair p2(pair.objectB, NULL);
        if (_pc->_collisionStatus.count(p2) > 0)
        {
            const CollisionInfo& ci = _pc->_collisionStatus[p2];
            std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = ci._listeners.begin();
            for (; iter != ci._listeners.end(); iter++)
            {
                GP_ASSERT(*iter);
                collisionInfo->_listeners.push_back(*iter);
            }
        }
    }

    // Fire collision event.
    if ((collisionInfo->_status & COLLISION) == 0)
    {
        std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = collisionInfo->_listeners.begin();
        for (; iter != collisionInfo->_listeners.end(); iter++)
        {
            GP_ASSERT(*iter);
            if ((collisionInfo->_status & REMOVE) == 0)
            {
                (*iter)->collisionEvent(PhysicsCollisionObject::CollisionListener::COLLIDING, pair, Vector3(cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z()),
                    Vector3(cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z()));
            }
        }
    }

    // Update the collision status cache (we remove the dirty bit
    // set in the controller's update so that this particular collision pair's
    // status is not reset to 'no collision' when the controller's update completes).
    collisionInfo->_status &= ~DIRTY;
    collisionInfo->_status |= COLLISION;
    return 0.0f;
}

void PhysicsController::initialize()
{
    _collisionConfiguration = bullet_new<btDefaultCollisionConfiguration>();
    _dispatcher = bullet_new<btCollisionDispatcher>(_collisionConfiguration);
    _overlappingPairCache = bullet_new<btDbvtBroadphase>();
    _solver = bullet_new<btSequentialImpulseConstraintSolver>();

    // Create the world.
    _world = bullet_new<btDiscreteDynamicsWorld>(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
    _world->setGravity(BV(_gravity));

    // Register ghost pair callback so bullet detects collisions with ghost objects (used for character collisions).
    GP_ASSERT(_world->getPairCache());
    _ghostPairCallback = bullet_new<btGhostPairCallback>();
    _world->getPairCache()->setInternalGhostPairCallback(_ghostPairCallback);
    _world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;

    // Set up debug drawing.
    _debugDrawer = new DebugDrawer();
    _world->setDebugDrawer(_debugDrawer);
}

void PhysicsController::finalize()
{
    // Clean up the world and its various components.
    SAFE_DELETE(_world);
    SAFE_DELETE(_ghostPairCallback);
    SAFE_DELETE(_solver);
    SAFE_DELETE(_overlappingPairCache);
    SAFE_DELETE(_dispatcher);
    SAFE_DELETE(_collisionConfiguration);
}

void PhysicsController::pause()
{
    // Unused
}

void PhysicsController::resume()
{
    // Unused
}

void PhysicsController::update(float elapsedTime)
{
    GP_ASSERT(_world);
    _isUpdating = true;

    // Update the physics simulation, with a maximum
    // of 10 simulation steps being performed in a given frame.
    //
    // Note that stepSimulation takes elapsed time in seconds
    // so we divide by 1000 to convert from milliseconds.
    _world->stepSimulation(elapsedTime * 0.001f, 10);

    // If we have status listeners, then check if our status has changed.
    if (_listeners || hasScriptListener(GP_GET_SCRIPT_EVENT(PhysicsController, statusEvent)))
    {
        Listener::EventType oldStatus = _status;

        if (_status == Listener::DEACTIVATED)
        {
            for (int i = 0; i < _world->getNumCollisionObjects(); i++)
            {
                GP_ASSERT(_world->getCollisionObjectArray()[i]);
                if (_world->getCollisionObjectArray()[i]->isActive())
                {
                    _status = Listener::ACTIVATED;
                    break;
                }
            }
        }
        else
        {
            bool allInactive = true;
            for (int i = 0; i < _world->getNumCollisionObjects(); i++)
            {
                GP_ASSERT(_world->getCollisionObjectArray()[i]);
                if (_world->getCollisionObjectArray()[i]->isActive())
                {
                    allInactive = false;
                    break;
                }
            }

            if (allInactive)
                _status = Listener::DEACTIVATED;
        }

        // If the status has changed, notify our listeners.
        if (oldStatus != _status)
        {
            if (_listeners)
            {
                for (unsigned int k = 0; k < _listeners->size(); k++)
                {
                    GP_ASSERT((*_listeners)[k]);
                    (*_listeners)[k]->statusEvent(_status);
                }
            }

            fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(PhysicsController, statusEvent), _status);
        }
    }

    // All statuses are set with the DIRTY bit before collision processing occurs.
    // During collision processing, if a collision occurs, the status is 
    // set to COLLISION and the DIRTY bit is cleared. Then, after collision processing 
    // is finished, if a given status is still dirty, the COLLISION bit is cleared.
    //
    // If an entry was marked for removal in the last frame, fire NOT_COLLIDING if appropriate and remove it now.

    // Dirty the collision status cache entries.
    std::map<PhysicsCollisionObject::CollisionPair, CollisionInfo>::iterator iter = _collisionStatus.begin();
    for (; iter != _collisionStatus.end();)
    {
        if ((iter->second._status & REMOVE) != 0)
        {
            if ((iter->second._status & COLLISION) != 0 && iter->first.objectB)
            {
                size_t size = iter->second._listeners.size();
                for (size_t i = 0; i < size; i++)
                {
                    PhysicsCollisionObject::CollisionPair cp(iter->first.objectA, NULL);
                    iter->second._listeners[i]->collisionEvent(PhysicsCollisionObject::CollisionListener::NOT_COLLIDING, cp);
                }
            }

            std::map<PhysicsCollisionObject::CollisionPair, CollisionInfo>::iterator eraseIter = iter;
            iter++;
            _collisionStatus.erase(eraseIter);
        }
        else
        {
            iter->second._status |= DIRTY;
            iter++;
        }
    }

    // Go through the collision status cache and perform all registered collision tests.
    iter = _collisionStatus.begin();
    for (; iter != _collisionStatus.end(); iter++)
    {
        // If this collision pair was one that was registered for listening, then perform the collision test.
        // (In the case where we register for all collisions with a rigid body, there will be a lot
        // of collision pairs in the status cache that we did not explicitly register for.)
        if ((iter->second._status & REGISTERED) != 0 && (iter->second._status & REMOVE) == 0)
        {
            if (iter->first.objectB)
                _world->contactPairTest(iter->first.objectA->getCollisionObject(), iter->first.objectB->getCollisionObject(), *_collisionCallback);
            else
                _world->contactTest(iter->first.objectA->getCollisionObject(), *_collisionCallback);
        }
    }

    // Update all the collision status cache entries.
    iter = _collisionStatus.begin();
    for ( ; iter != _collisionStatus.end(); iter++)
    {
        if ((iter->second._status & DIRTY) != 0)
        {
            if ((iter->second._status & COLLISION) != 0 && iter->first.objectB)
            {
                size_t size = iter->second._listeners.size();
                for (size_t i = 0; i < size; i++)
                {
                    iter->second._listeners[i]->collisionEvent(PhysicsCollisionObject::CollisionListener::NOT_COLLIDING, iter->first);
                }
            }

            iter->second._status &= ~COLLISION;
        }
    }

    _isUpdating = false;
}

void PhysicsController::addCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
{
    GP_ASSERT(listener);
    
    // One of the collision objects in the pair must be non-null.
    GP_ASSERT(objectA || objectB);
    PhysicsCollisionObject::CollisionPair pair(objectA, objectB);

    // Add the listener and ensure the status includes that this collision pair is registered.
    CollisionInfo& info = _collisionStatus[pair];
    info._listeners.push_back(listener);
    info._status |= PhysicsController::REGISTERED;
}

void PhysicsController::removeCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
{
    // One of the collision objects in the pair must be non-null.
    GP_ASSERT(objectA || objectB);
    PhysicsCollisionObject::CollisionPair pair(objectA, objectB);

    // Mark the collision pair for these objects for removal.
    if (_collisionStatus.count(pair) > 0)
    {
        _collisionStatus[pair]._status |= REMOVE;
    }
}

void PhysicsController::addCollisionObject(PhysicsCollisionObject* object)
{
    GP_ASSERT(object && object->getCollisionObject());
    GP_ASSERT(_world);

    // Assign user pointer for the bullet collision object to allow efficient
    // lookups of bullet objects -> gameplay objects.
    object->getCollisionObject()->setUserPointer(object);
    short group = (short)object->_group;
    short mask = (short)object->_mask;

    // Add the object to the physics world.
    switch (object->getType())
    {
    case PhysicsCollisionObject::RIGID_BODY:
        _world->addRigidBody(static_cast<btRigidBody*>(object->getCollisionObject()), group, mask);
        break;

    case PhysicsCollisionObject::CHARACTER:
        _world->addCollisionObject(object->getCollisionObject(), group, mask);
        break;

    case PhysicsCollisionObject::GHOST_OBJECT:
        _world->addCollisionObject(object->getCollisionObject(), group, mask);
        break;

    default:
        GP_ERROR("Unsupported collision object type (%d).", object->getType());
        break;
    }
}

void PhysicsController::removeCollisionObject(PhysicsCollisionObject* object, bool removeListeners)
{
    GP_ASSERT(object);
    GP_ASSERT(_world);
    GP_ASSERT(!_isUpdating);

    // Remove the collision object from the world.
    if (object->getCollisionObject())
    {
        switch (object->getType())
        {
        case PhysicsCollisionObject::RIGID_BODY:
            _world->removeRigidBody(static_cast<btRigidBody*>(object->getCollisionObject()));
            break;

        case PhysicsCollisionObject::CHARACTER:
        case PhysicsCollisionObject::GHOST_OBJECT:
            _world->removeCollisionObject(object->getCollisionObject());
            break;

        default:
            GP_ERROR("Unsupported collision object type (%d).", object->getType());
            break;
        }
    }

    // Find all references to the object in the collision status cache and mark them for removal.
    if (removeListeners)
    {
        std::map<PhysicsCollisionObject::CollisionPair, CollisionInfo>::iterator iter = _collisionStatus.begin();
        for (; iter != _collisionStatus.end(); iter++)
        {
            if (iter->first.objectA == object || iter->first.objectB == object)
                iter->second._status |= REMOVE;
        }
    }
}

PhysicsCollisionObject* PhysicsController::getCollisionObject(const btCollisionObject* collisionObject) const
{
    // Gameplay collision objects are stored in the userPointer data of Bullet collision objects.
    GP_ASSERT(collisionObject);
    return reinterpret_cast<PhysicsCollisionObject*>(collisionObject->getUserPointer());
}

static void getBoundingBox(Node* node, BoundingBox* out, bool merge = false)
{
    GP_ASSERT(node);
    GP_ASSERT(out);

    Model* model = dynamic_cast<Model*>(node->getDrawable());
    if (model != NULL)
    {
        GP_ASSERT(model->getMesh());

        if (merge)
            out->merge(model->getMesh()->getBoundingBox());
        else
        {
            out->set(model->getMesh()->getBoundingBox());
            merge = true;
        }
    }

    Node* child = node->getFirstChild();
    while (child)
    {
        getBoundingBox(child, out, merge);
        child = child->getNextSibling();
    }
}

static void getBoundingSphere(Node* node, BoundingSphere* out, bool merge = false)
{
    GP_ASSERT(node);
    GP_ASSERT(out);

    Model* model = dynamic_cast<Model*>(node->getDrawable());
    if (model != NULL)
    {
        GP_ASSERT(model->getMesh());

        if (merge)
            out->merge(model->getMesh()->getBoundingSphere());
        else
        {
            out->set(model->getMesh()->getBoundingSphere());
            merge = true;
        }
    }

    Node* child = node->getFirstChild();
    while (child)
    {
        getBoundingSphere(child, out, merge);
        child = child->getNextSibling();
    }
}

static void computeCenterOfMass(const Vector3& center, const Vector3& scale, Vector3* centerOfMassOffset)
{
    GP_ASSERT(centerOfMassOffset);

    // Update center of mass offset.
    *centerOfMassOffset = center;
    centerOfMassOffset->x *= scale.x;
    centerOfMassOffset->y *= scale.y;
    centerOfMassOffset->z *= scale.z;
    centerOfMassOffset->negate();
}

PhysicsCollisionShape* PhysicsController::createShape(Node* node, const PhysicsCollisionShape::Definition& shape, Vector3* centerOfMassOffset, bool dynamic)
{
    GP_ASSERT(node);

    PhysicsCollisionShape* collisionShape = NULL;

    // Get the node's world scale (we need to apply this during creation since rigid bodies don't scale dynamically).
    Vector3 scale;
    node->getWorldMatrix().getScale(&scale);

    switch (shape.type)
    {
    case PhysicsCollisionShape::SHAPE_BOX:
        {
            if (shape.isExplicit)
            {
                // Use the passed in box information.
                collisionShape = createBox(Vector3(shape.data.box.extents), Vector3::one());

                if (shape.centerAbsolute)
                {
                    computeCenterOfMass(Vector3(shape.data.box.center), Vector3::one(), centerOfMassOffset);
                }
                else
                {
                    BoundingBox box;
                    getBoundingBox(node, &box);
                    computeCenterOfMass(box.getCenter() + Vector3(shape.data.box.center), scale, centerOfMassOffset);
                }
            }
            else
            {
                // Automatically compute bounding box from mesh's bounding box.
                BoundingBox box;
                getBoundingBox(node, &box);
                collisionShape = createBox(Vector3(std::fabs(box.max.x - box.min.x), std::fabs(box.max.y - box.min.y), std::fabs(box.max.z - box.min.z)), scale);

                computeCenterOfMass(box.getCenter(), scale, centerOfMassOffset);
            }
        }
        break;

    case PhysicsCollisionShape::SHAPE_SPHERE:
        {
            if (shape.isExplicit)
            {
                // Use the passed in sphere information.
                collisionShape = createSphere(shape.data.sphere.radius, Vector3::one());

                if (shape.centerAbsolute)
                {
                    computeCenterOfMass(Vector3(shape.data.sphere.center), Vector3::one(), centerOfMassOffset);
                }
                else
                {
                    BoundingSphere sphere;
                    getBoundingSphere(node, &sphere);
                    computeCenterOfMass(sphere.center + Vector3(shape.data.sphere.center), scale, centerOfMassOffset);
                }
            }
            else
            {
                // Automatically compute bounding sphere from mesh's bounding sphere.
                BoundingSphere sphere;
                getBoundingSphere(node, &sphere);
                collisionShape = createSphere(sphere.radius, scale);

                computeCenterOfMass(sphere.center, scale, centerOfMassOffset);
            }
        }
        break;

    case PhysicsCollisionShape::SHAPE_CAPSULE:
        {
            if (shape.isExplicit)
            {
                // Use the passed in capsule information.
                collisionShape = createCapsule(shape.data.capsule.radius, shape.data.capsule.height, Vector3::one());

                if (shape.centerAbsolute)
                {
                    computeCenterOfMass(Vector3(shape.data.capsule.center), Vector3::one(), centerOfMassOffset);
                }
                else
                {
                    BoundingBox box;
                    getBoundingBox(node, &box);
                    computeCenterOfMass(box.getCenter() + Vector3(shape.data.capsule.center), scale, centerOfMassOffset);
                }
            }
            else
            {
                // Compute a capsule shape that roughly matches the bounding box of the mesh.
                BoundingBox box;
                getBoundingBox(node, &box);
                float radius = std::max((box.max.x - box.min.x) * 0.5f, (box.max.z - box.min.z) * 0.5f);
                float height = box.max.y - box.min.y;
                collisionShape = createCapsule(radius, height, scale);

                computeCenterOfMass(box.getCenter(), scale, centerOfMassOffset);
            }
        }
        break;

    case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
        {
            if (shape.isExplicit)
            {
                // Build heightfield rigid body from the passed in shape.
                collisionShape = createHeightfield(node, shape.data.heightfield, centerOfMassOffset);
            }
            else
            {
                // Build the heightfield from an attached terrain's height array
                if (dynamic_cast<Terrain*>(node->getDrawable()) == NULL)
                    GP_ERROR("Empty heightfield collision shapes can only be used on nodes that have an attached Terrain.");
                else
                    collisionShape = createHeightfield(node, dynamic_cast<Terrain*>(node->getDrawable())->_heightfield, centerOfMassOffset);
            }
        }
        break;

    case PhysicsCollisionShape::SHAPE_MESH:
        {
            // Build mesh from passed in shape.
            collisionShape = createMesh(shape.data.mesh, scale, dynamic);
        }
        break;

    default:
        GP_ERROR("Unsupported collision shape type (%d).", shape.type);
        break;
    }

    return collisionShape;
}

PhysicsCollisionShape* PhysicsController::createBox(const Vector3& extents, const Vector3& scale)
{
    btVector3 halfExtents(scale.x * 0.5 * extents.x, scale.y * 0.5 * extents.y, scale.z * 0.5 * extents.z);

    PhysicsCollisionShape* shape;

    // Return the box shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); ++i)
    {
        shape = _shapes[i];
        GP_ASSERT(shape);
        if (shape->getType() == PhysicsCollisionShape::SHAPE_BOX)
        {
            btBoxShape* box = static_cast<btBoxShape*>(shape->_shape);
            if (box && box->getHalfExtentsWithMargin() == halfExtents)
            {
                shape->addRef();
                return shape;
            }
        }
    }

    // Create the box shape and add it to the cache.
    shape = new PhysicsCollisionShape(PhysicsCollisionShape::SHAPE_BOX, bullet_new<btBoxShape>(halfExtents));
    _shapes.push_back(shape);

    return shape;
}

PhysicsCollisionShape* PhysicsController::createSphere(float radius, const Vector3& scale)
{
    // Since sphere shapes depend only on the radius, the best we can do is take
    // the largest dimension and apply that as the uniform scale to the rigid body.
    float uniformScale = scale.x;
    if (uniformScale < scale.y)
        uniformScale = scale.y;
    if (uniformScale < scale.z)
        uniformScale = scale.z;

    float scaledRadius = radius * uniformScale;

    PhysicsCollisionShape* shape;

    // Return the sphere shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); ++i)
    {
        shape = _shapes[i];
        GP_ASSERT(shape);
        if (shape->getType() == PhysicsCollisionShape::SHAPE_SPHERE)
        {
            btSphereShape* sphere = static_cast<btSphereShape*>(shape->_shape);
            if (sphere && sphere->getRadius() == scaledRadius)
            {
                shape->addRef();
                return shape;
            }
        }
    }

    // Create the sphere shape and add it to the cache.
    shape = new PhysicsCollisionShape(PhysicsCollisionShape::SHAPE_SPHERE, bullet_new<btSphereShape>(scaledRadius));
    _shapes.push_back(shape);

    return shape;
}

PhysicsCollisionShape* PhysicsController::createCapsule(float radius, float height, const Vector3& scale)
{
    float girthScale = scale.x;
    if (girthScale < scale.z)
        girthScale = scale.z;
    float scaledRadius = radius * girthScale;
    float scaledHeight = height * scale.y - radius * 2;

    PhysicsCollisionShape* shape;

    // Return the capsule shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); i++)
    {
        shape = _shapes[i];
        GP_ASSERT(shape);
        if (shape->getType() == PhysicsCollisionShape::SHAPE_CAPSULE)
        {
            btCapsuleShape* capsule = static_cast<btCapsuleShape*>(shape->_shape);
            if (capsule && capsule->getRadius() == scaledRadius && capsule->getHalfHeight() == 0.5f * scaledHeight)
            {
                shape->addRef();
                return shape;
            }
        }
    }

    // Create the capsule shape and add it to the cache.
    shape = new PhysicsCollisionShape(PhysicsCollisionShape::SHAPE_CAPSULE, bullet_new<btCapsuleShape>(scaledRadius, scaledHeight));
    _shapes.push_back(shape);

    return shape;
}

PhysicsCollisionShape* PhysicsController::createHeightfield(Node* node, HeightField* heightfield, Vector3* centerOfMassOffset)
{
    GP_ASSERT(node);
    GP_ASSERT(heightfield);
    GP_ASSERT(centerOfMassOffset);

    // Inspect the height array for the min and max values
    float* heights = heightfield->getArray();
    float minHeight = FLT_MAX, maxHeight = -FLT_MAX;
    for (unsigned int i = 0, count = heightfield->getColumnCount()*heightfield->getRowCount(); i < count; ++i)
    {
        float h = heights[i];
        if (h < minHeight)
            minHeight = h;
        if (h > maxHeight)
            maxHeight = h;
    }

    // Compute initial heightfield scale by pulling the current world scale out of the node
    Vector3 scale;
    node->getWorldMatrix().getScale(&scale);

    // If the node has a terrain, apply the terrain's local scale to the world scale
    Terrain* terrain = dynamic_cast<Terrain*>(node->getDrawable());
    if (terrain != NULL)
    {
        const Vector3& tScale = terrain->_localScale;
        scale.set(scale.x * tScale.x, scale.y * tScale.y, scale.z * tScale.z);
    }

    // Compute initial center of mass offset necessary to move the height from its position in bullet
    // physics (always centered around origin) to its intended location.
    centerOfMassOffset->set(0, -(minHeight + (maxHeight-minHeight)*0.5f) * scale.y, 0);

    // Create our heightfield data to be stored in the collision shape
    PhysicsCollisionShape::HeightfieldData* heightfieldData = new PhysicsCollisionShape::HeightfieldData();
    heightfieldData->heightfield = heightfield;
    heightfieldData->heightfield->addRef();
    heightfieldData->inverseIsDirty = true;
    heightfieldData->minHeight = minHeight;
    heightfieldData->maxHeight = maxHeight;

    // Create the bullet terrain shape
    btHeightfieldTerrainShape* terrainShape = bullet_new<btHeightfieldTerrainShape>(
        heightfield->getColumnCount(), heightfield->getRowCount(), heightfield->getArray(), 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, false);

    // Set initial bullet local scaling for the heightfield
    terrainShape->setLocalScaling(BV(scale));

    // Create our collision shape object and store heightfieldData in it.
    PhysicsCollisionShape* shape = new PhysicsCollisionShape(PhysicsCollisionShape::SHAPE_HEIGHTFIELD, terrainShape);
    shape->_shapeData.heightfieldData = heightfieldData;

    _shapes.push_back(shape);

    return shape;
}

PhysicsCollisionShape* PhysicsController::createMesh(Mesh* mesh, const Vector3& scale, bool dynamic)
{
    GP_ASSERT(mesh);

    // The mesh must have a valid URL (i.e. it must have been loaded from a Bundle)
    // in order to fetch mesh data for computing mesh rigid body.
    if (strlen(mesh->getUrl()) == 0)
    {
        GP_ERROR("Cannot create mesh rigid body for mesh without valid URL.");
        return NULL;
    }

    if (!dynamic)
    {
        // Static meshes use btBvhTriangleMeshShape and therefore only support triangle mesh shapes.
        // Dynamic meshes are approximated with a btConvexHullShape (convex wrapper on cloud of vertices)
        // and therefore can support any primitive type.
        bool triMesh = true;
        if (mesh->getPartCount() > 0)
        {
            for (unsigned int i = 0; i < mesh->getPartCount(); ++i)
            {
                if (mesh->getPart(i)->getPrimitiveType() != Mesh::TRIANGLES)
                {
                    triMesh = false;
                    break;
                }
            }
        }
        else
        {
            triMesh = mesh->getPrimitiveType() == Mesh::TRIANGLES;
        }

        if (!triMesh)
        {
            GP_ERROR("Mesh rigid bodies are currently only supported on meshes with TRIANGLES primitive type.");
            return NULL;
        }
    }

    // Read mesh data from URL
    Bundle::MeshData* data = Bundle::readMeshData(mesh->getUrl());
    if (data == NULL)
    {
        GP_ERROR("Failed to load mesh data from url '%s'.", mesh->getUrl());
        return NULL;
    }

    // Create mesh data to be populated and store in returned collision shape.
    PhysicsCollisionShape::MeshData* shapeMeshData = new PhysicsCollisionShape::MeshData();
    shapeMeshData->vertexData = NULL;

    // Copy the scaled vertex position data to the rigid body's local buffer.
    Matrix m;
    Matrix::createScale(scale, &m);
    unsigned int vertexCount = data->vertexCount;
    shapeMeshData->vertexData = new float[vertexCount * 3];
    Vector3 v;
    int vertexStride = data->vertexFormat.getVertexSize();
    for (unsigned int i = 0; i < data->vertexCount; i++)
    {
        v.set(*((float*)&data->vertexData[i * vertexStride + 0 * sizeof(float)]),
                *((float*)&data->vertexData[i * vertexStride + 1 * sizeof(float)]),
                *((float*)&data->vertexData[i * vertexStride + 2 * sizeof(float)]));
        v *= m;
        memcpy(&(shapeMeshData->vertexData[i * 3]), &v, sizeof(float) * 3);
    }

    btCollisionShape* collisionShape = NULL;
    btTriangleIndexVertexArray* meshInterface = NULL;

    if (dynamic)
    {
        // For dynamic meshes, use a btConvexHullShape approximation
        btConvexHullShape* originalConvexShape = bullet_new<btConvexHullShape>(shapeMeshData->vertexData, data->vertexCount, sizeof(float)*3);

        // Create a hull approximation for better performance
	    btShapeHull* hull = bullet_new<btShapeHull>(originalConvexShape);
	    hull->buildHull(originalConvexShape->getMargin());
	    collisionShape = bullet_new<btConvexHullShape>((btScalar*)hull->getVertexPointer(), hull->numVertices());

        SAFE_DELETE(hull);
        SAFE_DELETE(originalConvexShape);
    }
    else
    {
        // For static meshes, use btBvhTriangleMeshShape
        meshInterface = bullet_new<btTriangleIndexVertexArray>();

        size_t partCount = data->parts.size();
        if (partCount > 0)
        {
            PHY_ScalarType indexType = PHY_UCHAR;
            int indexStride = 0;
            Bundle::MeshPartData* meshPart = NULL;
            for (size_t i = 0; i < partCount; i++)
            {
                meshPart = data->parts[i];
                GP_ASSERT(meshPart);

                switch (meshPart->indexFormat)
                {
                case Mesh::INDEX8:
                    indexType = PHY_UCHAR;
                    indexStride = 1;
                    break;
                case Mesh::INDEX16:
                    indexType = PHY_SHORT;
                    indexStride = 2;
                    break;
                case Mesh::INDEX32:
                    indexType = PHY_INTEGER;
                    indexStride = 4;
                    break;
                default:
                    GP_ERROR("Unsupported index format (%d).", meshPart->indexFormat);
                    SAFE_DELETE(meshInterface);
                    SAFE_DELETE_ARRAY(shapeMeshData->vertexData);
                    SAFE_DELETE(shapeMeshData);
                    SAFE_DELETE(data);
                    return NULL;
                }

                // Move the index data into the rigid body's local buffer.
                // Set it to NULL in the MeshPartData so it is not released when the data is freed.
                shapeMeshData->indexData.push_back(meshPart->indexData);
                meshPart->indexData = NULL;

                // Create a btIndexedMesh object for the current mesh part.
                btIndexedMesh indexedMesh;
                indexedMesh.m_indexType = indexType;
                indexedMesh.m_numTriangles = meshPart->indexCount / 3; // assume TRIANGLES primitive type
                indexedMesh.m_numVertices = meshPart->indexCount;
                indexedMesh.m_triangleIndexBase = (const unsigned char*)shapeMeshData->indexData[i];
                indexedMesh.m_triangleIndexStride = indexStride*3;
                indexedMesh.m_vertexBase = (const unsigned char*)shapeMeshData->vertexData;
                indexedMesh.m_vertexStride = sizeof(float)*3;
                indexedMesh.m_vertexType = PHY_FLOAT;

                // Add the indexed mesh data to the mesh interface.
                meshInterface->addIndexedMesh(indexedMesh, indexType);
            }
        }
        else
        {
            // Generate index data for the mesh locally in the rigid body.
            unsigned int* indexData = new unsigned int[data->vertexCount];
            for (unsigned int i = 0; i < data->vertexCount; i++)
            {
                indexData[i] = i;
            }
            shapeMeshData->indexData.push_back((unsigned char*)indexData);

            // Create a single btIndexedMesh object for the mesh interface.
            btIndexedMesh indexedMesh;
            indexedMesh.m_indexType = PHY_INTEGER;
            indexedMesh.m_numTriangles = data->vertexCount / 3; // assume TRIANGLES primitive type
            indexedMesh.m_numVertices = data->vertexCount;
            indexedMesh.m_triangleIndexBase = shapeMeshData->indexData[0];
            indexedMesh.m_triangleIndexStride = sizeof(unsigned int);
            indexedMesh.m_vertexBase = (const unsigned char*)shapeMeshData->vertexData;
            indexedMesh.m_vertexStride = sizeof(float)*3;
            indexedMesh.m_vertexType = PHY_FLOAT;

            // Set the data in the mesh interface.
            meshInterface->addIndexedMesh(indexedMesh, indexedMesh.m_indexType);
        }

        // Create our collision shape object and store shapeMeshData in it.
        collisionShape = bullet_new<btBvhTriangleMeshShape>(meshInterface, true);
    }

    // Create our collision shape object and store shapeMeshData in it.
    PhysicsCollisionShape* shape = new PhysicsCollisionShape(PhysicsCollisionShape::SHAPE_MESH, collisionShape, meshInterface);
    shape->_shapeData.meshData = shapeMeshData;

    _shapes.push_back(shape);

    // Free the temporary mesh data now that it's stored in physics system.
    SAFE_DELETE(data);

    return shape;
}

void PhysicsController::destroyShape(PhysicsCollisionShape* shape)
{
    if (shape)
    {
        if (shape->getRefCount() == 1)
        {
            // Remove shape from shape cache.
            std::vector<PhysicsCollisionShape*>::iterator shapeItr = std::find(_shapes.begin(), _shapes.end(), shape);
            if (shapeItr != _shapes.end())
                _shapes.erase(shapeItr);
        }

        // Release the shape.
        shape->release();
    }
}

void PhysicsController::addConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint)
{
    GP_ASSERT(a);
    GP_ASSERT(constraint);
    GP_ASSERT(_world);

    a->addConstraint(constraint);
    if (b)
    {
        b->addConstraint(constraint);
    }
    
    _world->addConstraint(constraint->_constraint);
}

bool PhysicsController::checkConstraintRigidBodies(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    GP_ASSERT(a);

    if (!a->supportsConstraints())
    {
        GP_ASSERT(a->_node);
        GP_ERROR("Rigid body '%s' does not support constraints; unexpected behavior may occur.", a->_node->getId());
        return false;
    }
    
    if (b && !b->supportsConstraints())
    {
        GP_ASSERT(b->_node);
        GP_ERROR("Rigid body '%s' does not support constraints; unexpected behavior may occur.", b->_node->getId());
        return false;
    }

    return true;
}

void PhysicsController::removeConstraint(PhysicsConstraint* constraint)
{
    GP_ASSERT(constraint);
    GP_ASSERT(_world);

    // Find the constraint and remove it from the physics world.
    for (int i = _world->getNumConstraints() - 1; i >= 0; i--)
    {
        btTypedConstraint* currentConstraint = _world->getConstraint(i);
        if (constraint->_constraint == currentConstraint)
        {
            _world->removeConstraint(currentConstraint);
            break;
        }
    }
}

PhysicsController::DebugDrawer::DebugDrawer()
    : _mode(btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawConstraintLimits | btIDebugDraw::DBG_DrawConstraints | 
       btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe), _meshBatch(NULL), _lineCount(0)
{
    // Vertex shader for drawing colored lines.
    const char* vs_str = 
    {
        "uniform mat4 u_viewProjectionMatrix;\n"
        "attribute vec4 a_position;\n"
        "attribute vec4 a_color;\n"
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "    v_color = a_color;\n"
        "    gl_Position = u_viewProjectionMatrix * a_position;\n"
        "}"
    };

    // Fragment shader for drawing colored lines.
    const char* fs_str = 
    {
    #ifdef OPENGL_ES
        "precision highp float;\n"
    #endif
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "   gl_FragColor = v_color;\n"
        "}"
    };

    Effect* effect = Effect::createFromSource(vs_str, fs_str);
    Material* material = Material::create(effect);
    GP_ASSERT(material && material->getStateBlock());
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthFunction(RenderState::DEPTH_LEQUAL);

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
    };
    _meshBatch = MeshBatch::create(VertexFormat(elements, 2), Mesh::LINES, material, false, 4096, 4096);
    SAFE_RELEASE(material);
    SAFE_RELEASE(effect);
}

PhysicsController::DebugDrawer::~DebugDrawer()
{
    SAFE_DELETE(_meshBatch);
}

void PhysicsController::DebugDrawer::begin(const Matrix& viewProjection)
{
    GP_ASSERT(_meshBatch);
    _meshBatch->start();
    _meshBatch->getMaterial()->getParameter("u_viewProjectionMatrix")->setValue(viewProjection);
}

void PhysicsController::DebugDrawer::end()
{
    GP_ASSERT(_meshBatch && _meshBatch->getMaterial());
    _meshBatch->finish();
    _meshBatch->draw();
    _lineCount = 0;
}

void PhysicsController::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
    GP_ASSERT(_meshBatch);

    static DebugDrawer::DebugVertex vertices[2];

    vertices[0].x = from.getX();
    vertices[0].y = from.getY();
    vertices[0].z = from.getZ();
    vertices[0].r = fromColor.getX();
    vertices[0].g = fromColor.getY();
    vertices[0].b = fromColor.getZ();
    vertices[0].a = 1.0f;

    vertices[1].x = to.getX();
    vertices[1].y = to.getY();
    vertices[1].z = to.getZ();
    vertices[1].r = toColor.getX();
    vertices[1].g = toColor.getY();
    vertices[1].b = toColor.getZ();
    vertices[1].a = 1.0f;

    _meshBatch->add(vertices, 2);

    ++_lineCount;
    if (_lineCount >= 4096)
    {
        // Flush the batch when it gets full (don't want to to grow infinitely)
        end();
        _meshBatch->start();
    }
}

void PhysicsController::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    drawLine(from, to, color, color);
}

void PhysicsController::DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    drawLine(pointOnB, pointOnB + normalOnB, color);
}

void PhysicsController::DebugDrawer::reportErrorWarning(const char* warningString)
{
    GP_WARN(warningString);
}

void PhysicsController::DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    GP_WARN("Physics debug drawing: 3D text is not supported.");
}

void PhysicsController::DebugDrawer::setDebugMode(int mode)
{
    _mode = mode;
}

int PhysicsController::DebugDrawer::getDebugMode() const
{
    return _mode;
}

PhysicsController::Listener::~Listener()
{
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    Game::getInstance()->getPhysicsController()->removeStatusListener(this);
}

PhysicsController::HitFilter::HitFilter()
{
}

PhysicsController::HitFilter::~HitFilter()
{
}

bool PhysicsController::HitFilter::filter(PhysicsCollisionObject* object)
{
    return false;
}

bool PhysicsController::HitFilter::hit(const PhysicsController::HitResult& result)
{
    return true;
}

}
