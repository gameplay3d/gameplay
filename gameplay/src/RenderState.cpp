#include "Base.h"
#include "RenderState.h"
#include "Node.h"
#include "Pass.h"
#include "Technique.h"
#include "Node.h"

// Render state override bits
#define RS_BLEND 1
#define RS_BLEND_FUNC 2
#define RS_CULL_FACE 4
#define RS_DEPTH_TEST 8
#define RS_DEPTH_WRITE 16

namespace gameplay
{

RenderState::StateBlock* RenderState::StateBlock::_defaultState = NULL;

RenderState::RenderState()
    : _nodeBinding(NULL), _state(NULL), _parent(NULL)
{
}

RenderState::~RenderState()
{
    SAFE_RELEASE(_state);

    // Destroy all the material parameters
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        SAFE_RELEASE(_parameters[i]);
    }
}

void RenderState::initialize()
{
    if (StateBlock::_defaultState == NULL)
    {
        StateBlock::_defaultState = StateBlock::create();
    }
}

void RenderState::finalize()
{
    SAFE_RELEASE(StateBlock::_defaultState);
}

MaterialParameter* RenderState::getParameter(const char* name) const
{
    GP_ASSERT(name);

    // Search for an existing parameter with this name.
    MaterialParameter* param;
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        param = _parameters[i];
        GP_ASSERT(param);
        if (strcmp(param->getName(), name) == 0)
        {
            return param;
        }
    }

    // Create a new parameter and store it in our list.
    param = new MaterialParameter(name);
    _parameters.push_back(param);

    return param;
}

void RenderState::setParameterAutoBinding(const char* name, AutoBinding autoBinding)
{
    GP_ASSERT(name);

    // Store the auto-binding.
    if (autoBinding == NONE)
    {
        // Clear current auto binding.
        std::map<std::string, AutoBinding>::iterator itr = _autoBindings.find(name);
        if (itr != _autoBindings.end())
        {
            _autoBindings.erase(itr);
        }
    }
    else
    {
        // Set new auto binding.
        _autoBindings[name] = autoBinding;
    }

    // If we have a currently set node binding, apply the auto binding immediately.
    if (_nodeBinding)
    {
        applyAutoBinding(name, autoBinding);
    }
}

void RenderState::setParameterAutoBinding(const char* name, const char* autoBinding)
{
    GP_ASSERT(autoBinding);
    AutoBinding value = NONE;

    // Parse the passed in autoBinding string.
    if (strcmp(autoBinding, "WORLD_MATRIX") == 0)
    {
        value = WORLD_MATRIX;
    }
    else if (strcmp(autoBinding, "VIEW_MATRIX") == 0)
    {
        value = VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "PROJECTION_MATRIX") == 0)
    {
        value = PROJECTION_MATRIX;
    }
    else if (strcmp(autoBinding, "WORLD_VIEW_MATRIX") == 0)
    {
        value = WORLD_VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "VIEW_PROJECTION_MATRIX") == 0)
    {
        value = VIEW_PROJECTION_MATRIX;
    }
    else if (strcmp(autoBinding, "WORLD_VIEW_PROJECTION_MATRIX") == 0)
    {
        value = WORLD_VIEW_PROJECTION_MATRIX;
    }
    else if (strcmp(autoBinding, "INVERSE_TRANSPOSE_WORLD_MATRIX") == 0)
    {
        value = INVERSE_TRANSPOSE_WORLD_MATRIX;
    }
    else if (strcmp(autoBinding, "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX") == 0)
    {
        value = INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "CAMERA_WORLD_POSITION") == 0)
    {
        value = CAMERA_WORLD_POSITION;
    }
    else if (strcmp(autoBinding, "CAMERA_VIEW_POSITION") == 0)
    {
        value = CAMERA_VIEW_POSITION;
    }
    else if (strcmp(autoBinding, "MATRIX_PALETTE") == 0)
    {
        value = MATRIX_PALETTE;
    }
    else
    {
        // Ignore all other cases (the value was previously set to the default of NONE).
    }

    if (value != NONE)
    {
        setParameterAutoBinding(name, value);
    }
}

void RenderState::setStateBlock(StateBlock* state)
{
    if (_state != state)
    {
        SAFE_RELEASE(_state);

        _state = state;

        if (_state)
        {
            _state->addRef();
        }
    }
}

RenderState::StateBlock* RenderState::getStateBlock() const
{
    if (_state == NULL)
    {
        _state = StateBlock::create();
    }

    return _state;
}

void RenderState::setNodeBinding(Node* node)
{
    _nodeBinding = node;

    if (_nodeBinding)
    {
        // Apply all existing auto-bindings using this node.
        std::map<std::string, AutoBinding>::const_iterator itr = _autoBindings.begin();
        while (itr != _autoBindings.end())
        {
            applyAutoBinding(itr->first.c_str(), itr->second);
            ++itr;
        }
    }
}

