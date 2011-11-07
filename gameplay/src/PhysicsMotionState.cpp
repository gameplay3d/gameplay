/**
 * PhysicsMotionState.cpp
 */

#include "PhysicsMotionState.h"

namespace gameplay
{

PhysicsMotionState::PhysicsMotionState(Node* node, const Vector3* centerOfMassOffset) : _node(node),
    _needsUpdate(false), _centerOfMassOffset(btTransform::getIdentity())
{
    // Store the initial world transform (minus the scale) for use by Bullet later on.
    Quaternion rotation;
    const Matrix& m = _node->getWorldMatrix();
    m.getRotation(&rotation);

    if (centerOfMassOffset)
    {
        // Store the center of mass offset.
        _centerOfMassOffset.setOrigin(btVector3(centerOfMassOffset->x, centerOfMassOffset->y, centerOfMassOffset->z));

        // When there is a center of mass offset, we modify the initial world transformation
        // so that when physics is initially applied, the object is in the correct location.
        btQuaternion orientation(rotation.x, rotation.y, rotation.z, rotation.w);
        btTransform offset = btTransform(orientation, btVector3(0.0f, 0.0f, 0.0f)) * _centerOfMassOffset.inverse();

        btVector3 origin(m.m[12] + _centerOfMassOffset.getOrigin().getX() + offset.getOrigin().getX(),
            m.m[13] + _centerOfMassOffset.getOrigin().getY() + offset.getOrigin().getY(), 
            m.m[14] + _centerOfMassOffset.getOrigin().getZ() + offset.getOrigin().getZ());
        _worldTransform = btTransform(orientation, origin);
    }
    else
    {
		_worldTransform = btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), 
			btVector3(m.m[12], m.m[13], m.m[14]));
    }
}

PhysicsMotionState::~PhysicsMotionState()
{
}

void PhysicsMotionState::getWorldTransform(btTransform &transform) const
{
	transform = _centerOfMassOffset.inverse() * _worldTransform;
}

void PhysicsMotionState::setWorldTransform(const btTransform &transform)
{
    _worldTransform = transform * _centerOfMassOffset;
        
    const btQuaternion& rot = _worldTransform.getRotation();
	const btVector3& pos = _worldTransform.getOrigin();

	_node->setRotation(rot.x(), rot.y(), rot.z(), rot.w());
	_node->setTranslation(pos.x(), pos.y(), pos.z());
}

}
