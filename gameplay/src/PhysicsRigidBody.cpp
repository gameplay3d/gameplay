#include "Base.h"
#include "Game.h"
#include "Image.h"
#include "PhysicsController.h"
#include "PhysicsMotionState.h"
#include "PhysicsRigidBody.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

namespace gameplay
{

const int PhysicsRigidBody::Listener::DIRTY         = 0x01;
const int PhysicsRigidBody::Listener::COLLISION     = 0x02;
const int PhysicsRigidBody::Listener::REGISTERED    = 0x04;

// Internal values used for creating mesh and heightfield rigid bodies.
#define SHAPE_MESH ((PhysicsRigidBody::Type)(PhysicsRigidBody::SHAPE_NONE + 1))
#define SHAPE_HEIGHTFIELD ((PhysicsRigidBody::Type)(PhysicsRigidBody::SHAPE_NONE + 2))

// Helper function for calculating heights from heightmap (image) or heightfield data.
static float calculateHeight(float* data, unsigned int width, unsigned int height, float x, float y);

PhysicsRigidBody::PhysicsRigidBody(Node* node, PhysicsRigidBody::Type type, float mass, 
        float friction, float restitution, float linearDamping, float angularDamping)
        : _shape(NULL), _body(NULL), _node(node), _listeners(NULL), _angularVelocity(NULL),
        _anisotropicFriction(NULL), _gravity(NULL), _linearVelocity(NULL), _vertexData(NULL),
        _indexData(NULL), _heightfieldData(NULL), _inverse(NULL), _inverseIsDirty(true)
{
    switch (type)
    {
        case SHAPE_BOX:
        {
            const BoundingBox& box = node->getModel()->getMesh()->getBoundingBox();
            _shape = Game::getInstance()->getPhysicsController()->createBox(box.min, box.max, btVector3(node->getScaleX(), node->getScaleY(), node->getScaleZ()));
            break;
        }
        case SHAPE_SPHERE:
        {
            const BoundingSphere& sphere = node->getModel()->getMesh()->getBoundingSphere();
            _shape = Game::getInstance()->getPhysicsController()->createSphere(sphere.radius, btVector3(node->getScaleX(), node->getScaleY(), node->getScaleZ()));
            break;
        }
        case SHAPE_MESH:
        {
            _shape = Game::getInstance()->getPhysicsController()->createMesh(this);
            break;
        }
    }

    // Use the center of the bounding sphere as the center of mass offset.
    Vector3 c(node->getModel()->getMesh()->getBoundingSphere().center);
    c.negate();

    // Create the Bullet rigid body.
    if (c.lengthSquared() > MATH_EPSILON)
        _body = createRigidBodyInternal(_shape, mass, node, friction, restitution, linearDamping, angularDamping, &c);
    else
        _body = createRigidBodyInternal(_shape, mass, node, friction, restitution, linearDamping, angularDamping);

    // Add the rigid body to the physics world.
    Game::getInstance()->getPhysicsController()->addRigidBody(this);
}

PhysicsRigidBody::PhysicsRigidBody(Node* node, Image* image, float mass,
    float friction, float restitution, float linearDamping, float angularDamping)
        : _shape(NULL), _body(NULL), _node(node), _listeners(NULL), _angularVelocity(NULL),
        _anisotropicFriction(NULL), _gravity(NULL), _linearVelocity(NULL), _vertexData(NULL),
        _indexData(NULL), _heightfieldData(NULL), _inverse(NULL), _inverseIsDirty(true)
{
    // Get the width, length and minimum and maximum height of the heightfield.
    const BoundingBox& box = node->getModel()->getMesh()->getBoundingBox();
    float width = box.max.x - box.min.x;
    float minHeight = box.min.y;
    float maxHeight = box.max.y;
    float length = box.max.z - box.min.z;

    // Get the size in bytes of a pixel (we ensure that the image's
    // pixel format is actually supported before calling this constructor).
    unsigned int pixelSize = 0;
    switch (image->getFormat())
    {
        case Image::RGB888:
            pixelSize = 3;
            break;
        case Image::RGBA8888:
            pixelSize = 4;
            break;
    }

    // Calculate the heights for each pixel.
    float* data = new float[image->getWidth() * image->getHeight()];
    for (unsigned int x = 0; x < image->getWidth(); x++)
    {
        for (unsigned int y = 0; y < image->getHeight(); y++)
        {
            data[x + y * image->getWidth()] = ((((float)image->getData()[(x + y * image->getHeight()) * pixelSize + 0]) +
                ((float)image->getData()[(x + y * image->getHeight()) * pixelSize + 1]) +
                ((float)image->getData()[(x + y * image->getHeight()) * pixelSize + 2])) / 768.0f) * (maxHeight - minHeight) + minHeight;
        }
    }

    // Generate the heightmap data needed for physics (one height per world unit).
    unsigned int sizeWidth = width;
    unsigned int sizeHeight = length;
    _width = sizeWidth + 1;
    _height = sizeHeight + 1;
    _heightfieldData = new float[_width * _height];
    unsigned int heightIndex = 0;
    float widthImageFactor = (float)(image->getWidth() - 1) / sizeWidth;
    float heightImageFactor = (float)(image->getHeight() - 1) / sizeHeight;
    float x = 0.0f;
    float z = 0.0f;
    for (unsigned int row = 0, z = 0.0f; row <= sizeHeight; row++, z += 1.0f)
    {
        for (unsigned int col = 0, x = 0.0f; col <= sizeWidth; col++, x += 1.0f)
        {
            heightIndex = row * _width + col;
            _heightfieldData[heightIndex] = calculateHeight(data, image->getWidth(), image->getHeight(), x * widthImageFactor, (sizeHeight - z) * heightImageFactor);
        }
    }
    SAFE_DELETE_ARRAY(data);

    // Create the heightfield collision shape.
    _shape = bullet_new<btHeightfieldTerrainShape>(_width, _height, _heightfieldData,
        1.0f, minHeight, maxHeight, 1, PHY_FLOAT, false);

    // Offset the heightmap's center of mass according to the way that Bullet calculates the origin 
    // of its heightfield collision shape; see documentation for the btHeightfieldTerrainShape for more info.
    Vector3 s;
    node->getWorldMatrix().getScale(&s);
    Vector3 c (0.0f, -(maxHeight - (0.5f * (maxHeight - minHeight))) / s.y, 0.0f);

    // Create the Bullet rigid body.
    if (c.lengthSquared() > MATH_EPSILON)
        _body = createRigidBodyInternal(_shape, mass, node, friction, restitution, linearDamping, angularDamping, &c);
    else
        _body = createRigidBodyInternal(_shape, mass, node, friction, restitution, linearDamping, angularDamping);

    // Add the rigid body to the physics world.
    Game::getInstance()->getPhysicsController()->addRigidBody(this);

    // Add the rigid body as a listener on the node's transform.
    _node->addListener(this);
}

PhysicsRigidBody::~PhysicsRigidBody()
{
    // Clean up all constraints linked to this rigid body.
    PhysicsConstraint* ptr = NULL;
    while (_constraints.size() > 0)
    {
        ptr = _constraints.back();
        _constraints.pop_back();
        SAFE_DELETE(ptr);
    }

    // Clean up the rigid body and its related objects.
    if (_body)
    {
        if (_body->getMotionState())
            delete _body->getMotionState();

        SAFE_DELETE(_shape);

        Game::getInstance()->getPhysicsController()->removeRigidBody(this);
        SAFE_DELETE(_body);
    }

    SAFE_DELETE(_listeners);
    SAFE_DELETE(_angularVelocity);
    SAFE_DELETE(_anisotropicFriction);
    SAFE_DELETE(_gravity);
    SAFE_DELETE(_linearVelocity);
    SAFE_DELETE_ARRAY(_vertexData);
    for (unsigned int i = 0; i < _indexData.size(); i++)
    {
        SAFE_DELETE_ARRAY(_indexData[i]);
    }
    SAFE_DELETE_ARRAY(_heightfieldData);
    SAFE_DELETE(_inverse);
}

void PhysicsRigidBody::addCollisionListener(Listener* listener, PhysicsRigidBody* body)
{
    if (!_listeners)
        _listeners = new std::vector<Listener*>();

    CollisionPair pair(this, body);
    listener->_collisionStatus[pair] = PhysicsRigidBody::Listener::REGISTERED;

    _listeners->push_back(listener);
}

void PhysicsRigidBody::applyForce(const Vector3& force, const Vector3* relativePosition)
{
    // If the force is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the force.
    if (force.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        if (relativePosition)
            _body->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativePosition->x, relativePosition->y, relativePosition->z));
        else
            _body->applyCentralForce(btVector3(force.x, force.y, force.z));
    }
}

