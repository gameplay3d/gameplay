/*
 * VertexAttributeBinding.h
 */

#ifndef VERTEXATTRIBUTEBINDING_H_
#define VERTEXATTRIBUTEBINDING_H_

#include "Ref.h"

namespace gameplay
{

class Mesh;
class Effect;

/**
 * Represents a binding between the vertex layout of a Mesh and the vertex
 * input attributes of a vertex shader (Effect).
 *
 * In a perfect world, this class would be a binding directly between
 * a unique VertexFormat and an Effect, where the VertexFormat is simply the
 * definition of the layout of any annoymous vertex buffer. However, the OpenGL
 * mechanism for setting up these bindings is Vertex Array Objects (VAOs).
 * OpenGL requires a separate VAO per vertex buffer object (VBO), rather than per
 * vertex layout definition. Therefore, although we would like to define this
 * binding between a VertexFormat and Effect, we are specifying the binding
 * between a Mesh and Effect to satisfy the OpenGL require of one VAO per VBO.
 */
class VertexAttributeBinding : public Ref
{
public:

    /**
     * Creates a new VertexAttributeBinding between the given Mesh and Effect.
     *
     * @param mesh The mesh.
     * @param effect The effect.
     */
    static VertexAttributeBinding* create(Mesh* mesh, Effect* effect);

    /**
     * Binds this vertex array object.
     */
    void bind();

    /**
     * Unbinds this vertex array object.
     */
    void unbind();

private:

    class VertexAttribute
    {
    public:
        bool enabled;
        int size;
        GLenum type;
        bool normalized;
        unsigned int stride;
        unsigned int offset;
    };

    /**
     * Constructor.
     */
    VertexAttributeBinding();

    /**
     * Destructor.
     */
    ~VertexAttributeBinding();

    void setVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalize, GLsizei stride, unsigned int offset);

    GLuint _handle;
    VertexAttribute* _attributes;
    Mesh* _mesh;
    Effect* _effect;
};

}

#endif
