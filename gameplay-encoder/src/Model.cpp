#include "Model.h"

namespace gameplay
{

Model::Model(void) :
    ref(NULL),
    meshSkin(NULL)
{
}

Model::~Model(void)
{
}

unsigned int Model::getTypeId(void) const
{
    return MODEL_ID;
}
const char* Model::getElementName(void) const
{
    return "Model";
}
void Model::writeBinary(FILE* file)
{
    Object::writeBinary(file);

    // xref:Mesh
    if (ref != NULL)
        ref->writeBinaryXref(file);
    else
        write((unsigned int)0, file);
    // meshSkin
    // Write one unsigned char to indicate if this model has a skin
    if (meshSkin != NULL)
    {
        write((bool)true, file); // has a skin
        meshSkin->writeBinary(file);
    }
    else
    {
        write((bool)false, file); // doesn't have a skin
    }
    // materials[]
    writeBinaryObjects(materials, file);

}
void Model::writeText(FILE* file)
{
    fprintElementStart(file);
    if (ref != NULL)
    {
        fprintfElement(file, "ref", ref->getId());
    }
    if (meshSkin != NULL)
    {
        meshSkin->writeText(file);
    }
    fprintElementEnd(file);
}

MeshSkin* Model::getSkin()
{
    return meshSkin;
}

void Model::setMesh(Mesh* mesh)
{
    ref = mesh;
}

void Model::setSkin(MeshSkin* skin)
{
    meshSkin = skin;
}

}