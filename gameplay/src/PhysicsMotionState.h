/*
 * PhysicsMotionState.h
 */

#ifndef PHYSICSMOTIONSTATE_H_
#define PHYSICSMOTIONSTATE_H_

#include "Node.h"
#include "PhysicsRigidBody.h"
#include "Transform.h"

namespace gameplay
{

class PhysicsMotionState : public btMotionState
{
	friend class PhysicsRigidBody;

	PhysicsMotionState(Node* node) : _node(node)
	{
        // Store the initial world transform (minus the scale) for use by Bullet later on.
        Quaternion rotation;
        const Matrix& m = _node->getWorldMatrix();
        m.getRotation(&rotation);
		_worldTransform = btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), 
			btVector3(m.m[12], m.m[13], m.m[14]));
	}

public:

	virtual ~PhysicsMotionState()
	{
    }

    virtual void getWorldTransform(btTransform &transform) const
	{
		transform = _worldTransform;
    }

    virtual void setWorldTransform(const btTransform &transform)
	{
		// Calculate the actual world transform by 
		// taking into account the center of mass offset.
		_worldTransform = transform;

        const btQuaternion& rot = _worldTransform.getRotation();
		const btVector3& pos = _worldTransform.getOrigin();

		_node->setRotation(rot.x(), rot.y(), rot.z(), rot.w());
		_node->setTranslation(pos.x(), pos.y(), pos.z());
    }

private:
	Node* _node;
	btTransform _worldTransform;
};

}

#endif
