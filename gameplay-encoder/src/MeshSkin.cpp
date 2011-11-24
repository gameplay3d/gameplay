#include "MeshSkin.h"
#include "Node.h"
#include "StringUtil.h"
#include "Matrix.h"

namespace gameplay
{

MeshSkin::MeshSkin(void) :
    _vertexInfluenceCount(0)
{
    Matrix::setIdentity(_bindShape);
}

MeshSkin::~MeshSkin(void)
{
}

unsigned int MeshSkin::getTypeId(void) const
{
    return MESHPART_ID;
}

const char* MeshSkin::getElementName(void) const
{
    return "MeshSkin";
}

void MeshSkin::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_bindShape, 16, file);
    write(_joints.size(), file);
    for (std::list<Node*>::const_iterator i = _joints.begin(); i != _joints.end(); ++i)
    {
        (*i)->writeBinaryXref(file);
    }
    write(_bindPoses, file);
}

void MeshSkin::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintf(file, "<bindShape>");
    fprintfMatrix4f(file, _bindShape);
    fprintf(file, "</bindShape>");
    fprintf(file, "<joints>");
    for (std::list<std::string>::const_iterator i = _jointNames.begin(); i != _jointNames.end(); ++i)
    {
        fprintf(file, "%s ", i->c_str());
    }
    fprintf(file, "</joints>\n");
    fprintf(file, "<bindPoses count=\"%lu\">", _bindPoses.size());
    for (std::list<float>::const_iterator i = _bindPoses.begin(); i != _bindPoses.end(); ++i)
    {
        fprintf(file, "%f ", *i);
    }
    fprintf(file, "</bindPoses>\n");
    fprintElementEnd(file);
}

void MeshSkin::setBindShape(const float data[])
{
    for (int i = 0; i < 16; ++i)
    {
        _bindShape[i] = data[i];
    }
}

void MeshSkin::setVertexInfluenceCount(unsigned int count)
{
    _vertexInfluenceCount = count;
}

void MeshSkin::setJointNames(const std::list<std::string>& list)
{
    _jointNames = list;
}

const std::list<std::string>& MeshSkin::getJointNames()
{
    return _jointNames;
}

void MeshSkin::setJoints(const std::list<Node*>& list)
{
    _joints = list;
}

void MeshSkin::setBindPoses(std::vector<Matrix>& list)
{
    for (std::vector<Matrix>::iterator i = list.begin(); i != list.end(); ++i)
    {
        float* a = i->m;
        for (int j = 0; j < 16; ++j)
        {
            _bindPoses.push_back(a[j]);
        }
    }
}

bool MeshSkin::hasJoint(const char* id)
{
    for (std::list<std::string>::iterator i = _jointNames.begin(); i != _jointNames.end(); ++i)
    {
        if (equals(*i, id))
        {
            return true;
        }
    }
    return false;
}

}