void RenderState::applyAutoBinding(const char* uniformName, AutoBinding autoBinding)
{
    MaterialParameter* param = getParameter(uniformName);
    switch (autoBinding)
    {
    case WORLD_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getWorldMatrix);
        break;

    case VIEW_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getViewMatrix);
        break;

    case PROJECTION_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getProjectionMatrix);
        break;

    case WORLD_VIEW_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getWorldViewMatrix);
        break;

    case VIEW_PROJECTION_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getViewProjectionMatrix);
        break;

    case WORLD_VIEW_PROJECTION_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getWorldViewProjectionMatrix);
        break;

    case INVERSE_TRANSPOSE_WORLD_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getInverseTransposeWorldMatrix);
        break;

    case INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getInverseTransposeWorldViewMatrix);
        break;

    case CAMERA_WORLD_POSITION:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getActiveCameraTranslationWorld);
        break;

    case CAMERA_VIEW_POSITION:
        GP_ASSERT(param);
        param->bindValue(_nodeBinding, &Node::getActiveCameraTranslationView);
        break;

    case MATRIX_PALETTE:
        {
            Model* model = _nodeBinding->getModel();
            MeshSkin* skin = model ? model->getSkin() : NULL;
            if (skin)
            {
                GP_ASSERT(param);
                param->bindValue(skin, &MeshSkin::getMatrixPalette, &MeshSkin::getMatrixPaletteSize);
            }
        }
        break;

    default:
        GP_ERROR("Unsupported auto binding type (%d).", autoBinding);
        break;
    }
}

void RenderState::bind(Pass* pass)
{
    GP_ASSERT(pass);

    // Get the combined modified state bits for our RenderState hierarchy.
    long stateOverrideBits = _state ? _state->_bits : 0;
    RenderState* rs = _parent;
    while (rs)
    {
        if (rs->_state)
        {
            stateOverrideBits |= rs->_state->_bits;
        }
        rs = rs->_parent;
    }

    // Restore renderer state to its default, except for explicitly specified states
    StateBlock::restore(stateOverrideBits);

    // Apply parameter bindings and renderer state for the entire hierarchy, top-down.
    rs = NULL;
    Effect* effect = pass->getEffect();
    while ((rs = getTopmost(rs)))
    {
        for (unsigned int i = 0, count = rs->_parameters.size(); i < count; ++i)
        {
            GP_ASSERT(rs->_parameters[i]);
            rs->_parameters[i]->bind(effect);
        }

        if (rs->_state)
        {
            rs->_state->bindNoRestore();
        }
    }
}

RenderState* RenderState::getTopmost(RenderState* below)
{
    RenderState* rs = this;
    if (rs == below)
    {
        // Nothing below ourself.
        return NULL;
    }

    while (rs)
    {
        if (rs->_parent == below || rs->_parent == NULL)
        {
            // Stop traversing up here.
            return rs;
        }
        rs = rs->_parent;
    }

    return NULL;
}

void RenderState::cloneInto(RenderState* renderState, NodeCloneContext& context) const
{
    GP_ASSERT(renderState);

    for (std::map<std::string, AutoBinding>::const_iterator it = _autoBindings.begin(); it != _autoBindings.end(); ++it)
    {
        renderState->setParameterAutoBinding(it->first.c_str(), it->second);
    }
    for (std::vector<MaterialParameter*>::const_iterator it = _parameters.begin(); it != _parameters.end(); ++it)
    {
        const MaterialParameter* param = *it;
        GP_ASSERT(param);

        MaterialParameter* paramCopy = new MaterialParameter(param->getName());
        param->cloneInto(paramCopy);

        renderState->_parameters.push_back(paramCopy);
    }
    renderState->_parent = _parent;
    if (Node* node = context.findClonedNode(_nodeBinding))
    {
        renderState->setNodeBinding(node);
    }
    if (_state)
    {
        renderState->setStateBlock(_state);
    }
}

RenderState::StateBlock::StateBlock()
    : _cullFaceEnabled(false), _depthTestEnabled(false), _depthWriteEnabled(false),
      _blendEnabled(false), _blendSrc(RenderState::BLEND_ONE), _blendDst(RenderState::BLEND_ZERO),
      _bits(0L)
{
}

RenderState::StateBlock::StateBlock(const StateBlock& copy)
{
    // Hidden
}

RenderState::StateBlock::~StateBlock()
{
}

RenderState::StateBlock* RenderState::StateBlock::create()
{
    return new RenderState::StateBlock();
}