void PhysicsRigidBody::applyImpulse(const Vector3& impulse, const Vector3* relativePosition)
{
    // If the impulse is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the impulse.
    if (impulse.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();

        if (relativePosition)
        {
            _body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relativePosition->x, relativePosition->y, relativePosition->z));
        }
        else
            _body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
    }
}

void PhysicsRigidBody::applyTorque(const Vector3& torque)
{
    // If the torque is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorque(btVector3(torque.x, torque.y, torque.z));
    }
}

void PhysicsRigidBody::applyTorqueImpulse(const Vector3& torque)
{
    // If the torque impulse is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque impulse.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
    }
}

bool PhysicsRigidBody::collidesWith(PhysicsRigidBody* body)
{
    static CollidesWithCallback callback;

    callback.result = false;
    Game::getInstance()->getPhysicsController()->_world->contactPairTest(_body, body->_body, callback);
    return callback.result;
}

PhysicsRigidBody* PhysicsRigidBody::create(Node* node, const char* filePath)
{
    assert(filePath);

    // Load the rigid body properties from file.
    Properties* properties = Properties::create(filePath);
    assert(properties);
    if (properties == NULL)
    {
        WARN_VARG("Failed to load rigid body file: %s", filePath);
        return NULL;
    }

    PhysicsRigidBody* body = create(node, properties->getNextNamespace());
    SAFE_DELETE(properties);

    return body;
}

