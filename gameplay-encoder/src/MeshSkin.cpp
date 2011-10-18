#include "MeshSkin.h"
#include "Node.h"
#include "StringUtil.h"

namespace gameplay
{

MeshSkin::MeshSkin(void) :
    influences(NULL),
    vertexInfluenceCount(0),
    animation(NULL)
{
    setIdentityMatrix(bindShape);
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
    // bindShape
    write(bindShape, 16, file);
    // root joint
    write(influences->getId(), file);
    /*
    if (influences != NULL)
    {
        influences->writeBinaryXref(file);
    }
    else
    {
        write((unsigned int)0, file);
    }
    */
    // joints
    write(joints.size(), file);
    for (std::list<Node*>::const_iterator i = joints.begin(); i != joints.end(); i++)
    {
        (*i)->writeBinaryXref(file);
    }
    // bindPoses
    write(bindPoses, file);
}

void MeshSkin::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintf(file, "<bindShape>");
    fprintfMatrix4f(file, bindShape);
    fprintf(file, "</bindShape>");
    if (influences != NULL)
    {
        fprintf(file, "<ref xref=\"#%s\"/>", influences->getId().c_str());
    }
    fprintf(file, "<joints>");
    for (std::list<std::string>::const_iterator i = jointNames.begin(); i != jointNames.end(); i++)
    {
        fprintf(file, "%s ", i->c_str());
    }
    fprintf(file, "</joints>\n");
    fprintf(file, "<bindPoses count=\"%u\">", bindPoses.size());
    for (std::list<float>::const_iterator i = bindPoses.begin(); i != bindPoses.end(); i++)
    {
        fprintf(file, "%f ", *i);
    }
    fprintf(file, "</bindPoses>\n");
    fprintElementEnd(file);
}

void MeshSkin::setBindShape(const float data[])
{
    for (int i = 0; i < 16; i++)
    {
        bindShape[i] = data[i];
    }
}

void MeshSkin::setVertexInfluenceCount(unsigned int count)
{
    vertexInfluenceCount = count;
}

void MeshSkin::setNode(Node* node)
{
    influences = node;
}

void MeshSkin::setJointNames(const std::list<std::string>& list)
{
    jointNames = list;
}

const std::list<std::string>& MeshSkin::getJointNames()
{
    return jointNames;
}

void MeshSkin::setJoints(const std::list<Node*>& list)
{
    joints = list;
}

void MeshSkin::setBindPoses(std::vector<Matrix>& list)
{
    for (std::vector<Matrix>::iterator i = list.begin(); i != list.end(); i++)
    {   
        float* a = i->m;
        for (int j = 0; j < 16; j++)
        {
            bindPoses.push_back(a[j]);
        }
    }
}

bool MeshSkin::hasJoint(const char* id)
{
    for (std::list<std::string>::iterator i = jointNames.begin(); i != jointNames.end(); i++)
    {
        if (equals(*i, id))
        {
            return true;
        }
    }
    return false;
}

Animation* MeshSkin::getAnimation()
{
    if (!animation)
    {
        animation = new Animation();
    }
    return animation;
}

void MeshSkin::setAnimationId(const char* id)
{
    getAnimation()->setId(id);
}

}