void RenderState::StateBlock::bind()
{
    // When the public bind() is called with no RenderState object passed in,
    // we assume we are being called to bind the state of a single StateBlock,
    // irrespective of whether it belongs to a hierarchy of RenderStates.
    // Therefore, we call restore() here with only this StateBlock's override
    // bits to restore state before applying the new state.
    StateBlock::restore(_bits);

    bindNoRestore();
}

void RenderState::StateBlock::bindNoRestore()
{
    GP_ASSERT(_defaultState);

    // Update any state that differs from _defaultState and flip _defaultState bits
    if ((_bits & RS_BLEND) && (_blendEnabled != _defaultState->_blendEnabled))
    {
        if (_blendEnabled)
            GL_ASSERT( glEnable(GL_BLEND) );
        else
            GL_ASSERT( glDisable(GL_BLEND) );
        _defaultState->_blendEnabled = _blendEnabled;
    }
    if ((_bits & RS_BLEND_FUNC) && (_blendSrc != _defaultState->_blendSrc || _blendDst != _defaultState->_blendDst))
    {
        GL_ASSERT( glBlendFunc((GLenum)_blendSrc, (GLenum)_blendDst) );
        _defaultState->_blendSrc = _blendSrc;
        _defaultState->_blendDst = _blendDst;
    }
    if ((_bits & RS_CULL_FACE) && (_cullFaceEnabled != _defaultState->_cullFaceEnabled))
    {
        if (_cullFaceEnabled)
            GL_ASSERT( glEnable(GL_CULL_FACE) );
        else
            GL_ASSERT( glDisable(GL_CULL_FACE) );
        _defaultState->_cullFaceEnabled = _cullFaceEnabled;
    }
    if ((_bits & RS_DEPTH_TEST) && (_depthTestEnabled != _defaultState->_depthTestEnabled))
    {
        if (_depthTestEnabled) 
            GL_ASSERT( glEnable(GL_DEPTH_TEST) );
        else 
            GL_ASSERT( glDisable(GL_DEPTH_TEST) );
        _defaultState->_depthTestEnabled = _depthTestEnabled;
    }
    if ((_bits & RS_DEPTH_WRITE) && (_depthWriteEnabled != _defaultState->_depthWriteEnabled))
    {
        GL_ASSERT( glDepthMask(_depthWriteEnabled ? GL_TRUE : GL_FALSE) );
        _defaultState->_depthWriteEnabled = _depthWriteEnabled;
    }

    _defaultState->_bits |= _bits;
}

void RenderState::StateBlock::restore(long stateOverrideBits)
{
    GP_ASSERT(_defaultState);

    // If there is no state to restore (i.e. no non-default state), do nothing.
    if (_defaultState->_bits == 0)
    {
        return;
    }

    // Restore any state that is not overridden and is not default
    if (!(stateOverrideBits & RS_BLEND) && (_defaultState->_bits & RS_BLEND))
    {
        GL_ASSERT( glDisable(GL_BLEND) );
        _defaultState->_bits &= ~RS_BLEND;
        _defaultState->_blendEnabled = false;
    }
    if (!(stateOverrideBits & RS_BLEND_FUNC) && (_defaultState->_bits & RS_BLEND_FUNC))
    {
        GL_ASSERT( glBlendFunc(GL_ONE, GL_ZERO) );
        _defaultState->_bits &= ~RS_BLEND_FUNC;
        _defaultState->_blendSrc = RenderState::BLEND_ONE;
        _defaultState->_blendDst = RenderState::BLEND_ZERO;
    }
    if (!(stateOverrideBits & RS_CULL_FACE) && (_defaultState->_bits & RS_CULL_FACE))
    {
        GL_ASSERT( glDisable(GL_CULL_FACE) );
        _defaultState->_bits &= ~RS_CULL_FACE;
        _defaultState->_cullFaceEnabled = false;
    }
    if (!(stateOverrideBits & RS_DEPTH_TEST) && (_defaultState->_bits & RS_DEPTH_TEST))
    {
        GL_ASSERT( glDisable(GL_DEPTH_TEST) );
        _defaultState->_bits &= ~RS_DEPTH_TEST;
        _defaultState->_depthTestEnabled = false;
    }
    if (!(stateOverrideBits & RS_DEPTH_WRITE) && (_defaultState->_bits & RS_DEPTH_WRITE))
    {
        GL_ASSERT( glDepthMask(GL_TRUE) );
        _defaultState->_bits &= ~RS_DEPTH_WRITE;
        _defaultState->_depthWriteEnabled = true;
    }
}

