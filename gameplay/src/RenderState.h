#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include "Ref.h"

namespace gameplay
{

class MaterialParameter;
class Node;
class NodeCloneContext;
class Pass;

/**
 * Defines the render state of the graphics device.
 */
class RenderState : public Ref
{
    friend class Game;
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
         * Binds the Projection matrix of the active camera for the node's scene.
         */
        PROJECTION_MATRIX,

        /**
         * Binds a node's WorldView matrix.
         */
        WORLD_VIEW_MATRIX,

        /**
         * Binds the ViewProjection matrix of the active camera for the node's scene.
         */
        VIEW_PROJECTION_MATRIX,

        /**
         * Binds a node's WorldViewProjection matrix.
         */
        WORLD_VIEW_PROJECTION_MATRIX,

        /**
         * Binds a node's InverseTransposeWorl matrix.
         */
        INVERSE_TRANSPOSE_WORLD_MATRIX,

        /**
         * Binds a node's InverseTransposeWorldView matrix.
         */
        INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,

        /**
         * Binds the position (Vector3) of the active camera for the node's scene.
         */
        CAMERA_WORLD_POSITION,

        /**
         * Binds the view-space position (Vector3) of the active camera for the node's scene.
         */
        CAMERA_VIEW_POSITION,

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
        BLEND_SRC_COLOR = GL_SRC_COLOR,
        BLEN_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        BLEND_DST_COLOR = GL_DST_COLOR,
        BLEND_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        BLEND_SRC_ALPHA = GL_SRC_ALPHA,
        BLEND_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        BLEND_DST_ALPHA = GL_DST_ALPHA,
        BLEND_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        BLEND_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        BLEND_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        BLEND_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
    };

    /**
     * Defines a block of fixed-function render states that can be applied to a
     * RenderState object.
     */
    class StateBlock : public Ref
    {
        friend class RenderState;
        friend class Game;

    public:

        /**
         * Creates a new StateBlock with default render state settings.
         */
        static StateBlock* create();

        /**
         * Binds the state in this StateBlock to the renderer.
         *
         * This method handles both setting and restoring of render states to ensure that
         * only the state explicitly defined by this StateBlock is applied to the renderer.
         */
        void bind();

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
        void setBlendSrc(Blend blend);

        /**
         * Explicitly sets the source used in the blend function for this render state.
         *
         * Note that the blend function is only applied when blending is enabled.
         *
         * @param blend Specifies how the destination blending factors are computed.
         */
        void setBlendDst(Blend blend);
    
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
        void setState(const char* name, const char* value);

    private:

        /**
         * Constructor.
         */
        StateBlock();

        /**
         * Copy constructor.
         */
        StateBlock(const StateBlock& copy);

        /**
         * Destructor.
         */
        ~StateBlock();

        void bindNoRestore();

        static void restore(long stateOverrideBits);

        static void enableDepthWrite();

        // States
        bool _blendEnabled;
        bool _cullFaceEnabled;
        bool _depthTestEnabled;
        bool _depthWriteEnabled;
        Blend _srcBlend;
        Blend _dstBlend;
        // State bits
        long _bits;

        static StateBlock* _defaultState;
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
     * Sets the fixed-function render state of this object to the state contained
     * in the specified StateBlock.
     *
     * The passed in StateBlock is stored in this RenderState object with an 
     * increased reference count and released when either a different StateBlock
     * is assigned, or when this RenderState object is destroyed.
     *
     * @param state The state block to set.
     */
    void setStateBlock(StateBlock* state);

    /**
     * Gets the fixed-function StateBlock for this RenderState object.
     *
     * The returned StateBlock is referenced by this RenderState and therefore
     * should not be released by the user. To release a StateBlock for a
     * RenderState, the setState(StateBlock*) method should be called, passing
     * NULL. This removes the StateBlock and resets the fixed-function render
     * state to the default state.
     *
     * It is legal to pass the returned StateBlock to another RenderState object.
     * In this case, the StateBlock will be referenced by both RenderState objects
     * and any changes to the StateBlock will be refelcted in all objects
     * that reference it.
     *
     * @return The StateBlock for this RenderState.
     */
    StateBlock* getStateBlock() const;

protected:

    /**
     * Constructor.
     */
    RenderState();

    /**
     * Destructor.
     */
    virtual ~RenderState();

    /**
     * Static initializer that is called during game startup.
     */
    static void initialize();

    /**
     * Static finalizer that is called during game shutdown.
     */
    static void finalize();

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
     * Binds the render state for this RenderState and any of its parents, top-down, 
     * for the given pass.
     */
    void bind(Pass* pass);

    /**
     * Returns the topmost RenderState in the hierarchy below the given RenderState.
     */
    RenderState* getTopmost(RenderState* below);

    /**
     * Copies the data from this RenderState into the given RenderState.
     * 
     * @param renderState The RenderState to copy the data to.
     * @param context The clone context.
     */
    void cloneInto(RenderState* renderState, NodeCloneContext& context) const;

private:

    /**
     * Hidden copy constructor.
     */
    RenderState(const RenderState& copy);

    /**
     * Hidden copy assignment operator.
     */
    RenderState& operator=(const RenderState&);

protected:

    /**
     * Collection of MaterialParameter's to be applied to the gamplay::Effect.
     */
    mutable std::vector<MaterialParameter*> _parameters;
    
    /**
     * Map of IDs to AutoBindings.
     */
    std::map<std::string, AutoBinding> _autoBindings;

    /**
     * The Node bound to the RenderState.
     */
    Node* _nodeBinding;

    /**
     * The StateBlock of fixed-function render states that can be applied to the RenderState.
     */
    mutable StateBlock* _state;

    /**
     * The RenderState's parent.
     */
    RenderState* _parent;
};

}

// Include MaterialParameter after the Pass class declaration
// to avoid an erroneous circular dependency during compilation.
#include "MaterialParameter.h"

#endif
