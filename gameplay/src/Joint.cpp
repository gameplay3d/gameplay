#include "Base.h"
#include "Joint.h"

namespace gameplay
{

Joint::Joint(const char* id)
    : Node(id), _jointMatrixDirty(true), _skin(NULL)
{
}

Joint::~Joint()
{
}

Joint* Joint::create(const char* id)
{
    return new Joint(id);
}

Node::Type Joint::getType() const
{
    return Node::JOINT;
}

void Joint::transformChanged()
{
    Node::transformChanged();
    _jointMatrixDirty = true;
}

void Joint::updateJointMatrix(const Matrix& bindShape, Vector4* matrixPalette)
{
    if (_jointMatrixDirty)
    {
        _jointMatrixDirty = false;

        Matrix t;
        Matrix::multiply(getJointMatrix(), getInverseBindPose(), &t);
        Matrix::multiply(t, bindShape, &t);

        matrixPalette[0].set(t.m[0], t.m[4], t.m[8], t.m[12]);
        matrixPalette[1].set(t.m[1], t.m[5], t.m[9], t.m[13]);
        matrixPalette[2].set(t.m[2], t.m[6], t.m[10], t.m[14]);
    }
}

const Matrix& Joint::getInverseBindPose() const
{
    return _bindPose;
}

void Joint::setInverseBindPose(const Matrix& m)
{
    _bindPose = m;
    _jointMatrixDirty = true;
}

const Matrix& Joint::getWorldMatrix() const
{
    // If this is the root joint, then we 
    // also apply the transform of the model
    // that the skin is attached to to get the
    // actual world matrix.
    if (_parent == NULL && _skin != NULL)
    {
        Matrix::multiply(_skin->_model->getNode()->getWorldMatrix(), Node::getWorldMatrix(), &_jointWorld);
    }
    else
    {
        memcpy((void*)_jointWorld.m, Node::getWorldMatrix().m, sizeof(float) * 16);
    }

    return _jointWorld;
}

const Matrix& Joint::getJointMatrix() const
{
    return Node::getWorldMatrix();
}

}
