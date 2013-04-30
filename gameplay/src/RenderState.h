#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include "Ref.h"
#include "Vector3.h"
#include "Vector4.h"

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
        MATRIX_PALETTE,

        /**
         * Binds the current scene's ambient color (Vector3).
         */
        SCENE_AMBIENT_COLOR,

        /**
         * Binds the current scene's light color (Vector3).
         *
         * This is typically used for the main directional light in a scene, such as the Sun.
         */
        SCENE_LIGHT_COLOR,

        /**
         * Binds the current scene's light direction (Vector3).
         *
         * This is typically used for the main directional light in a scene, such as the Sun.
         */
        SCENE_LIGHT_DIRECTION
    };

    /**
     * Callback function prototype for resolving material parameter auto bindings.
     *
     * Functions matching this callback signature can be registered via the 
     * RenderState::registerAutoBindingResolver method to extend or override the set
     * of built-in material parameter auto bindings.
     *
     * @param autoBinding Name of the auto binding to resolve.
     * @param node Node that is bound to the material of the specified parameter.
     * @param parameter Material parameter to set the binding on.
     *
     * @return True ONLY if the implementations explicitly handles the auto binding, false otherwise.
     *      Returning true here will prevent any further code (including built-in resolving code) from
     *      handling the auto binding.
     *
     * @see RenderState::registerAutoBindingResolver(const char*, RenderState::AutoBindingResolver)
     * @script{ignore}
     */
    typedef bool (*ResolveAutoBindingCallback) (const char* autoBinding, Node* node, MaterialParameter* parameter);

    /**
     * Defines blend constants supported by the blend function.
     */
    enum Blend
    {
        BLEND_ZERO = GL_ZERO,
        BLEND_ONE = GL_ONE,
        BLEND_SRC_COLOR = GL_SRC_COLOR,
        BLEND_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
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
     * Defines the supported depth compare functions.
     *
     * Depth compare functions specify the comparison that takes place between the
     * incoming pixel's depth value and the depth value already in the depth buffer.
     * If the compare function passes, the new pixel will be drawn.
     *
     * The intial depth compare function is DEPTH_LESS.
     */
    enum DepthFunction
    {
        DEPTH_NEVER = GL_NEVER,
        DEPTH_LESS = GL_LESS,
        DEPTH_EQUAL = GL_EQUAL,
        DEPTH_LEQUAL = GL_LEQUAL,
        DEPTH_GREATER = GL_GREATER,
        DEPTH_NOTEQUAL = GL_NOTEQUAL,
        DEPTH_GEQUAL = GL_GEQUAL,
        DEPTH_ALWAYS = GL_ALWAYS
    };

    /**
     * Defines culling criteria for front-facing, back-facing and both-side 
     * facets.
     */
    enum CullFaceSide
    {
        CULL_FACE_SIDE_BACK = GL_BACK,
        CULL_FACE_SIDE_FRONT = GL_FRONT,
        CULL_FACE_SIDE_FRONT_AND_BACK = GL_FRONT_AND_BACK
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
         * @script{create}
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
         * Sets the side of the facets to cull.
         *
         * When not explicitly set, the default is to cull back-facing facets.
         * @param side The side to cull.
         */
        void setCullFaceSide(CullFaceSide side);

        /**
         * Toggles depth testing.
         *
         * By default, depth testing is disabled.
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
         * Sets the depth function to use when depth testing is enabled.
         *
         * When not explicitly set and when depth testing is enabled, the default
         * depth function is DEPTH_LESS.
         *
         * @param func The depth function.
         */
        void setDepthFunction(DepthFunction func);

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

        void cloneInto(StateBlock* state);

        // States
        bool _cullFaceEnabled;
        bool _depthTestEnabled;
        bool _depthWriteEnabled;
        DepthFunction _depthFunction;
        bool _blendEnabled;
        Blend _blendSrc;
        Blend _blendDst;
        CullFaceSide _cullFaceSide;
        long _bits;

        static StateBlock* _defaultState;
    };

    /**
     * Returns a MaterialParameter for the specified name.
     * 
     * The returned MaterialParameter can be used to set values for the specified
     * parameter name.
     *
     * Note that this method causes a new MaterialParameter to be created if one
     * does not already exist for the given parameter name.
     *
     * @param name Material parameter (uniform) name.
     * 
     * @return A MaterialParameter for the specified name.
     */
    MaterialParameter* getParameter(const char* name) const;

    /**
     * Clears the MaterialParameter with the given name.
     *
     * If a material parameter exists for the given name, it is destroyed and
     * removed from this RenderState.
     *
     * @param name Material parameter (uniform) name.
     */
    void clearParameter(const char* name);

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
     * and any changes to the StateBlock will be reflected in all objects
     * that reference it.
     *
     * @return The StateBlock for this RenderState.
     */
    StateBlock* getStateBlock() const;

    /**
     * Registers a custom auto binding resolver.
     *
     * Implementing a custom auto binding resolver allows the set of built-in parameter auto
     * bindings to be extended or overridden. Any parameter auto binding that is set on a
     * material will be forwarded to any custom auto binding resolvers, in the order in which
     * they are registered. If a registered resolver returns true (specifying that it handles
     * the specified autoBinding), no further code will be executed for that autoBinding.
     * This allows auto binding resolvers to not only implement new/custom binding strings,
     * but it also lets them override existing/built-in ones. For this reason, you should
     * ensure that you ONLY return true if you explicitly handle a custom auto binding; return
     * false otherwise.
     *
     * Note that the custom resolver is called only once for a RenderState object when its
     * node binding is initially set. This occurs when a material is initially bound to a
     * Model that belongs to a Node. The resolver is NOT called each frame or each time
     * the RenderState is bound. Therefore, when implementing custom auto bindings for values
     * that change over time, the you should bind a method pointer onto the passed in
     * MaterialParaemter using the MaterialParameter::bindValue method. This way, the bound
     * method will be called each frame to set an updated value into the MaterialParameter.
     *
     * If no registered resolvers explicitly handle an auto binding, the binding will attempt
     * to be resolved using the internal/built-in resolver, which is able to handle any
     * auto bindings found in the RenderState::AutoBinding enumeration.
     *
     * @param callback Callback function for resolving parameter auto bindings.
     * @script{ignore}
     */
    static void registerAutoBindingResolver(ResolveAutoBindingCallback callback);

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
     * Applies the specified custom auto-binding.
     *
     * @param uniformName Name of the shader uniform.
     * @param autoBinding Name of the auto binding.s
     */
    void applyAutoBinding(const char* uniformName, const char* autoBinding);

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

    // Internal auto binding handler methods.
    const Matrix& autoBindingGetWorldMatrix() const;
    const Matrix& autoBindingGetViewMatrix() const;
    const Matrix& autoBindingGetProjectionMatrix() const;
    const Matrix& autoBindingGetWorldViewMatrix() const;
    const Matrix& autoBindingGetViewProjectionMatrix() const;
    const Matrix& autoBindingGetWorldViewProjectionMatrix() const;
    const Matrix& autoBindingGetInverseTransposeWorldMatrix() const;
    const Matrix& autoBindingGetInverseTransposeWorldViewMatrix() const;
    Vector3 autoBindingGetCameraWorldPosition() const;
    Vector3 autoBindingGetCameraViewPosition() const;
    const Vector4* autoBindingGetMatrixPalette() const;
    unsigned int autoBindingGetMatrixPaletteSize() const;
    const Vector3& autoBindingGetAmbientColor() const;
    const Vector3& autoBindingGetLightColor() const;
    const Vector3& autoBindingGetLightDirection() const;

protected:

    /**
     * Collection of MaterialParameter's to be applied to the gameplay::Effect.
     */
    mutable std::vector<MaterialParameter*> _parameters;

    /**
     * Map of parameter names to auto binding strings.
     */
    std::map<std::string, std::string> _autoBindings;

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

    /**
     * Map of custom auto binding resolvers.
     */
    static std::vector<ResolveAutoBindingCallback> _customAutoBindingResolvers;
};

}

// Include MaterialParameter after the Pass class declaration
// to avoid an erroneous circular dependency during compilation.
#include "MaterialParameter.h"

#endif