PhysicsRigidBody* PhysicsRigidBody::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    assert(properties);
    if (!properties || !(strcmp(properties->getNamespace(), "rigidbody") == 0))
    {
        WARN("Failed to load rigid body from properties object: must be non-null object and have namespace equal to \'rigidbody\'.");
        return NULL;
    }

    // Set values to their defaults.
    PhysicsRigidBody::Type type = PhysicsRigidBody::SHAPE_NONE;
    float mass = 0.0;
    float friction = 0.5;
    float restitution = 0.0;
    float linearDamping = 0.0;
    float angularDamping = 0.0;
    bool kinematic = false;
    Vector3* gravity = NULL;
    Vector3* anisotropicFriction = NULL;
    const char* imagePath = NULL;

    // Load the defined properties.
    properties->rewind();
    const char* name;
    while (name = properties->getNextProperty())
    {
        if (strcmp(name, "type") == 0)
        {
            std::string typeStr = properties->getString();
            if (typeStr == "BOX")
                type = SHAPE_BOX;
            else if (typeStr == "SPHERE")
                type = SHAPE_SPHERE;
            else if (typeStr == "MESH")
                type = SHAPE_MESH;
            else if (typeStr == "HEIGHTFIELD")
                type = SHAPE_HEIGHTFIELD;
            else
            {
                WARN_VARG("Could not create rigid body; unsupported value for rigid body type: '%s'.", typeStr.c_str());
                return NULL;
            }
        }
        else if (strcmp(name, "mass") == 0)
        {
            mass = properties->getFloat();
        }
        else if (strcmp(name, "friction") == 0)
        {
            friction = properties->getFloat();
        }
        else if (strcmp(name, "restitution") == 0)
        {
            restitution = properties->getFloat();
        }
        else if (strcmp(name, "linearDamping") == 0)
        {
            linearDamping = properties->getFloat();
        }
        else if (strcmp(name, "angularDamping") == 0)
        {
            angularDamping = properties->getFloat();
        }
        else if (strcmp(name, "kinematic") == 0)
        {
            kinematic = properties->getBool();
        }
        else if (strcmp(name, "gravity") == 0)
        {
            gravity = new Vector3();
            properties->getVector3(NULL, gravity);
        }
        else if (strcmp(name, "anisotropicFriction") == 0)
        {
            anisotropicFriction = new Vector3();
            properties->getVector3(NULL, anisotropicFriction);
        }
        else if (strcmp(name, "image") == 0)
        {
            imagePath = properties->getString();
        }
    }

    // If the rigid body type is equal to mesh, check that the node's mesh's primitive type is supported.
    if (type == SHAPE_MESH)
    {
        Mesh* mesh = node->getModel()->getMesh();

        switch (mesh->getPrimitiveType())
        {
        case Mesh::TRIANGLES:
            break;
        case Mesh::LINES:
        case Mesh::LINE_STRIP:
        case Mesh::POINTS:
        case Mesh::TRIANGLE_STRIP:
            WARN("Mesh rigid bodies are currently only supported on meshes with primitive type equal to TRIANGLES.");

            SAFE_DELETE(gravity);
            SAFE_DELETE(anisotropicFriction);
            return NULL;
        }
    }

    // Create the rigid body.
    PhysicsRigidBody* body = NULL;
    if (imagePath == NULL)
    {
        body = new PhysicsRigidBody(node, type, mass, friction, restitution, linearDamping, angularDamping);
    }
    else
    {
        // Load the image data from the given file path.
        Image* image = Image::create(imagePath);
        if (!image)
            return NULL;

        // Ensure that the image's pixel format is supported.
        switch (image->getFormat())
        {
            case Texture::RGB888:
            case Texture::RGBA8888:
                break;
            default:
                WARN_VARG("Heightmap: pixel format is not supported: %d", image->getFormat());
                return NULL;
        }

        body = new PhysicsRigidBody(node, image, mass, friction, restitution, linearDamping, angularDamping);
        SAFE_RELEASE(image);
    }

    // Set any initially defined properties.
    if (kinematic)
        body->setKinematic(kinematic);
    if (gravity)
        body->setGravity(*gravity);
    if (anisotropicFriction)
        body->setAnisotropicFriction(*anisotropicFriction);

    // Clean up any loaded properties that are on the heap.
    SAFE_DELETE(gravity);
    SAFE_DELETE(anisotropicFriction);

    return body;
}


