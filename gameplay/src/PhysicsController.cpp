#include "Base.h"
#include "Game.h"
#include "MeshPart.h"
#include "PhysicsController.h"
#include "PhysicsMotionState.h"
#include "SceneLoader.h"

// The initial capacity of the Bullet debug drawer's vertex batch.
#define INITIAL_CAPACITY 280

namespace gameplay
{

PhysicsController::PhysicsController()
  : _collisionConfiguration(NULL), _dispatcher(NULL),
    _overlappingPairCache(NULL), _solver(NULL), _world(NULL), _debugDrawer(NULL), 
    _status(PhysicsController::Listener::DEACTIVATED), _listeners(NULL),
    _gravity(btScalar(0.0), btScalar(-9.8), btScalar(0.0))
{
    // Default gravity is 9.8 along the negative Y axis.
}

PhysicsController::~PhysicsController()
{
    SAFE_DELETE(_debugDrawer);
    SAFE_DELETE(_listeners);
}

void PhysicsController::addStatusListener(Listener* listener)
{
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    _listeners->push_back(listener);
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

const Vector3& PhysicsController::getGravity(const Vector3& gravity) const
{
    return _gravity;
}

void PhysicsController::setGravity(const Vector3& gravity)
{
    _gravity = gravity;

    if (_world)
        _world->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}

void PhysicsController::drawDebug(const Matrix& viewProjection)
{
    _debugDrawer->begin(viewProjection);
    _world->debugDrawWorld();
    _debugDrawer->end();
}

void PhysicsController::initialize()
{
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _overlappingPairCache = new btDbvtBroadphase();
    _solver = new btSequentialImpulseConstraintSolver();

    // Create the world.
    _world = new btDiscreteDynamicsWorld(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
    _world->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));

    // Set up debug drawing.
    _debugDrawer = new DebugDrawer();
    _world->setDebugDrawer(_debugDrawer);
}

void PhysicsController::finalize()
{
    // Clean up the world and its various components.
    SAFE_DELETE(_world);
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

    // Dirty all the collision listeners' collision status caches.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    iter->second |= PhysicsRigidBody::Listener::DIRTY;
                }
            }
        }
    }

    // Go through the physics rigid bodies and update the collision listeners.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    // If this collision pair was one that was registered for listening, then perform the collision test.
                    // (In the case where we register for all collisions with a rigid body, there will be a lot
                    // of collision pairs in the status cache that we did not explicitly register for.)
                    if ((iter->second & PhysicsRigidBody::Listener::REGISTERED) != 0)
                    {
                        if (iter->first._rbB)
                            Game::getInstance()->getPhysicsController()->_world->contactPairTest(iter->first._rbA->_body, iter->first._rbB->_body, *(*_bodies[i]->_listeners)[k]);
                        else
                            Game::getInstance()->getPhysicsController()->_world->contactTest(iter->first._rbA->_body, *(*_bodies[i]->_listeners)[k]);
                    }
                }   
            }
        }
    }

    // Go through all the collision listeners and update their collision status caches.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_listeners)
        {
            for (unsigned int k = 0; k < _bodies[i]->_listeners->size(); k++)
            {
                std::map<PhysicsRigidBody::CollisionPair, int>::iterator iter = (*_bodies[i]->_listeners)[k]->_collisionStatus.begin();
                for (; iter != (*_bodies[i]->_listeners)[k]->_collisionStatus.end(); iter++)
                {
                    if ((iter->second & PhysicsRigidBody::Listener::DIRTY) != 0)
                    {
                        iter->second &= ~PhysicsRigidBody::Listener::COLLISION;
                    }
                }
            }
        }
    }
}
    

void PhysicsController::addRigidBody(PhysicsRigidBody* body)
{
    _world->addRigidBody(body->_body);
    _bodies.push_back(body);
}
    
void PhysicsController::removeRigidBody(PhysicsRigidBody* rigidBody)
{
    // Find the rigid body and remove it from the world.
    for (int i = _world->getNumCollisionObjects() - 1; i >= 0 ; i--)
    {
        btCollisionObject* obj = _world->getCollisionObjectArray()[i];
        if (rigidBody->_body == obj)
        {
            _world->removeCollisionObject(obj);
            break;
        }
    }

    // Find the rigid body's collision shape and release the rigid body's reference to it.
    for (unsigned int i = 0; i < _shapes.size(); i++)
    {
        if (_shapes[i]->_shape == rigidBody->_shape)
        {
            if (_shapes[i]->getRefCount() == 1)
            {
                _shapes[i]->release();
                _shapes.erase(_shapes.begin() + i);
            }
            else
                _shapes[i]->release();

            return;
        }
    }
}

