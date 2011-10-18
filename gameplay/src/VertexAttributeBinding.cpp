/*
 * VertexAttributeBinding.cpp
 */

#include "Base.h"
#include "VertexAttributeBinding.h"
#include "Mesh.h"
#include "Effect.h"

namespace gameplay
{

static GLuint __maxVertexAttribs = 0;
static std::vector<VertexAttributeBinding*> __vertexAttributeBindingCache;

VertexAttributeBinding::VertexAttributeBinding() :
    _handle(0), _attributes(NULL), _mesh(NULL), _effect(NULL)
{
}

VertexAttributeBinding::~VertexAttributeBinding()
{
    // Delete from the vertex attribute binding cache.
    std::vector<VertexAttributeBinding*>::iterator itr = find(__vertexAttributeBindingCache.begin(), __vertexAttributeBindingCache.end(), this);
    if (itr != __vertexAttributeBindingCache.end())
    {
        __vertexAttributeBindingCache.erase(itr);
    }

    SAFE_RELEASE(_mesh);

    SAFE_RELEASE(_effect);

    if (_attributes)
    {
        delete[] _attributes;
        _attributes = NULL;
    }

    if (_handle)
    {
        GL_ASSERT( glDeleteVertexArrays(1, &_handle) );
        _handle = 0;
    }
}

VertexAttributeBinding* VertexAttributeBinding::create(Mesh* mesh, Effect* effect)
{
    // One-time initialization.
    if (__maxVertexAttribs == 0)
    {
        GLint temp;
        GL_ASSERT( glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &temp) );

        __maxVertexAttribs = temp;
        assert(__maxVertexAttribs > 0);
        if (__maxVertexAttribs <= 0)
        {
            return NULL;
        }
    }

    VertexAttributeBinding* b;

    // Search for an existing vertex attribute binding that can be used.
    for (unsigned int i = 0, count = __vertexAttributeBindingCache.size(); i < count; ++i)
    {
        b = __vertexAttributeBindingCache[i];
        if (b->_mesh == mesh && b->_effect == effect)
        {
            // Found a match!
            b->addRef();
            return b;
        }
    }

    // Create a new VertexAttributeBinding.
    b = new VertexAttributeBinding();

    if (glGenVertexArrays)
    {
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0) );

        // Use hardware VAOs.
        GL_ASSERT( glGenVertexArrays(1, &b->_handle) );

        if (b->_handle == 0)
        {
            delete b;
            return NULL;
        }

        // Bind the new VAO.
        GL_ASSERT( glBindVertexArray(b->_handle) );

        // Bind the Mesh VBO so our glVertexAttribPointer calls use it.
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer()) );
    }
    else
    {
        // Construct a software representation of a VAO.
        VertexAttribute* attribs = new VertexAttribute[__maxVertexAttribs];
        for (unsigned int i = 0; i < __maxVertexAttribs; ++i)
        {
            attribs[i].enabled = GL_FALSE;
            attribs[i].size = 4;
            attribs[i].stride = 0;
            attribs[i].type = GL_FLOAT;
            attribs[i].normalized = GL_FALSE;
            attribs[i].offset = 0;
        }
        b->_attributes = attribs;
    }

    b->_mesh = mesh;
    mesh->addRef();

    b->_effect = effect;
    effect->addRef();

    // Call setVertexAttribPointer for each vertex element in our mesh vertex format.
    std::string name;
    unsigned int offset = 0;
    const VertexFormat* vertexFormat = mesh->getVertexFormat();
    for (unsigned int i = 0, count = vertexFormat->getElementCount(); i < count; ++i)
    {
        const VertexFormat::Element& e = vertexFormat->getElement(i);

        gameplay::VertexAttribute attrib;

        // Constructor vertex attribute name expected in shader.
        switch (e.usage)
        {
        case VertexFormat::POSITION:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_POSITION_NAME);
            break;
        case VertexFormat::NORMAL:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_NORMAL_NAME);
            break;
        case VertexFormat::COLOR:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_COLOR_NAME);
            break;
        case VertexFormat::TANGENT:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_TANGENT_NAME);
            break;
        case VertexFormat::BINORMAL:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BINORMAL_NAME);
            break;
        case VertexFormat::BLENDWEIGHTS:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME);
            break;
        case VertexFormat::BLENDINDICES:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDINDICES_NAME);
            break;
        case VertexFormat::TEXCOORD0:
            attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX);
            // Try adding a "0" after the texcoord attrib name (flexible name for this case).
            if (attrib == -1)
            {
                name = VERTEX_ATTRIBUTE_TEXCOORD_PREFIX;
                name += "0";
                attrib = effect->getVertexAttribute(name.c_str());
            }
            break;
        case VertexFormat::TEXCOORD1:
        case VertexFormat::TEXCOORD2:
        case VertexFormat::TEXCOORD3:
        case VertexFormat::TEXCOORD4:
        case VertexFormat::TEXCOORD5:
        case VertexFormat::TEXCOORD6:
        case VertexFormat::TEXCOORD7:
            name = VERTEX_ATTRIBUTE_TEXCOORD_PREFIX;
            name += (e.usage - VertexFormat::TEXCOORD0);
            attrib = effect->getVertexAttribute(name.c_str());
            break;
        default:
            attrib = -1;
            break;
        }

        if (attrib == -1)
        {
            WARN_VARG("Vertex attribute not found for usage %d", (int)e.usage);
        }
        else
        {
            b->setVertexAttribPointer(attrib, (GLint)e.size, GL_FLOAT, GL_FALSE, (GLsizei)vertexFormat->getVertexSize(), offset);
        }

        offset += e.size * sizeof(float);
    }

    if (glBindVertexArray)
    {
        GL_ASSERT( glBindVertexArray(0) );
    }

    return b;
}

void VertexAttributeBinding::setVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalize, GLsizei stride, unsigned int offset)
{
    assert(indx < (GLuint)__maxVertexAttribs);

    if (_handle)
    {
        // Hardware mode
        GL_ASSERT( glEnableVertexAttribArray(indx) );
        GL_ASSERT( glVertexAttribPointer(indx, size, type, normalize, stride, (const GLvoid*)offset) );
    }
    else
    {
        // Software mode
        _attributes[indx].enabled = true;
        _attributes[indx].size = size;
        _attributes[indx].type = type;
        _attributes[indx].normalized = normalize;
        _attributes[indx].stride = stride;
        _attributes[indx].offset = offset;
    }
}

void VertexAttributeBinding::bind()
{
    if (_handle)
    {
        // Hardware mode
        GL_ASSERT( glBindVertexArray(_handle) );
    }
    else
    {
        // Software mode
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVertexBuffer()) );

        for (unsigned int i = 0; i < __maxVertexAttribs; ++i)
        {
            VertexAttribute& a = _attributes[i];
            if (a.enabled)
            {
                GL_ASSERT( glEnableVertexAttribArray(i) );
                GL_ASSERT( glVertexAttribPointer(i, a.size, a.type, a.normalized, a.stride, (const GLvoid*)a.offset) );
            }
        }
    }
}

void VertexAttributeBinding::unbind()
{
    if (_handle)
    {
        // Hardware mode
        GL_ASSERT( glBindVertexArray(0) );
    }
    else
    {
        // Software mode
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

        for (unsigned int i = 0; i < __maxVertexAttribs; ++i)
        {
            if (_attributes[i].enabled)
            {
                GL_ASSERT( glDisableVertexAttribArray(i) );
            }
        }
    }
}

}
