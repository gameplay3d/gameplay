#include "Base.h"
#include "Model.h"

namespace gameplay
{

Model::Model(void) :
    _mesh(NULL),
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
    if (_mesh != NULL)
    {
        _mesh->writeBinaryXref(file);
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
    // Compute mesh bounds before writing

    fprintElementStart(file);
    if (_mesh != NULL)
    {
        fprintfElement(file, "ref", _mesh->getId());
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

Mesh* Model::getMesh()
{
    return _mesh;
}

void Model::setMesh(Mesh* mesh)
{
    _mesh = mesh;

    if (mesh)
    {
        mesh->model = this;
    }

    if (_mesh && _meshSkin)
    {
        _meshSkin->_mesh = _mesh;
    }
}

void Model::setSkin(MeshSkin* skin)
{
    _meshSkin = skin;

    if (_meshSkin)
    {
        _meshSkin->_mesh = _mesh;
    }
}

}
