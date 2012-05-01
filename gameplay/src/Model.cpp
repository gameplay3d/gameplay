#include "Base.h"
#include "Model.h"
#include "MeshPart.h"
#include "Scene.h"
#include "Technique.h"
#include "Pass.h"
#include "Node.h"

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

Mesh* Model::getMesh() const
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

    Material* oldMaterial = NULL;

    if (partIndex == -1)
    {
        oldMaterial = _material;

        // Set new shared material.
        if (material)
        {
            _material = material;
            _material->addRef();
        }
    }
    else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
    {
        // Ensure mesh part count is up-to-date.
        validatePartCount();

        // Release existing part material and part binding.
        if (_partMaterials)
        {
            oldMaterial = _partMaterials[partIndex];
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
        }
    }

    // Release existing material and binding.
    if (oldMaterial)
    {
        for (unsigned int i = 0, tCount = material->getTechniqueCount(); i < tCount; ++i)
        {
            Technique* t = material->getTechnique(i);
            for (unsigned int j = 0, pCount = t->getPassCount(); j < pCount; ++j)
            {
                t->getPass(j)->setVertexAttributeBinding(NULL);
            }
        }
        SAFE_RELEASE(oldMaterial);
    }

    if (material)
    {
        // Hookup vertex attribute bindings for all passes in the new material.
        for (unsigned int i = 0, tCount = material->getTechniqueCount(); i < tCount; ++i)
        {
            Technique* t = material->getTechnique(i);
            for (unsigned int j = 0, pCount = t->getPassCount(); j < pCount; ++j)
            {
                Pass* p = t->getPass(j);
                VertexAttributeBinding* b = VertexAttributeBinding::create(_mesh, p->getEffect());
                p->setVertexAttributeBinding(b);
                SAFE_RELEASE(b);
            }
        }

        // Apply node binding for the new material.
        if (_node)
        {
            setMaterialNodeBinding(material);
        }
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

bool Model::hasMaterial(unsigned int partIndex) const
{
    return (partIndex < _partCount && _partMaterials && _partMaterials[partIndex]);
}

MeshSkin* Model::getSkin() const
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
        if (_skin)
            _skin->_model = this;
    }
}

Node* Model::getNode() const
{
    return _node;
}

void Model::setNode(Node* node)
{
    _node = node;

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
    unsigned int partCount = _mesh->getPartCount();
    if (partCount == 0)
    {
        // No mesh parts (index buffers).
        if (_material)
        {
            Technique* technique = _material->getTechnique();
            unsigned int passCount = technique->getPassCount();
            for (unsigned int i = 0; i < passCount; ++i)
            {
                Pass* pass = technique->getPass(i);
                pass->bind();
                GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
                if (wireframe && (_mesh->getPrimitiveType() == Mesh::TRIANGLES || _mesh->getPrimitiveType() == Mesh::TRIANGLE_STRIP))
                {
                    unsigned int vertexCount = _mesh->getVertexCount();
                    for (unsigned int j = 0; j < vertexCount; j += 3)
                    {
                        GL_ASSERT( glDrawArrays(GL_LINE_LOOP, j, 3) );
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
        for (unsigned int i = 0; i < partCount; ++i)
        {
            MeshPart* part = _mesh->getPart(i);

            // Get the material for this mesh part.
            Material* material = getMaterial(i);
            if (material)
            {
                Technique* technique = material->getTechnique();
                unsigned int passCount = technique->getPassCount();
                for (unsigned int j = 0; j < passCount; ++j)
                {
                    Pass* pass = technique->getPass(j);
                    pass->bind();
                    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->_indexBuffer) );
                    if (wireframe && (_mesh->getPrimitiveType() == Mesh::TRIANGLES || _mesh->getPrimitiveType() == Mesh::TRIANGLE_STRIP))
                    {
                        unsigned int indexCount = part->getIndexCount();
                        unsigned int indexSize = 0;
                        switch (part->getIndexFormat())
                        {
                        case Mesh::INDEX8:
                            indexSize = 1;
                            break;
                        case Mesh::INDEX16:
                            indexSize = 2;
                            break;
                        case Mesh::INDEX32:
                            indexSize = 4;
                            break;
                        }

                        for (unsigned int k = 0; k < indexCount; k += 3)
                        {
                            GL_ASSERT( glDrawElements(GL_LINE_LOOP, 3, part->getIndexFormat(), ((const GLvoid*)(k*indexSize))) );
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

Model* Model::clone(NodeCloneContext &context)
{
    Model* model = Model::create(getMesh());
    if (getSkin())
    {
        model->setSkin(getSkin()->clone(context));
    }
    Material* materialClone = getMaterial()->clone(context);
    model->setMaterial(materialClone); // TODO: Don't forget material parts
    materialClone->release();
    return model;
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