void RenderState::StateBlock::enableDepthWrite()
{
    GP_ASSERT(_defaultState);

    // Internal method used by Game::clear() to restore depth writing before a
    // clear operation. This is necessary if the last code to draw before the
    // next frame leaves depth writing disabled.
    if (!_defaultState->_depthWriteEnabled)
    {
        GL_ASSERT( glDepthMask(GL_TRUE) );
        _defaultState->_bits &= ~RS_DEPTH_WRITE;
        _defaultState->_depthWriteEnabled = true;
    }
}

static bool parseBoolean(const char* value)
{
    GP_ASSERT(value);

    if (strlen(value) == 4)
    {
        return (
            tolower(value[0]) == 't' &&
            tolower(value[1]) == 'r' &&
            tolower(value[2]) == 'u' &&
            tolower(value[3]) == 'e' );
    }

    return false;
}

static RenderState::Blend parseBlend(const char* value)
{
    GP_ASSERT(value);

    // Convert the string to uppercase for comparison.
    std::string upper(value);
    std::transform(upper.begin(), upper.end(), upper.begin(), (int(*)(int))toupper);
    if (upper == "ZERO")
        return RenderState::BLEND_ZERO;
    else if (upper == "ONE")
        return RenderState::BLEND_ONE;
    else if (upper == "SRC_ALPHA")
        return RenderState::BLEND_SRC_ALPHA;
    else if (upper == "ONE_MINUS_SRC_ALPHA")
        return RenderState::BLEND_ONE_MINUS_SRC_ALPHA;
    else if (upper == "DST_ALPHA")
        return RenderState::BLEND_DST_ALPHA;
    else if (upper == "ONE_MINUS_DST_ALPHA")
        return RenderState::BLEND_ONE_MINUS_DST_ALPHA;
    else if (upper == "CONSTANT_ALPHA")
        return RenderState::BLEND_CONSTANT_ALPHA;
    else if (upper == "ONE_MINUS_CONSTANT_ALPHA")
        return RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA;
    else if (upper == "SRC_ALPHA_SATURATE")
        return RenderState::BLEND_SRC_ALPHA_SATURATE;
    else
    {
        GP_ERROR("Unsupported blend value (%s). (Will default to BLEND_ONE if errors are treated as warnings)", value);
        return RenderState::BLEND_ONE;
    }
}

void RenderState::StateBlock::setState(const char* name, const char* value)
{
    GP_ASSERT(name);

    if (strcmp(name, "blend") == 0)
    {
        setBlend(parseBoolean(value));
    }
    else if (strcmp(name, "blendSrc") == 0 || strcmp(name, "srcBlend") == 0 )   // Leaving srcBlend for backward compat.
    {
        setBlendSrc(parseBlend(value));
    }
    else if (strcmp(name, "blendDst") == 0 || strcmp(name, "dstBlend") == 0)    // // Leaving dstBlend for backward compat.
    {
        setBlendDst(parseBlend(value));
    }
    else if (strcmp(name, "cullFace") == 0)
    {
        setCullFace(parseBoolean(value));
    }
    else if (strcmp(name, "depthTest") == 0)
    {
        setDepthTest(parseBoolean(value));
    }
    else if (strcmp(name, "depthWrite") == 0)
    {
        setDepthWrite(parseBoolean(value));
    }
    else
    {
        GP_ERROR("Unsupported render state string '%s'.", name);
    }
}

void RenderState::StateBlock::setBlend(bool enabled)
{
    _blendEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_BLEND;
    }
    else
    {
        _bits |= RS_BLEND;
    }
}

void RenderState::StateBlock::setBlendSrc(Blend blend)
{
    _blendSrc = blend;
    if (_blendSrc == BLEND_ONE && _blendDst == BLEND_ZERO)
    {
        // Default blend func
        _bits &= ~RS_BLEND_FUNC;
    }
    else
    {
        _bits |= RS_BLEND_FUNC;
    }
}

void RenderState::StateBlock::setBlendDst(Blend blend)
{
    _blendDst = blend;
    if (_blendSrc == BLEND_ONE && _blendDst == BLEND_ZERO)
    {
        // Default blend func
        _bits &= ~RS_BLEND_FUNC;
    }
    else
    {
        _bits |= RS_BLEND_FUNC;
    }
}

void RenderState::StateBlock::setCullFace(bool enabled)
{
    _cullFaceEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_CULL_FACE;
    }
    else
    {
        _bits |= RS_CULL_FACE;
    }
}

void RenderState::StateBlock::setDepthTest(bool enabled)
{
    _depthTestEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_DEPTH_TEST;
    }
    else
    {
        _bits |= RS_DEPTH_TEST;
    }
}

void RenderState::StateBlock::setDepthWrite(bool enabled)
{
    _depthWriteEnabled = enabled;
    if (enabled)
    {
        _bits &= ~RS_DEPTH_WRITE;
    }
    else
    {
        _bits |= RS_DEPTH_WRITE;
    }
}

}
