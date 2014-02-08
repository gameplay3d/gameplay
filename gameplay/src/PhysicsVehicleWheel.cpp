#include "Base.h"
#include "Node.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"

namespace gameplay
{

PhysicsVehicleWheel::PhysicsVehicleWheel(Node* node, const PhysicsCollisionShape::Definition& shape, const PhysicsRigidBody::Parameters& parameters)
    : PhysicsCollisionObject(node), _host(NULL), _indexInHost(0)
{
    findAncestorAndBind();
}

PhysicsVehicleWheel::PhysicsVehicleWheel(Node* node)
    : PhysicsCollisionObject(node), _host(NULL), _indexInHost(0)
{
    findAncestorAndBind();
}

PhysicsVehicleWheel* PhysicsVehicleWheel::create(Node* node, Properties* properties)
{
    PhysicsVehicleWheel* wheel = new PhysicsVehicleWheel(node);

    // Load the defined wheel parameters.
    properties->rewind();
    Vector3 v;
    const char* name;
    while ((name = properties->getNextProperty()) != NULL)
    {
        if (strcmp(name, "steerable") == 0)
        {
            wheel->setSteerable(properties->getBool(name));
        }
        else if (strcmp(name, "wheelDirection") == 0 && properties->getVector3(name, &v))
        {
            wheel->setWheelDirection(v);
        }
        else if (strcmp(name, "wheelAxle") == 0 && properties->getVector3(name, &v))
        {
            wheel->setWheelAxle(v);
        }
        else if (strcmp(name, "strutConnectionOffset") == 0 && properties->getVector3(name, &v))
        {
            wheel->setStrutConnectionOffset(v);
        }
        else if (strcmp(name, "strutRestLength") == 0)
        {
            wheel->setStrutRestLength(properties->getFloat(name));
        }
        else if (strcmp(name, "strutTravelMax") == 0)
        {
            wheel->setStrutTravelMax(properties->getFloat(name));
        }
        else if (strcmp(name, "strutStiffness") == 0)
        {
            wheel->setStrutStiffness(properties->getFloat(name));
        }
        else if (strcmp(name, "strutDampingCompression") == 0)
        {
            wheel->setStrutDampingCompression(properties->getFloat(name));
        }
        else if (strcmp(name, "strutDampingRelaxation") == 0)
        {
            wheel->setStrutDampingRelaxation(properties->getFloat(name));
        }
        else if (strcmp(name, "strutForceMax") == 0)
        {
            wheel->setStrutForceMax(properties->getFloat(name));
        }
        else if (strcmp(name, "frictionBreakout") == 0)
        {
            wheel->setFrictionBreakout(properties->getFloat(name));
        }
        else if (strcmp(name, "wheelRadius") == 0)
        {
            wheel->setWheelRadius(properties->getFloat(name));
        }
        else if (strcmp(name, "rollInfluence") == 0)
        {
            wheel->setRollInfluence(properties->getFloat(name));
        }
        else
        {
            // Ignore this case (we've already parsed the rigid body parameters).
        }
    }

    return wheel;
}

PhysicsVehicleWheel::~PhysicsVehicleWheel()
{
}

btCollisionObject* PhysicsVehicleWheel::getCollisionObject() const
{
    GP_ASSERT(_host);

    return _host->getCollisionObject();
}

PhysicsCollisionObject::Type PhysicsVehicleWheel::getType() const
{
    return PhysicsCollisionObject::VEHICLE_WHEEL;
}

void PhysicsVehicleWheel::setEnabled(bool enable)
{
    GP_ERROR("Operation not supported (PhysicsVehicleWheel::setEnabled(bool)). Use host vehicle instead.");
}

void PhysicsVehicleWheel::findAncestorAndBind()
{
    GP_ASSERT(getNode());

    // Search for the first PhysicsVehicle that shares a common ancestor, and
    // bind with it. The following code performs a naive search; nothing more
    // sophisticated is deemed necessary because:
    // (a) The root of the scene is NOT a node
    // (b) Scene graphs tend to be relatively flat.
    //
    // The search ends when a vehicle is found or n is null:
    // 1: Let n = this node
    // 2: Visit each sibling of n and perform a breadth-first search of its descendants
    // 3: Let n = the parent of n
    // 4: Go to 2.
    PhysicsVehicle* host = NULL;
    Node* m;
    for (Node* n = getNode(); n && !host; n = n->getParent())
    {
        // Visit siblings before n
        for (m = n->getPreviousSibling(); m && !host; m = m->getPreviousSibling())
        {
            host = findVehicle(m);
        }

        // Visit siblings after n
        for (m = n->getNextSibling(); m && !host; m = m->getNextSibling())
        {
            host = findVehicle(m);
        }
    }

    // Note: Currently this method is silent on failure to find a host.
    if (host)
    {
        host->addWheel(this);
        _initialOffset = _node->getTranslation() - host->_node->getTranslation();
    }
}

PhysicsVehicle* PhysicsVehicleWheel::findVehicle(Node* node)
{
    PhysicsCollisionObject* collisionObject = node->getCollisionObject();
    if (collisionObject && collisionObject->getType() == PhysicsCollisionObject::VEHICLE)
    {
        return static_cast<PhysicsVehicle*>(collisionObject);
    }

    PhysicsVehicle* result = NULL;
    for (Node* p = node->getFirstChild(); p && !result; p = p->getNextSibling())
    {
        result = findVehicle(p);
    }
    return result;
}

void PhysicsVehicleWheel::setHost(PhysicsVehicle* host, unsigned int indexInHost)
{
    _host = host;
    _indexInHost = indexInHost;
}

void PhysicsVehicleWheel::addToVehicle(btRaycastVehicle* vehicle)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->getNumWheels() == vehicle->getNumWheels() + 1);

    // Use safe defaults for now. Properties are assigned elsewhere.
    btRaycastVehicle::btVehicleTuning tuning;
    vehicle->addWheel(
        btVector3(0, 0, 0),
        btVector3(0, -1, 0),
        btVector3(-1, 0, 0),
        0.6f,
        0.5f,
        tuning,
        false);
}