float PhysicsRigidBody::getHeight(float x, float y) const
{
    // This function is only supported for heightfield rigid bodies.
    if (_shape->getShapeType() != TERRAIN_SHAPE_PROXYTYPE)
    {
        WARN("Attempting to get the height of a non-heightfield rigid body.");
        return 0.0f;
    }

    // Calculate the correct x, y position relative to the heightfield data.
    if (_inverseIsDirty)
    {
        if (_inverse == NULL)
            _inverse = new Matrix();

        _node->getWorldMatrix().invert(_inverse);
        _inverseIsDirty = false;
    }

    Vector3 v = (*_inverse) * Vector3(x, 0.0f, y);
    x = (v.x + (0.5f * (_width - 1))) * _width / (_width - 1);
    y = (v.z + (0.5f * (_height - 1))) * _height / (_height - 1);

    // Check that the x, y position is within the bounds.
    if (x < 0.0f || x > _width || y < 0.0f || y > _height)
    {
        WARN_VARG("Attempting to get height at point '%f, %f', which is outside the range of the heightfield with width %d and height %d.", x, y, _width, _height);
        return 0.0f;
    }

    return calculateHeight(_heightfieldData, _width, _height, x, y);
}

btRigidBody* PhysicsRigidBody::createRigidBodyInternal(btCollisionShape* shape, float mass, Node* node,
                                                       float friction, float restitution, float linearDamping, float angularDamping, 
                                                       const Vector3* centerOfMassOffset)
{
    // If the mass is non-zero, then the object is dynamic so we calculate the local 
    // inertia. However, if the collision shape is a triangle mesh, we don't calculate 
    // inertia since Bullet doesn't currently support this.
    btVector3 localInertia(0.0, 0.0, 0.0);
    if (mass != 0.0 && shape->getShapeType() != TRIANGLE_MESH_SHAPE_PROXYTYPE)
        shape->calculateLocalInertia(mass, localInertia);

    // Create the Bullet physics rigid body object.
    PhysicsMotionState* motionState = new PhysicsMotionState(node, centerOfMassOffset);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
    rbInfo.m_friction = friction;
    rbInfo.m_restitution = restitution;
    rbInfo.m_linearDamping = linearDamping;
    rbInfo.m_angularDamping = angularDamping;
    btRigidBody* body = bullet_new<btRigidBody>(rbInfo);

    return body;
}

