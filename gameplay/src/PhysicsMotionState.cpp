#include "Base.h"
#include "PhysicsMotionState.h"
#include "Node.h"

namespace gameplay
{

PhysicsMotionState::PhysicsMotionState(Node* node, const Vector3* centerOfMassOffset) : _node(node),
    _centerOfMassOffset(btTransform::getIdentity())
{
    if (centerOfMassOffset)
    {
        // Store the center of mass offset.
        _centerOfMassOffset.setOrigin(BV(*centerOfMassOffset));
    }

    updateTransformFromNode();
}

PhysicsMotionState::~PhysicsMotionState()
{
}

void PhysicsMotionState::getWorldTransform(btTransform &transform) const
{
    GP_ASSERT(_node);
    if (_node->getCollisionObject() && _node->getCollisionObject()->isKinematic())
        updateTransformFromNode();

    transform = _centerOfMassOffset.inverse() * _worldTransform;
}

void PhysicsMotionState::setWorldTransform(const btTransform &transform)
{
    GP_ASSERT(_node);

    _worldTransform = transform * _centerOfMassOffset;
        
    const btQuaternion& rot = _worldTransform.getRotation();
    const btVector3& pos = _worldTransform.getOrigin();

    _node->setRotation(rot.x(), rot.y(), rot.z(), rot.w());
    _node->setTranslation(pos.x(), pos.y(), pos.z());
}

void PhysicsMotionState::updateTransformFromNode() const
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

}
