#include "Base.h"
#include "Model.h"

namespace gameplay
{

Model::Model(void) :
    _mesh(NULL),
    _meshSkin(NULL),
    _material(NULL)
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
        writeZero(file);
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
    // Write the list of materials or zero if there are no materials
    if (_material && _materials.empty())
    {
        write((unsigned int)1, file);
        write(_material->getId(), file);
    }
    else
    {
        write((unsigned int)_materials.size(), file);
        if (_materials.size() > 0)
        {
            // Write the material names for each mesh part
            for (unsigned int i = 0; i < _materials.size(); ++i)
            {
                if (Material* mat = _materials[i])
                {
                    write(mat->getId(), file);
                }
                else
                {
                    writeZero(file);
                }
            }
        }
    }
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
    if (_material)
    {
        fprintfElement(file, "material", _material->getId().c_str());
    }
    for (unsigned int i = 0; i < _materials.size(); ++i)
    {
        if (Material* mat = _materials[i])
        {
            fprintfElement(file, "material", mat->getId().c_str());
        }
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

void Model::setMaterial(Material* material, int partIndex)
{
    if (partIndex < 0)
    {
        _material = material;
    }
    else
    {
        if ((int)_materials.size() < partIndex + 1)
        {
            _materials.resize(partIndex + 1, (Material*)NULL);
        }
        _materials[partIndex] = material;
    }
}

}
