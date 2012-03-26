#include "Base.h"
#include "Joint.h"
#include "MeshSkin.h"

namespace gameplay
{

Joint::Joint(const char* id)
    : Node(id), _jointMatrixDirty(true), _skinCount(0)
{
}

Joint::~Joint()
{
}

Joint* Joint::create(const char* id)
{
    return new Joint(id);
}

Node* Joint::cloneSingleNode(CloneContext &context) const
{
    Joint* copy = Joint::create(getId());
    context.registerClonedNode(this, copy);
    copy->_bindPose = _bindPose;
    copy->_skinCount = _skinCount;
    Node::cloneInto(copy, context);
    return copy;
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
    // Note: If more than one MeshSkin influences this Joint, we need to skip
    // the _jointMatrixDirty optimization since updateJointMatrix() may be
    // called multiple times a frame with different bindShape matrices (and
    // different matrixPallete pointers).
    if (_skinCount > 1 || _jointMatrixDirty)
    {
        _jointMatrixDirty = false;

        static Matrix t;
        Matrix::multiply(Node::getWorldMatrix(), getInverseBindPose(), &t);
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
