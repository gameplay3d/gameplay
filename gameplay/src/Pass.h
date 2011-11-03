/*
 * Pass.h
 */

#ifndef PASS_H_
#define PASS_H_

#include "RenderState.h"
#include "VertexAttributeBinding.h"

namespace gameplay
{

class Technique;

/**
 * Defines a pass for an object to be rendered.
 *
 * This class encapsulates the parameters and logic required to apply a shader
 * to an object to be rendered. This includes specifying both a vertex and fragment
 * shader, as well as any uniforms and vertex attributes to be applied to these.
 */
class Pass : public RenderState
{
    friend class Technique;
    friend class Material;
    friend class RenderState;

public:

    /**
     * Returns the Id of this pass.
     */ 
    const char* getId() const;

    /**
     * Returns the effect for this Pass.
     */
    Effect* getEffect() const;

    /**
     * Get a list of properties to be auto-bound.
     */
    const std::vector<std::string>* getAutoBindProperties() const;

    /**
     * Stores a binding for this pass onto the specified mesh.
     *
     * This method creates and stores a VertexAttributeBinding for this pass onto the 
     * specified Mesh. When a mesh binding is set, the VertexAttributeBinding
     * will be automatically bound when the bind() method is called for the pass.
     *
     * @param mesh The Mesh to create and store a VertexAttributeBinding for (or NULL to remove an existing mesh binding).
     *
     * @return The newly created VertexAttributeBinding between this pass and the specified mesh.
     */
    VertexAttributeBinding* setMeshBinding(Mesh* mesh);

    /**
     * Binds the render state for this pass.
     *
     * This method should be called before executing any drawing code that should
     * use this pass. When drawing code is complete, the unbind() method should be called.
     */
    void bind();

    /**
     * Unbinds the render state for this pass.
     * 
     * This method should always be called when rendering for a pass is complete, to
     * restore the render state to the state it was in previous to calling bind().
     */
    void unbind();

private:

    /**
     * Constructor.
     */
    Pass(const char* id, Technique* technique, Effect* effect);

    /**
     * Hidden copy constructor.
     */
    Pass(const Pass& copy);

    /**
     * Destructor.
     */
    ~Pass();

    /**
     * Creates a new pass for the given shaders.
     */
    static Pass* create(const char* id, Technique* technique, const char* vshPath, const char* fshPath, const char* defines);

    std::string _id;
    Technique* _technique;
    Effect* _effect;
    VertexAttributeBinding* _vaBinding;
};

}

#endif
