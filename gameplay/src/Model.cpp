/*
 * Model.cpp
 */

#include "Base.h"
#include "Model.h"
#include "MeshPart.h"
#include "Scene.h"
#include "Technique.h"
#include "Pass.h"

namespace gameplay
{

Model::Model(Mesh* mesh) :
    _mesh(mesh), _material(NULL), _partCount(0), _partMaterials(NULL), _node(NULL), _skin(NULL)
{
    _partCount = mesh->getPartCount();
}

Model::~Model()
{
    SAFE_RELEASE(_material);

    if (_partMaterials)
    {
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            SAFE_RELEASE(_partMaterials[i]);
        }
        SAFE_DELETE_ARRAY(_partMaterials);
    }

    SAFE_RELEASE(_mesh);

    SAFE_DELETE(_skin);
}

Model* Model::create(Mesh* mesh)
{
    mesh->addRef();
    return new Model(mesh);
}

Mesh* Model::getMesh()
{
    return _mesh;
}

unsigned int Model::getMeshPartCount() const
{
    return _mesh->getPartCount();
}

Material* Model::getMaterial(int partIndex)
{
    assert(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

    Material* m = NULL;

    if (partIndex >= 0 && partIndex < (int)_partCount)
    {
        // Look up explicitly specified part material.
        if (_partMaterials)
        {
            m = _partMaterials[partIndex];
        }
    }

    if (m == NULL)
    {
        // Return the shared material.
         m = _material;
    }

    return m;
}

void Model::setMaterial(Material* material, int partIndex)
{
    assert(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

    if (partIndex == -1)
    {
        // Release existing shared material and binding.
        if (_material)
        {
            _material->setMeshBinding(NULL);
            SAFE_RELEASE(_material);
        }

        // Set new shared material.
        if (material)
        {
            _material = material;
            _material->addRef();
            _material->setMeshBinding(_mesh);
        }
    }
    else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
    {
        // Ensure mesh part count is up-to-date.
        validatePartCount();

        // Release existing part material and part binding.
        if (_partMaterials)
        {
            if (_partMaterials[partIndex])
            {
                _partMaterials[partIndex]->setMeshBinding(NULL);
                SAFE_RELEASE(_partMaterials[partIndex]);
            }
        }
        else
        {
            // Allocate part arrays for the first time.
            if (_partMaterials == NULL)
            {
                _partMaterials = new Material*[_partCount];
                memset(_partMaterials, 0, sizeof(Material*) * _partCount);
            }
        }

        // Set new part material.
        if (material)
        {
            _partMaterials[partIndex] = material;
            material->addRef();
            material->setMeshBinding(_mesh);
        }
    }

    // Apply node binding for the new material.
    if (material && _node)
    {
        setMaterialNodeBinding(material);
    }
}

Material* Model::setMaterial(const char* vshPath, const char* fshPath, const char* defines, int partIndex)
{
    // Try to create a Material with the given parameters.
    Material* material = Material::create(vshPath, fshPath, defines);
    if (material == NULL)
    {
        return NULL;
    }

    // Assign the material to us.
    setMaterial(material, partIndex);

    // Release the material since we now have a reference to it.
    material->release();

    return material;
}

Material* Model::setMaterial(const char* materialPath, int partIndex)
{
    // Try to create a Material from the specified material file.
    Material* material = Material::create(materialPath);
    if (material == NULL)
    {
        return NULL;
    }

    // Assign the material to us
    setMaterial(material, partIndex);

    // Release the material since we now have a reference to it
    material->release();

    return material;
}

MeshSkin* Model::getSkin()
{
    return _skin;
}

void Model::setSkin(MeshSkin* skin)
{
    if (_skin != skin)
    {
        // Free the old skin
        SAFE_DELETE(_skin);

        // Assign the new skin
        _skin = skin;
        _skin->_model = this;
    }
}

Node* Model::getNode() const
{
    return _node;
}

void Model::setNode(Node* node)
{
    if (_node != node)
    {
        _node = node;
    }

    // Re-bind node related material parameters
    if (node)
    {
        if (_material)
        {
           setMaterialNodeBinding(_material);
        }
        if (_partMaterials)
        {
            for (unsigned int i = 0; i < _partCount; ++i)
            {
                if (_partMaterials[i])
                {
                    setMaterialNodeBinding(_partMaterials[i]);
                }
            }
        }
    }
}

void Model::draw(bool wireframe)
{
    unsigned int count = _mesh->getPartCount();
    if (count == 0)
    {
        // No mesh parts (index buffers).
        if (_material)
        {
            Technique* technique = _material->getTechnique();
            for (unsigned int i = 0, count = technique->getPassCount(); i < count; ++i)
            {
                Pass* pass = technique->getPass(i);
                pass->bind();
                if (wireframe)
                {
                    for (unsigned int i = 0, count = _mesh->getVertexCount(); i < count; i += 3)
                    {
                        GL_ASSERT( glDrawArrays(GL_LINE_LOOP, i, 3) );
                    }
                }
                else
                {
                    GL_ASSERT( glDrawArrays(_mesh->getPrimitiveType(), 0, _mesh->getVertexCount()) );
                }
                pass->unbind();
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < count; ++i)
        {
            MeshPart* part = _mesh->getPart(i);

            // Get the material for this mesh part.
            Material* material;
            if (_partMaterials && i < _partCount && _partMaterials[i])
            {
                material = _partMaterials[i]; // Use part material
            }
            else
            {
                material = _material; // Use shared material
            }

            if (material)
            {
                Technique* technique = material->getTechnique();
                for (unsigned int i = 0, count = technique->getPassCount(); i < count; ++i)
                {
                    Pass* pass = technique->getPass(i);
                    pass->bind();
                    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->_indexBuffer) );
                    if (wireframe)
                    {
                        for (unsigned int i = 0, count = part->getIndexCount(); i < count; i += 3)
                        {
                            GL_ASSERT( glDrawElements(GL_LINE_LOOP, 3, part->getIndexFormat(), (const GLvoid*)i) );
                        }
                    }
                    else
                    {
                        GL_ASSERT( glDrawElements(part->getPrimitiveType(), part->getIndexCount(), part->getIndexFormat(), 0) );
                    }
                    pass->unbind();
                }
            }
        }
    }
}

void Model::validatePartCount()
{
    unsigned int partCount = _mesh->getPartCount();

    if (_partCount != partCount)
    {
        // Allocate new arrays and copy old items to them.
        if (_partMaterials)
        {
            Material** oldArray = _partMaterials;
            _partMaterials = new Material*[partCount];
            memset(_partMaterials, 0, sizeof(Material*) * partCount);
            for (unsigned int i = 0; i < _partCount; ++i)
            {
                _partMaterials[i] = oldArray[i];
            }
            SAFE_DELETE_ARRAY(oldArray);
        }

        // Update local part count.
        _partCount = _mesh->getPartCount();
    }
}

void Model::setMaterialNodeBinding(Material *material)
{
    if (_node)
    {
        material->setNodeBinding(_node);

        unsigned int techniqueCount = material->getTechniqueCount();
        for (unsigned int i = 0; i < techniqueCount; ++i)
        {
            Technique* technique = material->getTechnique(i);
            
            technique->setNodeBinding(_node);

            unsigned int passCount = technique->getPassCount();
            for (unsigned int j = 0; j < passCount; ++j)
            {
                Pass* pass = technique->getPass(j);

                pass->setNodeBinding(_node);
            }
        }
    }
}

}
