/*
 * MeshSkin.cpp
 */

#include "Base.h"
#include "MeshSkin.h"
#include "Joint.h"

// The number of rows in each palette matrix.
#define PALETTE_ROWS 3

namespace gameplay
{

MeshSkin::MeshSkin() : _matrixPalette(NULL)
{
}

MeshSkin::~MeshSkin()
{
    clearJoints();

    SAFE_DELETE_ARRAY(_matrixPalette);
}

const Matrix& MeshSkin::getBindShape() const
{
    return _bindShape;
}

void MeshSkin::setBindShape(const float* matrix)
{
    _bindShape.set(matrix);
}

Joint* MeshSkin::getJoint(const char* id) const
{
    assert(id);

    for (unsigned int i = 0, count = _joints.size(); i < count; ++i)
    {
        Joint* j = _joints[i];
        if (j && j->getId() != NULL && strcmp(j->getId(), id) == 0)
        {
            return j;
        }
    }

    return NULL;
}

void MeshSkin::setJointCount(unsigned int jointCount)
{
    // Erase the joints vector and release all joints
    clearJoints();

    // Resize the joints vector and initialize to NULL
    _joints.resize(jointCount);
    for (unsigned int i = 0; i < jointCount; i++)
    {
        _joints[i] = NULL;
    }

    // Rebuild the matrix palette. Each matrix is 3 rows of Vector4.
    SAFE_DELETE_ARRAY(_matrixPalette);

    if (jointCount > 0)
    {
        _matrixPalette = new Vector4[jointCount * PALETTE_ROWS];
        for (unsigned int i = 0; i < jointCount * PALETTE_ROWS; i+=PALETTE_ROWS)
        {
            _matrixPalette[i+0].set(1.0f, 0.0f, 0.0f, 0.0f);
            _matrixPalette[i+1].set(0.0f, 1.0f, 0.0f, 0.0f);
            _matrixPalette[i+2].set(0.0f, 0.0f, 1.0f, 0.0f);
        }
    }
}

void MeshSkin::setJoint(Joint* joint, unsigned int index)
{
    assert(index < _joints.size());

    _joints[index] = joint;
}

Vector4* MeshSkin::getMatrixPalette() const
{
    unsigned int count = _joints.size();
    for (unsigned int i = 0; i < count; i++)
    {
        _joints[i]->updateJointMatrix(getBindShape(), &_matrixPalette[i * PALETTE_ROWS]);
    }
    return _matrixPalette;
}

unsigned int MeshSkin::getMatrixPaletteSize() const
{
    return _joints.size() * PALETTE_ROWS;
}

Joint* MeshSkin::getJoint(unsigned int index) const
{
    assert(index < _joints.size());
    return _joints[index];
}

void MeshSkin::clearJoints()
{
    _joints.clear();
}

}
