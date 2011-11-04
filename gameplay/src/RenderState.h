/**
 * RenderState.h
 */

#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include "Ref.h"

namespace gameplay
{

class MaterialParameter;
class Node;
class Pass;

class RenderState : public Ref
{
    friend class Material;
    friend class Technique;
    friend class Pass;
    friend class Model;

public:

    /**
     * Built-in auto-bind targets for material parameters.
     */
    enum AutoBinding
    {
        NONE,

        /**
         * Binds a node's World matrix.
         */
        WORLD_MATRIX,

        /**
         * Binds the View matrix of the active camera for the node's scene.
         */
        VIEW_MATRIX,

        /**
         * Binds a node's WorldView matrix.
         */
        WORLD_VIEW_MATRIX,

        /**
         * Binds a node's WorldViewProjection matrix.
         */
        WORLD_VIEW_PROJECTION_MATRIX,

        /**
         * Binds a node's InverseTransposeWorldView matrix.
         */
        INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,

        /**
         * Binds the position (Vector3) of the active camera for the node's scene.
         */
        CAMERA_POSITION,

        /**
         * Binds the matrix palette of MeshSkin attached to a node's model.
         */
        MATRIX_PALETTE
    };

    /**
     * Defines blend constants supported by the blend function.
     */
    enum Blend
    {
        BLEND_ZERO = GL_ZERO,
        BLEND_ONE = GL_ONE,
        BLEND_SRC_ALPHA = GL_SRC_ALPHA,
        BLEND_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        BLEND_DST_ALPHA = GL_DST_ALPHA,
        BLEND_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        BLEND_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        BLEND_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        BLEND_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
    };

    /**
     * Returns a MaterialParameter for the specified name.
     * 
     * The returned MaterialParameter can be used to set values for the specified
     * parameter name.
     *
     * @param name Material parameter (uniform) name.
     * 
     * @return A MaterialParameter for the specified name.
     */
    MaterialParameter* getParameter(const char* name) const;

    /**
     * Sets a material parameter auto-binding.
     *
     * @param name The name of the material parameter to store an auto-binding for.
     * @param autoBinding A valid AutoBinding value.
     */
    void setParameterAutoBinding(const char* name, AutoBinding autoBinding);

    /**
     * Sets a material parameter auto-binding.
     *
     * This method parses the passed in autoBinding string and attempts to convert it
     * to an AutoBinding enumeration value, which is then stored in this render state.
     *
     * @param name The name of the material parameter to store an auto-binding for.
     * @param autoBinding A string matching one of the built-in AutoBinding enum constants.
     */
    void setParameterAutoBinding(const char* name, const char* autoBinding);

    /**
     * Sets a render state from the given name and value strings.
     *
     * This method attempts to interpret the passed in strings as render state
     * name and value. This is normally used when loading render states from
     * material files.
     *
     * @param name Name of the render state to set.
     * @param value Value of the specified render state.
     */
    void setRenderState(const char* name, const char* value);

    /**
     * Toggles blending.
     *
      * @param enabled true to enable, false to disable.
     */
    void setBlend(bool enabled);

    /**
     * Explicitly sets the source used in the blend function for this render state.
     *
     * Note that the blend function is only applied when blending is enabled.
     *
     * @param blend Specifies how the source blending factors are computed.
     */
    void setSrcBlend(Blend blend);

    /**
     * Explicitly sets the source used in the blend function for this render state.
     *
     * Note that the blend function is only applied when blending is enabled.
     *
     * @param blend Specifies how the destination blending factors are computed.
     */
    void setDstBlend(Blend blend);
    
    /**
     * Explicitly enables or disables backface culling.
     *
     * @param enabled true to enable, false to disable.
     */
    void setCullFace(bool enabled);

    /**
     * Toggles depth testing.
     *
     * @param enabled true to enable, false to disable.
     */
    void setDepthTest(bool enabled);

    /** 
     * Toggles depth writing.
     *
     * @param enabled true to enable, false to disable.
     */
    void setDepthWrite(bool enabled);

protected:

    /**
     * Stores render state to be applied or reverted.
     */
    class StateBlock
    {
    public:

        StateBlock();

        long bits;
        unsigned char blendEnabled;
        unsigned char cullFaceEnabled;
        unsigned char depthTestEnabled;
        unsigned char depthWriteEnabled;
        Blend srcBlend;
        Blend dstBlend;
    };

    /**
     * Constructor.
     */
    RenderState();

    /**
     * Hidden copy constructor.
     */
    RenderState(const RenderState& copy);

    /**
     * Destructor.
     */
    virtual ~RenderState();

    /**
     * Sets the node that this render state is bound to.
     *
     * The specified node is used to apply auto-bindings for the render state.
     * This is typically set to the node of the model that a material is 
     * applied to.
     *
     * @param node The node to use for applying auto-bindings.
     */
    void setNodeBinding(Node* node);

    /**
     * Applies the specified auto-binding.
     */
    void applyAutoBinding(const char* uniformName, AutoBinding binding);

    /**
     * Binds this render state for the given pass.
     */
    void bind(Pass* pass);

    /**
     * Unbinds this render state for the given pass.
     */
    void unbind(Pass* pass);

    /**
     * Creates the render state block(s) if neccessary.
     */
    void createStateBlock();
    
    mutable std::vector<MaterialParameter*> _parameters;
    std::map<std::string, AutoBinding> _autoBindings;
    Node* _nodeBinding;
    StateBlock* _state;
    StateBlock* _restoreState;
};

}

// Include MaterialParameter after the Pass class declaration
// to avoid an erroneous circular dependency during compilation.
#include "MaterialParameter.h"

#endif