void PhysicsRigidBody::addConstraint(PhysicsConstraint* constraint)
{
    _constraints.push_back(constraint);
}

void PhysicsRigidBody::removeConstraint(PhysicsConstraint* constraint)
{
    for (unsigned int i = 0; i < _constraints.size(); i++)
    {
        if (_constraints[i] == constraint)
        {
            _constraints.erase(_constraints.begin() + i);
            break;
        }
    }
}

bool PhysicsRigidBody::supportsConstraints()
{
    return _shape->getShapeType() != TRIANGLE_MESH_SHAPE_PROXYTYPE && _shape->getShapeType() != TERRAIN_SHAPE_PROXYTYPE;
}

void PhysicsRigidBody::transformChanged(Transform* transform, long cookie)
{
    _inverseIsDirty = true;
}

PhysicsRigidBody::CollisionPair::CollisionPair(PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
    : _rbA(rbA), _rbB(rbB)
{
    // Unused
}

PhysicsRigidBody::Listener::~Listener()
{
    // Unused
}

btScalar PhysicsRigidBody::Listener::addSingleResult(btManifoldPoint& cp, 
                                                     const btCollisionObject* a, int partIdA, int indexA, 
                                                     const btCollisionObject* b, int partIdB, int indexB)
{
    // Get pointers to the PhysicsRigidBody objects.
    PhysicsRigidBody* rbA = Game::getInstance()->getPhysicsController()->getRigidBody(a);
    PhysicsRigidBody* rbB = Game::getInstance()->getPhysicsController()->getRigidBody(b);
    
    // If the given rigid body pair has collided in the past, then
    // we notify the listener only if the pair was not colliding
    // during the previous frame. Otherwise, it's a new pair, so notify the listener.
    CollisionPair pair(rbA, rbB);
    if (_collisionStatus.count(pair) > 0)
    {
        if ((_collisionStatus[pair] & COLLISION) == 0)
            collisionEvent(pair, Vector3(cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z()));
    }
    else
    {
        collisionEvent(pair, Vector3(cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z()));
    }

    // Update the collision status cache (we remove the dirty bit
    // set in the controller's update so that this particular collision pair's
    // status is not reset to 'no collision' when the controller's update completes).
    _collisionStatus[pair] &= ~DIRTY;
    _collisionStatus[pair] |= COLLISION;
    return 0.0f;
}

btScalar PhysicsRigidBody::CollidesWithCallback::addSingleResult(btManifoldPoint& cp, 
                                                                 const btCollisionObject* a, int partIdA, int indexA, 
                                                                 const btCollisionObject* b, int partIdB, int indexB)
{
    result = true;
    return 0.0f;
}

float calculateHeight(float* data, unsigned int width, unsigned int height, float x, float y)
{
    unsigned int x1 = x;
    unsigned int y1 = y;
    unsigned int x2 = x1 + 1;
    unsigned int y2 = y1 + 1;
    float tmp;
    float xFactor = modf(x, &tmp);
    float yFactor = modf(y, &tmp);
    float xFactorI = 1.0f - xFactor;
    float yFactorI = 1.0f - yFactor;

    if (x2 >= width && y2 >= height)
    {
        return data[x1 + y1 * width];
    }
    else if (x2 >= width)
    {
        return data[x1 + y1 * width] * yFactorI + data[x1 + y2 * width] * yFactor;
    }
    else if (y2 >= height)
    {
        return data[x1 + y1 * width] * xFactorI + data[x2 + y1 * width] * xFactor;
    }
    else
    {
        return data[x1 + y1 * width] * xFactorI * yFactorI + data[x1 + y2 * width] * xFactorI * yFactor + 
            data[x2 + y2 * width] * xFactor * yFactor + data[x2 + y1 * width] * xFactor * yFactorI;
    }
}

}
