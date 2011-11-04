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

    PhysicsMotionState(Node* node, const Vector3* centerOfMassOffset = NULL) : _node(node),
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

public:

	virtual ~PhysicsMotionState()
	{
    }

    virtual void getWorldTransform(btTransform &transform) const
	{
		transform = _centerOfMassOffset.inverse() * _worldTransform;
    }

    virtual void setWorldTransform(const btTransform &transform)
	{
        _worldTransform = transform * _centerOfMassOffset;
        
        const btQuaternion& rot = _worldTransform.getRotation();
		const btVector3& pos = _worldTransform.getOrigin();

		_node->setRotation(rot.x(), rot.y(), rot.z(), rot.w());
		_node->setTranslation(pos.x(), pos.y(), pos.z());
    }

    /*
    void update()
    {
        static Quaternion r;
        static Vector3 t;

        if (_needsUpdate)
        {
            if (_node->getParent()->getPhysicsRigidBody())
                _node->getParent()->getPhysicsRigidBody()->update();

            // Use the inverse of the parent's world matrix to
            // calculate the local transform change due to physics.
            const Matrix& m = _node->getParent()->getWorldMatrix();
            m.getRotation(&r);
            m.getTranslation(&t);
            btTransform parentWorldInverse = (btTransform(btQuaternion(r.x, r.y, r.z, r.w), btVector3(t.x, t.y, t.z))).inverse();
            _worldTransform = parentWorldInverse * _worldTransform;

            _needsUpdate = false;
        }
    }

    const btTransform& getCenterOfMassOffset()
    {
        return _centerOfMassOffset;
    }
    */

private:
	Node* _node;
    btTransform _centerOfMassOffset;
	btTransform _worldTransform;
    bool _needsUpdate;
};

}

#endif