void PhysicsVehicleWheel::transform(Node* node) const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_node);

    node->setRotation(_orientation);

    // Use only the component parallel to the defined strut line
    Vector3 strutLine;
    getWheelDirection(&strutLine);
    _host->_node->getMatrix().transformVector(&strutLine);
    Vector3 wheelPos;
    getWheelPos(&wheelPos);
    node->setTranslation(wheelPos + strutLine*(strutLine.dot(_positionDelta) / strutLine.lengthSquared()));
}

void PhysicsVehicleWheel::update(float elapsedTime)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    const btTransform& trans = _host->_vehicle->getWheelInfo(_indexInHost).m_worldTransform;
    const btQuaternion& rot = trans.getRotation();
    const btVector3& pos = trans.getOrigin();
    _orientation.set(rot.x(), rot.y(), rot.z(), rot.w());

    Vector3 commandedPosition(pos.x(), pos.y(), pos.z());
    Vector3 wheelPos;
    getWheelPos(&wheelPos);
    commandedPosition -= wheelPos;

    // Filter out noise from Bullet
    Vector3 delta(_positionDelta, commandedPosition);
    float threshold = getStrutRestLength() * 2.0f;
    float responseTime = (delta.lengthSquared() > threshold*threshold) ? 0 : 60;
    _positionDelta.smooth(commandedPosition, elapsedTime, responseTime);
}

void PhysicsVehicleWheel::getConnectionDefault(Vector3* result) const
{
    // projected strut length
    getWheelDirection(result);
    result->normalize();
    float length = 0.58f * getStrutRestLength();
    *result *= -length;

    // nudge wheel contact point to outer edge of tire for stability
    Vector3 nudge;
    getWheelAxle(&nudge);
    nudge *= nudge.dot(_initialOffset);
    nudge.normalize();
    *result += nudge * 0.068f * getWheelRadius(); // rough-in for tire width

    // offset at bind time
    *result += _initialOffset;
}

void PhysicsVehicleWheel::getWheelPos(Vector3* result) const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_node);

    *result = _initialOffset;
    _host->_node->getMatrix().transformPoint(result);
}

bool PhysicsVehicleWheel::isSteerable() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_bIsFrontWheel;
}

void PhysicsVehicleWheel::setSteerable(bool steerable)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_bIsFrontWheel = steerable;
}

