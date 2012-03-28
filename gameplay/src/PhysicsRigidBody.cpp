#include "Base.h"
#include "PhysicsRigidBody.h"
#include "PhysicsMotionState.h"
#include "PhysicsController.h"
#include "Game.h"
#include "Image.h"
#include "MeshPart.h"
#include "Node.h"

namespace gameplay
{

PhysicsRigidBody::PhysicsRigidBody(Node* node, const PhysicsCollisionShape::Definition& shape, const Parameters& parameters)
        : PhysicsCollisionObject(node), _body(NULL), _mass(parameters.mass), _constraints(NULL)
{
	// Create our collision sh ape
	Vector3 centerOfMassOffset;
	_collisionShape = Game::getInstance()->getPhysicsController()->createShape(node, shape, &centerOfMassOffset);

	// Create motion state object
	_motionState = new PhysicsMotionState(node, (centerOfMassOffset.lengthSquared() > MATH_EPSILON) ? &centerOfMassOffset : NULL);

    // If the mass is non-zero, then the object is dynamic so we calculate the local 
    // inertia. However, if the collision shape is a triangle mesh, we don't calculate 
    // inertia since Bullet doesn't currently support this.
    btVector3 localInertia(0.0, 0.0, 0.0);
	if (parameters.mass != 0.0 && _collisionShape->getType() != PhysicsCollisionShape::SHAPE_MESH)
		_collisionShape->getShape<btCollisionShape>()->calculateLocalInertia(parameters.mass, localInertia);

    // Create the Bullet physics rigid body object.
    btRigidBody::btRigidBodyConstructionInfo rbInfo(parameters.mass, _motionState, _collisionShape->getShape<btCollisionShape>(), localInertia);
    rbInfo.m_friction = parameters.friction;
    rbInfo.m_restitution = parameters.restitution;
    rbInfo.m_linearDamping = parameters.linearDamping;
    rbInfo.m_angularDamping = parameters.angularDamping;

	// Create + assign the new bullet rigid body object.
	_body = bullet_new<btRigidBody>(rbInfo);

	// Set other initially defined properties.
    setKinematic(parameters.kinematic);
	setAnisotropicFriction(parameters.anisotropicFriction);
	setGravity(parameters.gravity);

    // Add ourself to the physics world.
    Game::getInstance()->getPhysicsController()->addCollisionObject(this);

	if (_collisionShape->getType() == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
	{
		// Add a listener on the node's transform so we can track dirty changes to calculate
		// an inverse matrix for transforming heightfield points between world and local space.
		_node->addListener(this);
	}
}

PhysicsRigidBody::~PhysicsRigidBody()
{
	// Clean up all constraints linked to this rigid body.
	if (_constraints)
	{
		for (unsigned int i = 0; i < _constraints->size(); ++i)
		{
			SAFE_DELETE((*_constraints)[i]);
		}
		SAFE_DELETE(_constraints);
	}

	// Remove collision object from physics controller
    Game::getInstance()->getPhysicsController()->removeCollisionObject(this);

    // Clean up the rigid body and its related objects.
	SAFE_DELETE(_body);

	// Unregister node listener (only registered for heihgtfield collision shape types)
	if (_collisionShape->getType() == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
	{
		_node->removeListener(this);
	}
}

PhysicsCollisionObject::Type PhysicsRigidBody::getType() const
{
    return PhysicsCollisionObject::RIGID_BODY;
}

btCollisionObject* PhysicsRigidBody::getCollisionObject() const
{
    return _body;
}

void PhysicsRigidBody::applyForce(const Vector3& force, const Vector3* relativePosition)
{
    // If the force is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the force.
    if (force.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        if (relativePosition)
            _body->applyForce(BV(force), BV(*relativePosition));
        else
            _body->applyCentralForce(BV(force));
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
            _body->applyImpulse(BV(impulse), BV(*relativePosition));
        }
        else
            _body->applyCentralImpulse(BV(impulse));
    }
}

void PhysicsRigidBody::applyTorque(const Vector3& torque)
{
    // If the torque is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorque(BV(torque));
    }
}

void PhysicsRigidBody::applyTorqueImpulse(const Vector3& torque)
{
    // If the torque impulse is significant enough, activate the rigid body 
    // to make sure that it isn't sleeping and apply the torque impulse.
    if (torque.lengthSquared() > MATH_EPSILON)
    {
        _body->activate();
        _body->applyTorqueImpulse(BV(torque));
    }
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
	bool typeSpecified = false;
	PhysicsCollisionShape::Type type;
	Parameters parameters;
    const char* imagePath = NULL;
    float radius, height;
	Vector3 center, min, max;
	int bits = 0;

    // Load the defined properties.
    properties->rewind();
    const char* name;
    while (name = properties->getNextProperty())
    {
        if (strcmp(name, "type") == 0)
        {
            std::string typeStr = properties->getString();
            if (typeStr == "BOX")
                type = PhysicsCollisionShape::SHAPE_BOX;
            else if (typeStr == "SPHERE")
                type = PhysicsCollisionShape::SHAPE_SPHERE;
			else if (typeStr == "CAPSULE")
                type = PhysicsCollisionShape::SHAPE_CAPSULE;
			else if (typeStr == "HEIGHTFIELD")
                type = PhysicsCollisionShape::SHAPE_HEIGHTFIELD;
            else if (typeStr == "MESH")
                type = PhysicsCollisionShape::SHAPE_MESH;
            else
            {
                WARN_VARG("Could not create rigid body; unsupported value for rigid body type: '%s'.", typeStr.c_str());
                return NULL;
            }
			typeSpecified = true;
        }
        else if (strcmp(name, "mass") == 0)
        {
            parameters.mass = properties->getFloat();
        }
        else if (strcmp(name, "friction") == 0)
        {
            parameters.friction = properties->getFloat();
        }
        else if (strcmp(name, "restitution") == 0)
        {
            parameters.restitution = properties->getFloat();
        }
        else if (strcmp(name, "linearDamping") == 0)
        {
            parameters.linearDamping = properties->getFloat();
        }
        else if (strcmp(name, "angularDamping") == 0)
        {
            parameters.angularDamping = properties->getFloat();
        }
        else if (strcmp(name, "kinematic") == 0)
        {
            parameters.kinematic = properties->getBool();
        }
        else if (strcmp(name, "anisotropicFriction") == 0)
        {
            properties->getVector3(NULL, &parameters.anisotropicFriction);
        }
        else if (strcmp(name, "gravity") == 0)
        {
            properties->getVector3(NULL, &parameters.gravity);
        }
        else if (strcmp(name, "image") == 0)
        {
            imagePath = properties->getString();
        }
        else if (strcmp(name, "radius") == 0)
        {
			radius = properties->getFloat();
			bits |= 1;
        }
        else if (strcmp(name, "height") == 0)
        {
            height = properties->getFloat();
			bits |= 2;
        }
		else if (strcmp(name, "center") == 0)
		{
			properties->getVector3(NULL, &center);
			bits |= 4;
		}
		else if (strcmp(name, "min") == 0)
		{
			properties->getVector3(NULL, &min);
			bits |= 8;
		}
		else if (strcmp(name, "max") == 0)
		{
			properties->getVector3(NULL, &max);
			bits |= 16;
		}
    }

	if (!typeSpecified)
	{
		WARN("Missing 'type' specifier for rigid body definition.");
		return NULL;
	}

	PhysicsRigidBody* body = NULL;

	switch (type)
	{
	case PhysicsCollisionShape::SHAPE_BOX:
		if ((bits & 8/*min*/) || (bits & 16/*max*/))
		{
			// Explicitly defined box shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::box(min, max), parameters);
		}
		else
		{
			// Auto box shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::box(), parameters);
		}
		break;

	case PhysicsCollisionShape::SHAPE_SPHERE:
		if ((bits & 4/*center*/) || (bits & 1/*radius*/))
		{
			// Explicitly defined sphere shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::sphere(radius, center), parameters);
		}
		else
		{
			// Auto sphere shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::sphere(), parameters);
		}
		break;

    case PhysicsCollisionShape::SHAPE_CAPSULE:
		if ((bits & 1/*radius*/) || (bits & 2/*height*/))
		{
			// Explicitly defined capsule shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::capsule(radius, height, center), parameters);
		}
		else
		{
			// Auto capsule shape
			body = new PhysicsRigidBody(node, PhysicsCollisionShape::capsule(), parameters);
		}
        break;

    case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
		{
			if (imagePath == NULL)
			{
				WARN("Heightfield rigid body requires an image path.");
				return NULL;
			}

            // Load the image data from the given file path.
            Image* image = Image::create(imagePath);
            if (!image)
			{
				WARN_VARG("Failed to load heightmap image: %s", imagePath);
                return NULL;
			}

            // Ensure that the image's pixel format is supported.
            switch (image->getFormat())
            {
                case Image::RGB:
                case Image::RGBA:
                    break;
                default:
                    WARN_VARG("Heightmap: pixel format is not supported: %d", image->getFormat());
                    return NULL;
            }

			body = new PhysicsRigidBody(node, PhysicsCollisionShape::heightfield(image), parameters);

			SAFE_RELEASE(image);
		}
        break;

	case PhysicsCollisionShape::SHAPE_MESH:
		{
			// Mesh is required on node
			Mesh* mesh = node->getModel() ? node->getModel()->getMesh() : NULL;
			if (mesh == NULL)
			{
				WARN("Cannot create mesh rigid body for node without mode/mesh.");
				return NULL;
			}

			body = new PhysicsRigidBody(node, PhysicsCollisionShape::mesh(mesh), parameters);
		}
		break;

	default:
		break;
	}

    return body;
}

void PhysicsRigidBody::setKinematic(bool kinematic)
{
    if (kinematic)
    {
        _body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        _body->setActivationState(DISABLE_DEACTIVATION);
    }
    else
    {
        _body->setCollisionFlags(_body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        _body->setActivationState(ACTIVE_TAG);
    }
}

float PhysicsRigidBody::getHeight(float x, float y) const
{
    // This function is only supported for heightfield rigid bodies.
	if (_collisionShape->getType() != PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
    {
        WARN("Attempting to get the height of a non-heightfield rigid body.");
        return 0.0f;
    }

    // Calculate the correct x, y position relative to the heightfield data.
	if (_collisionShape->_shapeData.heightfieldData->inverseIsDirty)
    {
		_node->getWorldMatrix().invert(&_collisionShape->_shapeData.heightfieldData->inverse);
		_collisionShape->_shapeData.heightfieldData->inverseIsDirty = false;
    }

	float w = _collisionShape->_shapeData.heightfieldData->width;
	float h = _collisionShape->_shapeData.heightfieldData->height;

    Vector3 v = _collisionShape->_shapeData.heightfieldData->inverse * Vector3(x, 0.0f, y);
    x = (v.x + (0.5f * (w - 1))) * w / (w - 1);
    y = (v.z + (0.5f * (h - 1))) * h / (h - 1);

    // Check that the x, y position is within the bounds.
    if (x < 0.0f || x > w || y < 0.0f || y > h)
    {
        WARN_VARG("Attempting to get height at point '%f, %f', which is outside the range of the heightfield with width %d and height %d.", x, y, w, h);
        return 0.0f;
    }

	return PhysicsController::calculateHeight(_collisionShape->_shapeData.heightfieldData->heightData, w, h, x, y);
}

void PhysicsRigidBody::addConstraint(PhysicsConstraint* constraint)
{
	if (_constraints == NULL)
		_constraints = new std::vector<PhysicsConstraint*>();

    _constraints->push_back(constraint);
}

void PhysicsRigidBody::removeConstraint(PhysicsConstraint* constraint)
{
	if (_constraints)
	{
		for (unsigned int i = 0; i < _constraints->size(); ++i)
		{
			if ((*_constraints)[i] == constraint)
			{
				_constraints->erase(_constraints->begin() + i);
				break;
			}
		}
	}
}

bool PhysicsRigidBody::supportsConstraints()
{
	return (getShapeType() != PhysicsCollisionShape::SHAPE_HEIGHTFIELD && getShapeType() != PhysicsCollisionShape::SHAPE_MESH);
}

void PhysicsRigidBody::transformChanged(Transform* transform, long cookie)
{
	if (getShapeType() == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
	{
		_collisionShape->_shapeData.heightfieldData->inverseIsDirty = true;
	}
}

}
