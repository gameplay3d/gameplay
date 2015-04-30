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
    std::vector<VertexAttributeBinding*>::iterator itr = std::find(__vertexAttributeBindingCache.begin(), __vertexAttributeBindingCache.end(), this);
    if (itr != __vertexAttributeBindingCache.end())
    {
        __vertexAttributeBindingCache.erase(itr);
    }

    SAFE_RELEASE(_mesh);
    SAFE_RELEASE(_effect);
    SAFE_DELETE_ARRAY(_attributes);

    if (_handle)
    {
        GL_ASSERT( glDeleteVertexArrays(1, &_handle) );
        _handle = 0;
    }
}

VertexAttributeBinding* VertexAttributeBinding::create(Mesh* mesh, Effect* effect)
{
    GP_ASSERT(mesh);

    // Search for an existing vertex attribute binding that can be used.
    VertexAttributeBinding* b;
    for (size_t i = 0, count = __vertexAttributeBindingCache.size(); i < count; ++i)
    {
        b = __vertexAttributeBindingCache[i];
        GP_ASSERT(b);
        if (b->_mesh == mesh && b->_effect == effect)
        {
            // Found a match!
            b->addRef();
            return b;
        }
    }

    b = create(mesh, mesh->getVertexFormat(), 0, effect);

    // Add the new vertex attribute binding to the cache.
    if (b)
    {
        __vertexAttributeBindingCache.push_back(b);
    }

    return b;
}

VertexAttributeBinding* VertexAttributeBinding::create(const VertexFormat& vertexFormat, void* vertexPointer, Effect* effect)
{
    return create(NULL, vertexFormat, vertexPointer, effect);
}

VertexAttributeBinding* VertexAttributeBinding::create(Mesh* mesh, const VertexFormat& vertexFormat, void* vertexPointer, Effect* effect)
{
    GP_ASSERT(effect);

    // One-time initialization.
    if (__maxVertexAttribs == 0)
    {
        GLint temp;
        GL_ASSERT( glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &temp) );

        __maxVertexAttribs = temp;
        if (__maxVertexAttribs <= 0)
        {
            GP_ERROR("The maximum number of vertex attributes supported by OpenGL on the current device is 0 or less.");
            return NULL;
        }
    }

    // Create a new VertexAttributeBinding.
    VertexAttributeBinding* b = new VertexAttributeBinding();

#ifdef GP_USE_VAO
    if (mesh && glGenVertexArrays)
    {
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );

        // Use hardware VAOs.
        GL_ASSERT( glGenVertexArrays(1, &b->_handle) );

        if (b->_handle == 0)
        {
            GP_ERROR("Failed to create VAO handle.");
            SAFE_DELETE(b);
            return NULL;
        }

        // Bind the new VAO.
        GL_ASSERT( glBindVertexArray(b->_handle) );

        // Bind the Mesh VBO so our glVertexAttribPointer calls use it.
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer()) );
    }
    else
#endif
    {
        // Construct a software representation of a VAO.
        VertexAttribute* attribs = new VertexAttribute[__maxVertexAttribs];
        for (unsigned int i = 0; i < __maxVertexAttribs; ++i)
        {
            // Set GL defaults
            attribs[i].enabled = GL_FALSE;
            attribs[i].size = 4;
            attribs[i].stride = 0;
            attribs[i].type = GL_FLOAT;
            attribs[i].normalized = GL_FALSE;
            attribs[i].pointer = 0;
        }
        b->_attributes = attribs;
    }

    if (mesh)
    {
        b->_mesh = mesh;
        mesh->addRef();
    }
    
    b->_effect = effect;
    effect->addRef();

    // Call setVertexAttribPointer for each vertex element.
    std::string name;
    size_t offset = 0;
    for (size_t i = 0, count = vertexFormat.getElementCount(); i < count; ++i)
    {
        const VertexFormat::Element& e = vertexFormat.getElement(i);
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
            if ((attrib = effect->getVertexAttribute(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME)) != -1)
                break;

        case VertexFormat::TEXCOORD1:
        case VertexFormat::TEXCOORD2:
        case VertexFormat::TEXCOORD3:
        case VertexFormat::TEXCOORD4:
        case VertexFormat::TEXCOORD5:
        case VertexFormat::TEXCOORD6:
        case VertexFormat::TEXCOORD7:
            name = VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME;
            name += '0' + (e.usage - VertexFormat::TEXCOORD0);
            attrib = effect->getVertexAttribute(name.c_str());
            break;
        default:
            // This happens whenever vertex data contains extra information (not an error).
            attrib = -1;
            break;
        }

        if (attrib == -1)
        {
            //GP_WARN("Warning: Vertex element with usage '%s' in mesh '%s' does not correspond to an attribute in effect '%s'.", VertexFormat::toString(e.usage), mesh->getUrl(), effect->getId());
        }
        else
        {
            void* pointer = vertexPointer ? (void*)(((unsigned char*)vertexPointer) + offset) : (void*)offset;
            b->setVertexAttribPointer(attrib, (GLint)e.size, GL_FLOAT, GL_FALSE, (GLsizei)vertexFormat.getVertexSize(), pointer);
        }

        offset += e.size * sizeof(float);
    }

    if (b->_handle)
    {
        GL_ASSERT( glBindVertexArray(0) );
    }

    return b;
}

void VertexAttributeBinding::setVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalize, GLsizei stride, void* pointer)
{
    GP_ASSERT(indx < (GLuint)__maxVertexAttribs);

    if (_handle)
    {
        // Hardware mode.
        GL_ASSERT( glVertexAttribPointer(indx, size, type, normalize, stride, pointer) );
        GL_ASSERT( glEnableVertexAttribArray(indx) );
    }
    else
    {
        // Software mode.
        GP_ASSERT(_attributes);
        _attributes[indx].enabled = true;
        _attributes[indx].size = size;
        _attributes[indx].type = type;
        _attributes[indx].normalized = normalize;
        _attributes[indx].stride = stride;
        _attributes[indx].pointer = pointer;
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
        if (_mesh)
        {
            GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVertexBuffer()) );
        }
        else
        {
            GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        }

        GP_ASSERT(_attributes);
        for (unsigned int i = 0; i < __maxVertexAttribs; ++i)
        {
            VertexAttribute& a = _attributes[i];
            if (a.enabled)
            {
                GL_ASSERT( glVertexAttribPointer(i, a.size, a.type, a.normalized, a.stride, a.pointer) );
                GL_ASSERT( glEnableVertexAttribArray(i) );
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
        if (_mesh)
        {
            GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        }

        GP_ASSERT(_attributes);
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
