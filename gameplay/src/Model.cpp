/*
 * Model.cpp
 */

#include "Base.h"
#include "Model.h"
#include "MeshPart.h"
#include "Scene.h"

namespace gameplay
{

Model::Model(Mesh* mesh) :
    _mesh(mesh), _material(NULL), _vaBinding(NULL), _partCount(0), _partMaterials(NULL), _partVaBindings(NULL), _node(NULL), _skin(NULL)
{
    _partCount = mesh->getPartCount();
    mesh->addRef();
}

Model::~Model()
{
    SAFE_RELEASE(_vaBinding);

    if (_partVaBindings)
    {
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            SAFE_RELEASE(_partVaBindings[i]);
        }

        delete[] _partVaBindings;
        _partVaBindings = NULL;
    }

    SAFE_RELEASE(_material);

    if (_partMaterials)
    {
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            SAFE_RELEASE(_partMaterials[i]);
        }

        delete[] _partMaterials;
        _partMaterials = NULL;
    }

    SAFE_RELEASE(_mesh);

    SAFE_DELETE(_skin);
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
        SAFE_RELEASE(_material);
        SAFE_RELEASE(_vaBinding)

        // Set new shared material.
        if (material)
        {
            _vaBinding = VertexAttributeBinding::create(_mesh, material->getEffect());
            if (_vaBinding)
            {
                _material = material;
                material->addRef();
            }
        }
    }
    else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
    {
        // Ensure mesh part count is up-to-date.
        validatePartCount();

        // Release existing part material and part binding.
        if (_partMaterials)
        {
            SAFE_RELEASE(_partMaterials[partIndex]);
            SAFE_RELEASE(_partVaBindings[partIndex]);
        }
        else
        {
            // Allocate part arrays for the first time.
            if (_partMaterials == NULL)
            {
                _partMaterials = new Material*[_partCount];
                memset(_partMaterials, 0, sizeof(Material*) * _partCount);

                _partVaBindings = new VertexAttributeBinding*[_partCount];
                memset(_partVaBindings, 0, sizeof(VertexAttributeBinding*) * _partCount);
            }
        }

        // Set new part material.
        if (material)
        {
            _partVaBindings[partIndex] = VertexAttributeBinding::create(_mesh, material->getEffect());
            _partMaterials[partIndex] = material;
            material->addRef();
        }
    }

    // Auto bind materials.
    autoBindParameters(material);
}

Material* Model::setMaterial(const char* vshPath, const char* fshPath, const char* defines, int partIndex)
{
    // Try to create a Material with the given parameters.
    Material* mat = Material::createMaterial(vshPath, fshPath, defines);
    if (mat == NULL)
    {
        return NULL;
    }

    // Assign the material to us.
    setMaterial(mat, partIndex);

    // Release the material since we now have a reference to it.
    mat->release();

    return mat;
}

MeshSkin* Model::getSkin()
{
    return _skin;
}

void Model::setSkin(MeshSkin* skin)
{
    if (_skin != skin)
    {
        if (_skin)
        {
            delete _skin;
        }
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
        SAFE_RELEASE(_node);

        _node = node;

        if (_node)
        {
            node->addRef();
        }
    }
}

void Model::draw(bool wireframe)
{
    unsigned int count = _mesh->getPartCount();
    if (count == 0)
    {
        // No mesh parts (index buffers).
        if (_material && _vaBinding)
        {
            _material->bind();
            _vaBinding->bind();
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
            _vaBinding->unbind();
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

            // Get the vertex attribute binding for this mesh part.
            VertexAttributeBinding* partVaBinding;
            if (_partVaBindings && i < _partCount && _partVaBindings[i])
            {
                partVaBinding = _partVaBindings[i]; // Use part binding
            }
            else
            {
                partVaBinding = _vaBinding; // Use shared binding
            }

            if (material && partVaBinding)
            {
                material->bind();
                partVaBinding->bind();
                GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->_indexBuffer) );
                GL_ASSERT( glDrawElements(part->getPrimitiveType(), part->getIndexCount(), part->getIndexFormat(), 0) );
                partVaBinding->unbind();
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
            delete[] oldArray;
            oldArray = NULL;
        }

        if (_partVaBindings)
        {
            VertexAttributeBinding** oldArray = _partVaBindings;
            _partVaBindings = new VertexAttributeBinding*[partCount];
            memset(_partVaBindings, 0, sizeof(VertexAttributeBinding*) * partCount);
            for (unsigned int i = 0; i < _partCount; ++i)
            {
                _partVaBindings[i] = oldArray[i];
            }
            delete[] oldArray;
            oldArray = NULL;
        }

        // Update local part count.
        _partCount = _mesh->getPartCount();
    }
}

void Model::autoBindParameters(Material *m)
{
    Effect* e = m->getEffect();
    
    // Go through all the shader uniforms to find the auto-bind(able) uniforms.
    int uniformCount = e->getUniformCount();
    for (int i = 0; i < uniformCount; i++)
    {
        char* uniformName = (char*)e->getUniform(i)->getName();
        if (strcmp(uniformName, UNIFORM_WORLD_MATRIX) == 0)
        {
            m->getParameter(UNIFORM_WORLD_MATRIX)->bindValue(getNode(), &Node::getWorldMatrix);
        }
        else if (strcmp(uniformName, UNIFORM_VIEW_MATRIX) == 0)
        {
            m->getParameter(UNIFORM_VIEW_MATRIX)->bindValue(getNode(), &Node::getViewMatrix);
        }
        else if (strcmp(uniformName, UNIFORM_WORLD_VIEW_PROJECTION_MATRIX) == 0)
        {
            m->getParameter(UNIFORM_WORLD_VIEW_PROJECTION_MATRIX)->bindValue(getNode(), &Node::getWorldViewProjectionMatrix);
        }
        else if (strcmp(uniformName, UNIFORM_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX) == 0)
        {
            m->getParameter(UNIFORM_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)->bindValue(getNode(), &Node::getInverseTransposeWorldViewMatrix);
        }

        else if (strcmp(uniformName, UNIFORM_CAMERA_POSITION) == 0)
        {
            Node* node = getNode();
            if (node)
            {
                // Get this scene's active camera and try to bind its world-space position.
                Camera* c = node->getScene()->getActiveCamera();
                if (c)
                {
                    m->getParameter(UNIFORM_CAMERA_POSITION)->bindValue(c->getNode(), &Node::getWorldTranslation);
                }
            }
        }
    }
}

Model* Model::create(Mesh* mesh)
{
    return new Model(mesh);
}

}