PhysicsRigidBody* PhysicsController::getRigidBody(const btCollisionObject* collisionObject)
{
    // Find the rigid body and remove it from the world.
    for (unsigned int i = 0; i < _bodies.size(); i++)
    {
        if (_bodies[i]->_body == collisionObject)
            return _bodies[i];
    }
    
    return NULL;
}

btCollisionShape* PhysicsController::createBox(const Vector3& min, const Vector3& max, const btVector3& scale)
{
    btVector3 halfExtents(scale.x() * 0.5 * abs(max.x - min.x), scale.y() * 0.5 * abs(max.y - min.y), scale.z() * 0.5 * abs(max.z - min.z));

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

btCollisionShape* PhysicsController::createSphere(float radius, const btVector3& scale)
{
    // Since sphere shapes depend only on the radius, the best we can do is take
    // the largest dimension and apply that as the uniform scale to the rigid body.
    float uniformScale = scale.x();
    if (uniformScale < scale.y())
        uniformScale = scale.y();
    if (uniformScale < scale.z())
        uniformScale = scale.z();
    
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

btCollisionShape* PhysicsController::createMesh(PhysicsRigidBody* body)
{
    // Retrieve the mesh rigid body data from the loaded scene.
    const SceneLoader::MeshRigidBodyData* data = SceneLoader::getMeshRigidBodyData(body->_node->getId());

    // Copy the scaled vertex position data to the rigid body's local buffer.
    Matrix m;
    Matrix::createScale(body->_node->getScaleX(), body->_node->getScaleY(), body->_node->getScaleZ(), &m);
    unsigned int vertexCount = data->mesh->getVertexCount();
    body->_vertexData = new float[vertexCount * 3];
    Vector3 v;
    int vertexStride = data->mesh->getVertexFormat().getVertexSize();
    for (unsigned int i = 0; i < vertexCount; i++)
    {
        v.set(*((float*)&data->vertexData[i * vertexStride + 0 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 1 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 2 * sizeof(float)]));
        v *= m;
        memcpy(&(body->_vertexData[i * 3]), &v, sizeof(float) * 3);
    }
    
    btTriangleIndexVertexArray* meshInterface = bullet_new<btTriangleIndexVertexArray>();

    if (data->mesh->getPartCount() > 0)
    {
        PHY_ScalarType indexType = PHY_UCHAR;
        int indexStride = 0;
        MeshPart* meshPart = NULL;
        for (unsigned int i = 0; i < data->mesh->getPartCount(); i++)
        {
            meshPart = data->mesh->getPart(i);

            switch (meshPart->getIndexFormat())
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

            // Copy the index data to the rigid body's local buffer.
            unsigned int indexDataSize = meshPart->getIndexCount() * indexStride;
            unsigned char* indexData = new unsigned char[indexDataSize];
            memcpy(indexData, data->indexData[i], indexDataSize);
            body->_indexData.push_back(indexData);

            // Create a btIndexedMesh object for the current mesh part.
            btIndexedMesh indexedMesh;
            indexedMesh.m_indexType = indexType;
            indexedMesh.m_numTriangles = meshPart->getIndexCount() / 3;
            indexedMesh.m_numVertices = meshPart->getIndexCount();
            indexedMesh.m_triangleIndexBase = (const unsigned char*)body->_indexData[i];
            indexedMesh.m_triangleIndexStride = indexStride;
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
        unsigned int* indexData = new unsigned int[data->mesh->getVertexCount()];
        for (unsigned int i = 0; i < data->mesh->getVertexCount(); i++)
        {
            indexData[i] = i;
        }
        body->_indexData.push_back((unsigned char*)indexData);

        // Create a single btIndexedMesh object for the mesh interface.
        btIndexedMesh indexedMesh;
        indexedMesh.m_indexType = PHY_INTEGER;
        indexedMesh.m_numTriangles = data->mesh->getVertexCount() / 3;
        indexedMesh.m_numVertices = data->mesh->getVertexCount();
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
    _meshBatch->getMaterial()->getParameter("u_viewProjectionMatrix")->setValue(_viewProjection);
    _meshBatch->draw();
    _meshBatch->end();
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

}
