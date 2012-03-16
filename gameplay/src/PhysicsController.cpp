#include "Base.h"
#include "Game.h"
#include "MeshPart.h"
#include "PhysicsController.h"
#include "PhysicsMotionState.h"
#include "Package.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// The initial capacity of the Bullet debug drawer's vertex batch.
#define INITIAL_CAPACITY 280

namespace gameplay
{

const int PhysicsController::DIRTY         = 0x01;
const int PhysicsController::COLLISION     = 0x02;
const int PhysicsController::REGISTERED    = 0x04;
const int PhysicsController::REMOVE        = 0x08;

PhysicsController::PhysicsController()
  : _collisionConfiguration(NULL), _dispatcher(NULL),
    _overlappingPairCache(NULL), _solver(NULL), _world(NULL), _ghostPairCallback(NULL),
    _debugDrawer(NULL), _status(PhysicsController::Listener::DEACTIVATED), _listeners(NULL),
    _gravity(btScalar(0.0), btScalar(-9.8), btScalar(0.0))
{
    // Default gravity is 9.8 along the negative Y axis.
}

PhysicsController::~PhysicsController()
{
    SAFE_DELETE(_ghostPairCallback);
    SAFE_DELETE(_debugDrawer);
    SAFE_DELETE(_listeners);
}

void PhysicsController::addStatusListener(Listener* listener)
{
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    _listeners->push_back(listener);
}

PhysicsCharacter* PhysicsController::createCharacter(Node* node, float radius, float height, const Vector3& center)
{
    return new PhysicsCharacter(node, radius, height, center);
}

void PhysicsController::destroyCharacter(PhysicsCharacter* character)
{
    SAFE_DELETE(character);
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
    _debugDrawer->begin(viewProjection);
    _world->debugDrawWorld();
    _debugDrawer->end();
}

PhysicsCollisionObject* PhysicsController::rayTest(const Ray& ray, float distance, Vector3* hitPoint, float* hitFraction)
{
    btCollisionWorld::ClosestRayResultCallback callback(BV(ray.getOrigin()), BV(distance * ray.getDirection()));
    _world->rayTest(BV(ray.getOrigin()), BV(distance * ray.getDirection()), callback);
    if (callback.hasHit())
    {
        if (hitPoint)
            hitPoint->set(callback.m_hitPointWorld.x(), callback.m_hitPointWorld.y(), callback.m_hitPointWorld.z());

        if (hitFraction)
            *hitFraction = callback.m_closestHitFraction;

        return getCollisionObject(callback.m_collisionObject);
    }

    return NULL;
}

btScalar PhysicsController::addSingleResult(btManifoldPoint& cp, const btCollisionObject* a, int partIdA, int indexA, 
    const btCollisionObject* b, int partIdB, int indexB)
{
    // Get pointers to the PhysicsRigidBody objects.
    PhysicsCollisionObject* rbA = Game::getInstance()->getPhysicsController()->getCollisionObject(a);
    PhysicsCollisionObject* rbB = Game::getInstance()->getPhysicsController()->getCollisionObject(b);

    // If the given rigid body pair has collided in the past, then
    // we notify the listeners only if the pair was not colliding
    // during the previous frame. Otherwise, it's a new pair, so add a
    // new entry to the cache with the appropriate listeners and notify them.
    PhysicsRigidBody::CollisionPair pair(rbA, rbB);

    CollisionInfo* collisionInfo;
    if (_collisionStatus.count(pair) > 0)
    {
        collisionInfo = &_collisionStatus[pair];
    }
    else
    {
        // Add a new collision pair for these objects
        collisionInfo = &_collisionStatus[pair];

        // Add the appropriate listeners.
        PhysicsRigidBody::CollisionPair p1(pair.objectA, NULL);
        if (_collisionStatus.count(p1) > 0)
        {
            const CollisionInfo& ci = _collisionStatus[p1];
            std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = ci._listeners.begin();
            for (; iter != ci._listeners.end(); iter++)
            {
                collisionInfo->_listeners.push_back(*iter);
            }
        }
        PhysicsRigidBody::CollisionPair p2(pair.objectB, NULL);
        if (_collisionStatus.count(p2) > 0)
        {
            const CollisionInfo& ci = _collisionStatus[p2];
            std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = ci._listeners.begin();
            for (; iter != ci._listeners.end(); iter++)
            {
                collisionInfo->_listeners.push_back(*iter);
            }
        }
    }

    // Fire collision event
    if ((collisionInfo->_status & COLLISION) == 0)
    {
        std::vector<PhysicsCollisionObject::CollisionListener*>::const_iterator iter = collisionInfo->_listeners.begin();
        for (; iter != collisionInfo->_listeners.end(); iter++)
        {
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
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _overlappingPairCache = new btDbvtBroadphase();
    _solver = new btSequentialImpulseConstraintSolver();

    // Create the world.
    _world = new btDiscreteDynamicsWorld(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
    _world->setGravity(BV(_gravity));

    // Register ghost pair callback so bullet detects collisions with ghost objects (used for character collisions).
    _ghostPairCallback = bullet_new<btGhostPairCallback>();
    _world->getPairCache()->setInternalGhostPairCallback(_ghostPairCallback);

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

void PhysicsController::update(long elapsedTime)
{
    // Update the physics simulation, with a maximum
    // of 10 simulation steps being performed in a given frame.
    //
    // Note that stepSimulation takes elapsed time in seconds
    // so we divide by 1000 to convert from milliseconds.
    _world->stepSimulation((float)elapsedTime * 0.001, 10);

    // If we have status listeners, then check if our status has changed.
    if (_listeners)
    {
        Listener::EventType oldStatus = _status;

        if (_status = Listener::DEACTIVATED)
        {
            for (int i = 0; i < _world->getNumCollisionObjects(); i++)
            {
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
            for (unsigned int k = 0; k < _listeners->size(); k++)
            {
                (*_listeners)[k]->statusEvent(_status);
            }
        }
    }

    // All statuses are set with the DIRTY bit before collision processing occurs.
    // During collision processing, if a collision occurs, the status is 
    // set to COLLISION and the DIRTY bit is cleared. Then, after collision processing 
    // is finished, if a given status is still dirty, the COLLISION bit is cleared.
    //
    // If an entry was marked for removal in the last frame, remove it now.

    // Dirty the collision status cache entries.
    std::map<PhysicsRigidBody::CollisionPair, CollisionInfo>::iterator iter = _collisionStatus.begin();
    for (; iter != _collisionStatus.end();)
    {
        if ((iter->second._status & REMOVE) != 0)
        {
            std::map<PhysicsRigidBody::CollisionPair, CollisionInfo>::iterator eraseIter = iter;
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
                _world->contactPairTest(iter->first.objectA->getCollisionObject(), iter->first.objectB->getCollisionObject(), *this);
            else
                _world->contactTest(iter->first.objectA->getCollisionObject(), *this);
        }
    }

    // Update all the collision status cache entries.
    iter = _collisionStatus.begin();
    for (; iter != _collisionStatus.end(); iter++)
    {
        if ((iter->second._status & DIRTY) != 0)
        {
            if ((iter->second._status & COLLISION) != 0 && iter->first.objectB)
            {
                unsigned int size = iter->second._listeners.size();
                for (unsigned int i = 0; i < size; i++)
                {
                    iter->second._listeners[i]->collisionEvent(PhysicsCollisionObject::CollisionListener::NOT_COLLIDING, iter->first);
                }
            }

            iter->second._status &= ~COLLISION;
        }
    }
}

void PhysicsController::addCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
{
    PhysicsCollisionObject::CollisionPair pair(objectA, objectB);

    // Add the listener and ensure the status includes that this collision pair is registered.
    CollisionInfo& info = _collisionStatus[pair];
    info._listeners.push_back(listener);
    info._status |= PhysicsController::REGISTERED;
}

void PhysicsController::removeCollisionListener(PhysicsCollisionObject::CollisionListener* listener, PhysicsCollisionObject* objectA, PhysicsCollisionObject* objectB)
{
    // Mark the collision pair for these objects for removal
    PhysicsCollisionObject::CollisionPair pair(objectA, objectB);
    if (_collisionStatus.count(pair) > 0)
    {
        _collisionStatus[pair]._status |= REMOVE;
    }
}

void PhysicsController::addCollisionObject(PhysicsCollisionObject* object)
{
    // Assign user pointer for the bullet collision object to allow efficient
    // lookups of bullet objects -> gameplay objects.
    object->getCollisionObject()->setUserPointer(object);

    // Add the object to the physics world
    switch (object->getType())
    {
    case PhysicsCollisionObject::RIGID_BODY:
        _world->addRigidBody(static_cast<btRigidBody*>(object->getCollisionObject()));
        break;

    case PhysicsCollisionObject::CHARACTER:
        _world->addCollisionObject(object->getCollisionObject(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter);
        break;

    case PhysicsCollisionObject::GHOST_OBJECT:
        _world->addCollisionObject(object->getCollisionObject(), btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter);
        break;

    default:
        assert(0); // unexpected (new type?)
        break;
    }
}

void PhysicsController::removeCollisionObject(PhysicsCollisionObject* object)
{
    // Remove the collision object from the world
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
            assert(0); // unexpected (new type?)
            break;
        }
    }

    // Release collision shape
    if (object->getCollisionShape())
    {
        PhysicsCollisionShape* shape = reinterpret_cast<PhysicsCollisionShape*>(object->getCollisionShape()->getUserPointer());
        if (shape)
        {
            if (shape->getRefCount() == 1)
            {
                std::vector<PhysicsCollisionShape*>::iterator shapeItr = std::find(_shapes.begin(), _shapes.end(), shape);
                shape->release();
                if (shapeItr != _shapes.end())
                    _shapes.erase(shapeItr);
            }
            else
            {
                shape->release();
            }
        }
    }

    // Find all references to the object in the collision status cache and mark them for removal.
    std::map<PhysicsRigidBody::CollisionPair, CollisionInfo>::iterator iter = _collisionStatus.begin();
    for (; iter != _collisionStatus.end(); iter++)
    {
        if (iter->first.objectA == object || iter->first.objectB == object)
            iter->second._status |= REMOVE;
    }
}

PhysicsCollisionObject* PhysicsController::getCollisionObject(const btCollisionObject* collisionObject) const
{
    // Gameplay rigid bodies are stored in the userPointer data of bullet collision objects
    return reinterpret_cast<PhysicsCollisionObject*>(collisionObject->getUserPointer());
}

btCollisionShape* PhysicsController::createBox(const Vector3& min, const Vector3& max, const Vector3& scale)
{
    btVector3 halfExtents(scale.x * 0.5 * abs(max.x - min.x), scale.y * 0.5 * abs(max.y - min.y), scale.z * 0.5 * abs(max.z - min.z));

    // Return the box shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); i++)
    {
        if (_shapes[i]->_shape->getShapeType() == BOX_SHAPE_PROXYTYPE)
        {
            btBoxShape* box = static_cast<btBoxShape*>(_shapes[i]->_shape);
            if (box->getHalfExtentsWithMargin() == halfExtents)
            {
                _shapes[i]->addRef();
                return box;
            }
        }
    }
    
    // Create the box shape and add it to the cache.
    btBoxShape* box = bullet_new<btBoxShape>(halfExtents);
    _shapes.push_back(new PhysicsCollisionShape(box));

    return box;
}

btCollisionShape* PhysicsController::createCapsule(float radius, float height)
{
    // Return the capsule shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); i++)
    {
        if (_shapes[i]->_shape->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
        {
            btCapsuleShape* capsule = static_cast<btCapsuleShape*>(_shapes[i]->_shape);
            if (capsule->getRadius() == radius && capsule->getHalfHeight() == 0.5f * height)
            {
                _shapes[i]->addRef();
                return capsule;
            }
        }
    }
    
    // Create the capsule shape and add it to the cache.
    btCapsuleShape* capsule = bullet_new<btCapsuleShape>(radius, height);
    _shapes.push_back(new PhysicsCollisionShape(capsule));

    return capsule;
}

btCollisionShape* PhysicsController::createSphere(float radius, const Vector3& scale)
{
    // Since sphere shapes depend only on the radius, the best we can do is take
    // the largest dimension and apply that as the uniform scale to the rigid body.
    float uniformScale = scale.x;
    if (uniformScale < scale.y)
        uniformScale = scale.y;
    if (uniformScale < scale.z)
        uniformScale = scale.z;
    
    // Return the sphere shape from the cache if it already exists.
    for (unsigned int i = 0; i < _shapes.size(); i++)
    {
        if (_shapes[i]->_shape->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
        {
            btSphereShape* sphere = static_cast<btSphereShape*>(_shapes[i]->_shape);
            if (sphere->getRadius() == uniformScale * radius)
            {
                _shapes[i]->addRef();
                return sphere;
            }
        }
    }

    // Create the sphere shape and add it to the cache.
    btSphereShape* sphere = bullet_new<btSphereShape>(uniformScale * radius);
    _shapes.push_back(new PhysicsCollisionShape(sphere));

    return sphere;
}

btCollisionShape* PhysicsController::createMesh(PhysicsRigidBody* body, const Vector3& scale)
{
    assert(body);

    // Retrieve the mesh rigid body data from the node's mesh.
    Model* model = body->_node ? body->_node->getModel() : NULL;
    Mesh* mesh = model ? model->getMesh() : NULL;
    if (mesh == NULL)
    {
        LOG_ERROR("Cannot create mesh rigid body for node without model/mesh.");
        return NULL;
    }

    // Only support meshes with triangle list primitive types
    if (mesh->getPrimitiveType() != Mesh::TRIANGLES)
    {
        LOG_ERROR("Cannot create mesh rigid body for mesh without TRIANGLES primitive type.");
        return NULL;
    }

    // The mesh must have a valid URL (i.e. it must have been loaded from a Package)
    // in order to fetch mesh data for computing mesh rigid body.
    if (strlen(mesh->getUrl()) == 0)
    {
        LOG_ERROR("Cannot create mesh rigid body for mesh without valid URL.");
        return NULL;
    }

    Package::MeshData* data = Package::readMeshData(mesh->getUrl());
    if (data == NULL)
    {
        return NULL;
    }

    // Copy the scaled vertex position data to the rigid body's local buffer.
    Matrix m;
    Matrix::createScale(scale, &m);
    unsigned int vertexCount = data->vertexCount;
    body->_vertexData = new float[vertexCount * 3];
    Vector3 v;
    int vertexStride = data->vertexFormat.getVertexSize();
    for (unsigned int i = 0; i < data->vertexCount; i++)
    {
        v.set(*((float*)&data->vertexData[i * vertexStride + 0 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 1 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 2 * sizeof(float)]));
        v *= m;
        memcpy(&(body->_vertexData[i * 3]), &v, sizeof(float) * 3);
    }

    btTriangleIndexVertexArray* meshInterface = bullet_new<btTriangleIndexVertexArray>();

    unsigned int partCount = data->parts.size();
    if (partCount > 0)
    {
        PHY_ScalarType indexType = PHY_UCHAR;
        int indexStride = 0;
        Package::MeshPartData* meshPart = NULL;
        for (unsigned int i = 0; i < partCount; i++)
        {
            meshPart = data->parts[i];

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
            }

            // Move the index data into the rigid body's local buffer.
            // Set it to NULL in the MeshPartData so it is not released when the data is freed.
            body->_indexData.push_back(meshPart->indexData);
            meshPart->indexData = NULL;

            // Create a btIndexedMesh object for the current mesh part.
            btIndexedMesh indexedMesh;
            indexedMesh.m_indexType = indexType;
            indexedMesh.m_numTriangles = meshPart->indexCount / 3; // assume TRIANGLES primitive type
            indexedMesh.m_numVertices = meshPart->indexCount;
            indexedMesh.m_triangleIndexBase = (const unsigned char*)body->_indexData[i];
            indexedMesh.m_triangleIndexStride = indexStride*3;
            indexedMesh.m_vertexBase = (const unsigned char*)body->_vertexData;
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
        body->_indexData.push_back((unsigned char*)indexData);

        // Create a single btIndexedMesh object for the mesh interface.
        btIndexedMesh indexedMesh;
        indexedMesh.m_indexType = PHY_INTEGER;
        indexedMesh.m_numTriangles = data->vertexCount / 3; // assume TRIANGLES primitive type
        indexedMesh.m_numVertices = data->vertexCount;
        indexedMesh.m_triangleIndexBase = body->_indexData[0];
        indexedMesh.m_triangleIndexStride = sizeof(unsigned int);
        indexedMesh.m_vertexBase = (const unsigned char*)body->_vertexData;
        indexedMesh.m_vertexStride = sizeof(float)*3;
        indexedMesh.m_vertexType = PHY_FLOAT;

        // Set the data in the mesh interface.
        meshInterface->addIndexedMesh(indexedMesh, indexedMesh.m_indexType);
    }

    btBvhTriangleMeshShape* shape = bullet_new<btBvhTriangleMeshShape>(meshInterface, true);
    _shapes.push_back(new PhysicsCollisionShape(shape));

    // Free the temporary mesh data now that it's stored in physics system
    SAFE_DELETE(data);

    return shape;
}

btCollisionShape* PhysicsController::createHeightfield(int width, int height, void* heightfieldData, float minHeight, float maxHeight)
{
    btCollisionShape* shape = bullet_new<btHeightfieldTerrainShape>(width, height, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, false);
    _shapes.push_back(new PhysicsCollisionShape(shape));
    return shape;
}

void PhysicsController::addConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b, PhysicsConstraint* constraint)
{
    a->addConstraint(constraint);
    if (b)
    {
        b->addConstraint(constraint);
    }
    
    _world->addConstraint(constraint->_constraint);
}

bool PhysicsController::checkConstraintRigidBodies(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    if (!a->supportsConstraints())
    {
        WARN_VARG("Rigid body '%s' does not support constraints; unexpected behavior may occur.", a->_node->getId());
        return false;
    }
    
    if (b && !b->supportsConstraints())
    {
        WARN_VARG("Rigid body '%s' does not support constraints; unexpected behavior may occur.", b->_node->getId());
        return false;
    }

    return true;
}

void PhysicsController::removeConstraint(PhysicsConstraint* constraint)
{
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
       btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe), _viewProjection(NULL), _meshBatch(NULL)
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
    material->getStateBlock()->setDepthTest(true);

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
    };
    _meshBatch = MeshBatch::create(VertexFormat(elements, 2), Mesh::LINES, material, false);
    
    SAFE_RELEASE(material);
    SAFE_RELEASE(effect);
}

PhysicsController::DebugDrawer::~DebugDrawer()
{
    SAFE_DELETE(_meshBatch);
}

void PhysicsController::DebugDrawer::begin(const Matrix& viewProjection)
{
    _viewProjection = &viewProjection;
    _meshBatch->begin();
}

void PhysicsController::DebugDrawer::end()
{
    _meshBatch->end();
    _meshBatch->getMaterial()->getParameter("u_viewProjectionMatrix")->setValue(_viewProjection);
    _meshBatch->draw();
}

void PhysicsController::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
    static DebugDrawer::DebugVertex fromVertex, toVertex;
    
    fromVertex.x = from.getX();
    fromVertex.y = from.getY();
    fromVertex.z = from.getZ();
    fromVertex.r = fromColor.getX();
    fromVertex.g = fromColor.getY();
    fromVertex.b = fromColor.getZ();
    fromVertex.a = 1.0f;

    toVertex.x = to.getX();
    toVertex.y = to.getY();
    toVertex.z = to.getZ();
    toVertex.r = toColor.getX();
    toVertex.g = toColor.getY();
    toVertex.b = toColor.getZ();
    toVertex.a = 1.0f;

    _meshBatch->add(&fromVertex, 1);
    _meshBatch->add(&toVertex, 1);
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
    WARN(warningString);
}

void PhysicsController::DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    WARN("Physics debug drawing: 3D text is not supported.");
}

void PhysicsController::DebugDrawer::setDebugMode(int mode)
{
    _mode = mode;
}

int	PhysicsController::DebugDrawer::getDebugMode() const
{
    return _mode;
}

PhysicsController::PhysicsCollisionShape::PhysicsCollisionShape(btCollisionShape* shape)
    : _shape(shape)
{
    // Assign user pointer to allow efficient lookup of PhysicsCollisionShape from bullet object
    shape->setUserPointer(this);
}

PhysicsController::PhysicsCollisionShape::~PhysicsCollisionShape()
{
    SAFE_DELETE(_shape);
}

}
