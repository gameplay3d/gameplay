/*
 * Joint.cpp
 */

#include "Base.h"
#include "Joint.h"

namespace gameplay
{

Joint::Joint(const char* id)
    : Node(id), _jointMatrixDirty(true)
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

    const char* id = _id.c_str();
    _jointMatrixDirty = true;
}

void Joint::updateJointMatrix(const Matrix& bindShape, Vector4* matrixPalette)
{
    const char* id = _id.c_str();

    if (_jointMatrixDirty)
    {
        _jointMatrixDirty = false;

        float r = (float)rand( ) / (float)RAND_MAX;
        Matrix w = getWorldMatrix();
        w.translate(r, r, r);

        Matrix t;
        Matrix::multiply(getWorldMatrix(), getInverseBindPose(), &t);
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

}
