#include "Model.h"

namespace gameplay
{

Model::Model(void) :
    _ref(NULL),
    _meshSkin(NULL)
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
    if (_ref != NULL)
    {
        _ref->writeBinaryXref(file);
    }
    else
    {
        write((unsigned int)0, file);
    }
    // _meshSkin
    // Write one unsigned char to indicate if this model has a skin
    if (_meshSkin != NULL)
    {
        write((bool)true, file); // has a skin
        _meshSkin->writeBinary(file);
    }
    else
    {
        write((bool)false, file); // doesn't have a skin
    }
    // materials[]
    writeBinaryObjects(_materials, file);

}
void Model::writeText(FILE* file)
{
    fprintElementStart(file);
    if (_ref != NULL)
    {
        fprintfElement(file, "ref", _ref->getId());
    }
    if (_meshSkin != NULL)
    {
        _meshSkin->writeText(file);
    }
    fprintElementEnd(file);
}

MeshSkin* Model::getSkin()
{
    return _meshSkin;
}

void Model::setMesh(Mesh* mesh)
{
    _ref = mesh;
}

void Model::setSkin(MeshSkin* skin)
{
    _meshSkin = skin;
}

}