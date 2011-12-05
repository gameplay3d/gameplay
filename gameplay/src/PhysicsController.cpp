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
    PhysicsFixedConstraint* constraint = new PhysicsFixedConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsGenericConstraint* PhysicsController::createGenericConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b,
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsGenericConstraint* constraint = new PhysicsGenericConstraint(a, rotationOffsetA, translationOffsetA, 
                                                                        b, rotationOffsetB, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsHingeConstraint* PhysicsController::createHingeConstraint(PhysicsRigidBody* a,
    const Quaternion& rotationOffsetA, const Vector3& translationOffsetA, PhysicsRigidBody* b, 
    const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsHingeConstraint* constraint = new PhysicsHingeConstraint(a, rotationOffsetA, translationOffsetA, 
                                                                    b, rotationOffsetB, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSocketConstraint* PhysicsController::createSocketConstraint(PhysicsRigidBody* a,
    const Vector3& translationOffsetA, PhysicsRigidBody* b, const Vector3& translationOffsetB)
{
    PhysicsSocketConstraint* constraint = new PhysicsSocketConstraint(a,translationOffsetA, 
                                                                      b, translationOffsetB);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a, PhysicsRigidBody* b)
{
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, b);
    addConstraint(a, b, constraint);
    return constraint;
}

PhysicsSpringConstraint* PhysicsController::createSpringConstraint(PhysicsRigidBody* a, const Quaternion& rotationOffsetA, const Vector3& translationOffsetA,           
                                                                   PhysicsRigidBody* b, const Quaternion& rotationOffsetB, const Vector3& translationOffsetB)
{
    PhysicsSpringConstraint* constraint = new PhysicsSpringConstraint(a, rotationOffsetA, translationOffsetA, 
                                                                      b, rotationOffsetB, translationOffsetB);
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
    BULLET_NEW_VARG(btBoxShape, box, halfExtents);
    _shapes.push_back(box);

    return box;
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
    
    BULLET_NEW_VARG(btSphereShape, sphere, uniformScale * radius);
    _shapes.push_back(sphere);
    
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
    int vertexStride = data->mesh->getVertexFormat()->getVertexSize();
    for (unsigned int i = 0; i < vertexCount; i++)
    {
        v.set(*((float*)&data->vertexData[i * vertexStride + 0 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 1 * sizeof(float)]),
              *((float*)&data->vertexData[i * vertexStride + 2 * sizeof(float)]));
        v *= m;
        memcpy(&(body->_vertexData[i * 3]), &v, sizeof(float) * 3);
    }
    
    BULLET_NEW(btTriangleIndexVertexArray, meshInterface);

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

    BULLET_NEW_VARG(btBvhTriangleMeshShape, shape, meshInterface, true);
    _shapes.push_back(shape);

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
       btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe), _effect(NULL), _positionAttrib(0), _colorAttrib(0),
       _viewProjectionMatrixUniform(NULL), _viewProjection(NULL), _vertexData(NULL), _vertexCount(0), _vertexDataSize(0)
{
    // Unused
}

PhysicsController::DebugDrawer::~DebugDrawer()
{
    SAFE_DELETE_ARRAY(_vertexData);
}

void PhysicsController::DebugDrawer::begin(const Matrix& viewProjection)
{
    _viewProjection = &viewProjection;
    _vertexCount = 0;
}

void PhysicsController::DebugDrawer::end()
{
    // Lazy load the effect for drawing.
    if (!_effect)
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
        
        _effect = Effect::createFromSource(vs_str, fs_str);
        _positionAttrib = _effect->getVertexAttribute("a_position");
        _colorAttrib = _effect->getVertexAttribute("a_color");
        _viewProjectionMatrixUniform = _effect->getUniform("u_viewProjectionMatrix");
    }
    
    // Bind the effect and set the vertex attributes.
    _effect->bind();
    GL_ASSERT( glEnableVertexAttribArray(_positionAttrib) );
    GL_ASSERT( glEnableVertexAttribArray(_colorAttrib) );
    GL_ASSERT( glVertexAttribPointer(_positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, _vertexData) );
    GL_ASSERT( glVertexAttribPointer(_colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, &_vertexData[3]) );
    
    // Set the camera's view projection matrix and draw.
    _effect->setValue( _viewProjectionMatrixUniform, _viewProjection);
    GL_ASSERT( glDrawArrays(GL_LINES, 0, _vertexCount / 7) );
}

void PhysicsController::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
    // Allocate extra space in the vertex data batch if it is needed.
    if (_vertexDataSize - _vertexCount < 14)
    {
        if (_vertexDataSize > 0)
        {
            unsigned int newVertexDataSize = _vertexDataSize * 2;
            float* newVertexData = new float[newVertexDataSize];
            memcpy(newVertexData, _vertexData, _vertexDataSize * sizeof(float));
            SAFE_DELETE_ARRAY(_vertexData);
            _vertexData = newVertexData;
            _vertexDataSize = newVertexDataSize;
        }
        else
        {
            _vertexDataSize = INITIAL_CAPACITY;
            _vertexData = new float[_vertexDataSize];
        }
    }
    
    // Create the vertex data for the line and copy it into the batch.
    float vertexData[] = 
    {
        from.getX(), from.getY(), from.getZ(), 
        fromColor.getX(), fromColor.getY(), fromColor.getZ(), 1.0f,
        to.getX(), to.getY(), to.getZ(),
        toColor.getX(), toColor.getY(), toColor.getZ(), 1.0f
    };
    memcpy(&_vertexData[_vertexCount], vertexData, sizeof(float) * 14);
    _vertexCount += 14;
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