void PhysicsVehicleWheel::getWheelDirection(Vector3* wheelDirection) const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    const btVector3& v = _host->_vehicle->getWheelInfo(_indexInHost).m_wheelDirectionCS;
    wheelDirection->set(v.x(), v.y(), v.z());
}

void PhysicsVehicleWheel::setWheelDirection(const Vector3& wheelDirection)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_wheelDirectionCS.setValue(wheelDirection.x, wheelDirection.y, wheelDirection.z);
}

void PhysicsVehicleWheel::getWheelAxle(Vector3* wheelAxle) const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    const btVector3& v = _host->_vehicle->getWheelInfo(_indexInHost).m_wheelAxleCS;
    wheelAxle->set(v.x(), v.y(), v.z());
}

void PhysicsVehicleWheel::setWheelAxle(const Vector3& wheelAxle)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_wheelAxleCS.setValue( wheelAxle.x, wheelAxle.y, wheelAxle.z);
}

void PhysicsVehicleWheel::getStrutConnectionOffset(Vector3* strutConnectionOffset) const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    const btVector3& v = _host->_vehicle->getWheelInfo(_indexInHost).m_chassisConnectionPointCS;
    strutConnectionOffset->set(v.x(), v.y(), v.z());
    Vector3 strutConnectionDefault;
    getConnectionDefault(&strutConnectionDefault);
    *strutConnectionOffset -= strutConnectionDefault;
}

void PhysicsVehicleWheel::setStrutConnectionOffset(const Vector3& strutConnectionOffset)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    Vector3 strutConnectionPoint;
    getConnectionDefault(&strutConnectionPoint);
    strutConnectionPoint += strutConnectionOffset;
    _host->_vehicle->getWheelInfo(_indexInHost).m_chassisConnectionPointCS.setValue(strutConnectionPoint.x,
                                                                                    strutConnectionPoint.y,
                                                                                    strutConnectionPoint.z);
}

float PhysicsVehicleWheel::getStrutRestLength() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_suspensionRestLength1;
}

void PhysicsVehicleWheel::setStrutRestLength(float strutRestLength)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_suspensionRestLength1 = strutRestLength;
}

float PhysicsVehicleWheel::getStrutTravelMax() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_maxSuspensionTravelCm / 100.0f;
}

void PhysicsVehicleWheel::setStrutTravelMax(float strutTravelMax)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_maxSuspensionTravelCm = strutTravelMax * 100.0f;
}

float PhysicsVehicleWheel::getStrutStiffness() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_suspensionStiffness;
}

void PhysicsVehicleWheel::setStrutStiffness(float strutStiffness)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_suspensionStiffness = strutStiffness;
}

float PhysicsVehicleWheel::getStrutDampingCompression() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsDampingCompression;
}

void PhysicsVehicleWheel::setStrutDampingCompression(float strutDampingCompression)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsDampingCompression = strutDampingCompression;
}

float PhysicsVehicleWheel::getStrutDampingRelaxation() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsDampingRelaxation;
}

void PhysicsVehicleWheel::setStrutDampingRelaxation(float strutDampingRelaxation)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsDampingRelaxation = strutDampingRelaxation;
}

float PhysicsVehicleWheel::getStrutForceMax() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_maxSuspensionForce;
}

void PhysicsVehicleWheel::setStrutForceMax(float strutForceMax)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_maxSuspensionForce = strutForceMax;
}

float PhysicsVehicleWheel::getFrictionBreakout() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_frictionSlip;
}

void PhysicsVehicleWheel::setFrictionBreakout(float frictionBreakout)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_frictionSlip = frictionBreakout;
}

float PhysicsVehicleWheel::getWheelRadius() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsRadius;
}

void PhysicsVehicleWheel::setWheelRadius(float wheelRadius)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_wheelsRadius = wheelRadius;
}

float PhysicsVehicleWheel::getRollInfluence() const
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    return _host->_vehicle->getWheelInfo(_indexInHost).m_rollInfluence;
}

void PhysicsVehicleWheel::setRollInfluence(float rollInfluence)
{
    GP_ASSERT(_host);
    GP_ASSERT(_host->_vehicle);

    _host->_vehicle->getWheelInfo(_indexInHost).m_rollInfluence = rollInfluence;
}

}
