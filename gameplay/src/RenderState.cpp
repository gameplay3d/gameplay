/**
 * RenderState.cpp
 */

#include "Base.h"
#include "RenderState.h"
#include "Node.h"
#include "Pass.h"

// Render state override bits
#define RS_BLEND 1
#define RS_BLEND_FUNC 2
#define RS_CULL_FACE 4
#define RS_DEPTH_TEST 8
#define RS_DEPTH_WRITE 16

namespace gameplay
{

RenderState::RenderState()
    : _nodeBinding(NULL), _state(NULL), _restoreState(NULL)
{
}

RenderState::RenderState(const RenderState& copy)
{
    SAFE_DELETE(_state);
    SAFE_DELETE(_restoreState);
}

RenderState::~RenderState()
{
    // Destroy all the material parameters
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        MaterialParameter* parameter = _parameters[i];
        if (parameter)
        {
            SAFE_RELEASE(parameter);
        }
    }
}

MaterialParameter* RenderState::getParameter(const char* name) const
{
    assert(name);

    MaterialParameter* param;

    // Search for an existing parameter with this name
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        param = _parameters[i];
        if (strcmp(param->getName(), name) == 0)
        {
            return param;
        }
    }

    // Create a new parameter and store it in our list
    param = new MaterialParameter(name);
    _parameters.push_back(param);

    return param;
}

void RenderState::setParameterAutoBinding(const char* name, AutoBinding autoBinding)
{
    // Store the auto-binding
    if (autoBinding == NONE)
    {
        // Clear current auto binding
        std::map<std::string, AutoBinding>::iterator itr = _autoBindings.find(name);
        if (itr != _autoBindings.end())
        {
            _autoBindings.erase(itr);
        }
    }
    else
    {
        // Set new auto binding
        _autoBindings[name] = autoBinding;
    }

    // If we have a currently set node binding, apply the auto binding immediately
    if (_nodeBinding)
    {
        applyAutoBinding(name, autoBinding);
    }
}

void RenderState::setParameterAutoBinding(const char* name, const char* autoBinding)
{
    AutoBinding value = NONE;

    // Parse the passed in autoBinding string
    if (strcmp(autoBinding, "WORLD_MATRIX") == 0)
    {
        value = WORLD_MATRIX;
    }
    else if (strcmp(autoBinding, "VIEW_MATRIX") == 0)
    {
        value = VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "WORLD_VIEW_MATRIX") == 0)
    {
        value = WORLD_VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "WORLD_VIEW_PROJECTION_MATRIX") == 0)
    {
        value = WORLD_VIEW_PROJECTION_MATRIX;
    }
    else if (strcmp(autoBinding, "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX") == 0)
    {
        value = INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    }
    else if (strcmp(autoBinding, "CAMERA_POSITION") == 0)
    {
        value = CAMERA_POSITION;
    }
    else if (strcmp(autoBinding, "MATRIX_PALETTE") == 0)
    {
        value = MATRIX_PALETTE;
    }

    if (value != NONE)
    {
        setParameterAutoBinding(name, value);
    }
}

bool parseBoolean(const char* value)
{
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

RenderState::Blend parseBlend(const char* value)
{
    // Conver the string to uppercase for comparison
    std::string upper(value);
    std::transform(upper.begin(), upper.end(), upper.begin(), std::toupper);
    if (upper == "ZERO")
        return RenderState::BLEND_ZERO;
    if (upper == "ONE")
        return RenderState::BLEND_ONE;
    if (upper == "SRC_ALPHA")
        return RenderState::BLEND_SRC_ALPHA;
    if (upper == "ONE_MINUS_SRC_ALPHA")
        return RenderState::BLEND_ONE_MINUS_SRC_ALPHA;
    if (upper == "DST_ALPHA")
        return RenderState::BLEND_DST_ALPHA;
    if (upper == "ONE_MINUS_DST_ALPHA")
        return RenderState::BLEND_ONE_MINUS_DST_ALPHA;
    if (upper == "CONSTANT_ALPHA")
        return RenderState::BLEND_CONSTANT_ALPHA;
    if (upper == "ONE_MINUS_CONSTANT_ALPHA")
        return RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (upper == "SRC_ALPHA_SATURATE")
        return RenderState::BLEND_SRC_ALPHA_SATURATE;

    WARN_VARG("Warning: Unrecognized blend value (%s), defaulting to BLEND_ONE.", value);
    return RenderState::BLEND_ONE;
}

void RenderState::setRenderState(const char* name, const char* value)
{
    assert(name && value);

    if (strcmp(name, "blend") == 0)
    {
        setBlend(parseBoolean(value));
    }
    else if (strcmp(name, "srcBlend") == 0)
    {
        setSrcBlend(parseBlend(value));
    }
    else if (strcmp(name, "dstBlend") == 0)
    {
        setDstBlend(parseBlend(value));
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
        WARN_VARG("Warning: Invalid render state: %s", name);
    }
}

void RenderState::setBlend(bool enabled)
{
    createStateBlock();
    _state->blendEnabled = enabled;
    _state->bits |= RS_BLEND;
}

void RenderState::setSrcBlend(Blend blend)
{
    createStateBlock();
    _state->srcBlend = blend;
    _state->bits |= RS_BLEND_FUNC;
}

void RenderState::setDstBlend(Blend blend)
{
    createStateBlock();
    _state->dstBlend = blend;
    _state->bits |= RS_BLEND_FUNC;
}

void RenderState::setCullFace(bool enabled)
{
    createStateBlock();
    _state->cullFaceEnabled = enabled;
    _state->bits |= RS_CULL_FACE;
}

void RenderState::setDepthTest(bool enabled)
{
    createStateBlock();
    _state->depthTestEnabled = enabled;
    _state->bits |= RS_DEPTH_TEST;
}

void RenderState::setDepthWrite(bool enabled)
{
    createStateBlock();
    _state->depthWriteEnabled = enabled;
    _state->bits |= RS_DEPTH_WRITE;
}

void RenderState::setNodeBinding(Node* node)
{
    _nodeBinding = node;

    if (_nodeBinding)
    {
        // Apply all existing auto-bindings using this node
        std::map<std::string, AutoBinding>::const_iterator itr = _autoBindings.begin();
        while (itr != _autoBindings.end())
        {
            applyAutoBinding(itr->first.c_str(), itr->second);
            itr++;
        }
    }
}

void RenderState::applyAutoBinding(const char* uniformName, AutoBinding autoBinding)
{
    switch (autoBinding)
    {
    case WORLD_MATRIX:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getWorldMatrix);
        break;

    case VIEW_MATRIX:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getViewMatrix);
        break;

    case WORLD_VIEW_MATRIX:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getWorldViewMatrix);
        break;

    case WORLD_VIEW_PROJECTION_MATRIX:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getWorldViewProjectionMatrix);
        break;

    case INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getInverseTransposeWorldViewMatrix);
        break;

    case CAMERA_POSITION:
        getParameter(uniformName)->bindValue(_nodeBinding, &Node::getActiveCameraTranslation);
        break;

    case MATRIX_PALETTE:
        {
            Model* model = _nodeBinding->getModel();
            MeshSkin* skin = model ? model->getSkin() : NULL;
            if (skin)
            {
                getParameter(uniformName)->bindValue(skin, &MeshSkin::getMatrixPalette, &MeshSkin::getMatrixPaletteSize);
            }
        }
        break;
    }
}

void RenderState::bind(Pass* pass)
{
    // Apply all parameter bindings
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        _parameters[i]->bind(pass->getEffect());
    }

    if (_state)
    {
        GLint temp;

        // Backup old state and set new state.
        if (_state->bits & RS_BLEND)
        {
            _restoreState->blendEnabled = glIsEnabled(GL_BLEND);
            if (_restoreState->blendEnabled != _state->blendEnabled)
            {
                _state->blendEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
                _restoreState->bits |= RS_BLEND;
            }
        }
        if (_state->bits & RS_BLEND_FUNC)
        {
            glGetIntegerv(GL_BLEND_SRC_RGB, &temp);
            _restoreState->srcBlend = (Blend)temp;
            glGetIntegerv(GL_BLEND_DST_RGB, &temp);
            _restoreState->dstBlend = (Blend)temp;
            if (_restoreState->srcBlend != _state->srcBlend || _restoreState->dstBlend != _state->dstBlend)
            {
                glBlendFunc((GLenum)_state->srcBlend, (GLenum)_state->dstBlend);
                _restoreState->bits |= RS_BLEND_FUNC;
            }
        }
        if (_state->bits & RS_CULL_FACE)
        {
            _restoreState->cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
            if (_restoreState->cullFaceEnabled != _state->cullFaceEnabled)
            {
                _state->cullFaceEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
                _restoreState->bits |= RS_CULL_FACE;
            }
        }
        if (_state->bits & RS_DEPTH_TEST)
        {
            _restoreState->depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
            if (_restoreState->depthTestEnabled != _state->depthTestEnabled)
            {
                _state->depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
                _restoreState->bits |= RS_DEPTH_TEST;
            }
        }
        if (_state->bits & RS_DEPTH_WRITE)
        {
            glGetIntegerv(GL_DEPTH_WRITEMASK, &temp);
            _restoreState->depthWriteEnabled = (unsigned char)temp;
            if (_restoreState->depthWriteEnabled != _state->depthWriteEnabled)
            {
                glDepthMask(_state->depthWriteEnabled);
                _restoreState->bits |= RS_DEPTH_WRITE;
            }
        }
    }
}

void RenderState::unbind(Pass* pass)
{
    if (_restoreState && _restoreState->bits)
    {
        // Restore render state to what it was before binding, unless
        // _restoreState->bits has marked that we have nothing to restore
        if (_restoreState->bits & RS_BLEND)
        {
            _restoreState->blendEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        }
        if (_restoreState->bits & RS_BLEND_FUNC)
        {
            glBlendFunc((GLenum)_restoreState->srcBlend, (GLenum)_restoreState->dstBlend);
        }
        if (_restoreState->bits & RS_CULL_FACE)
        {
            _restoreState->cullFaceEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        }
        if (_restoreState->bits & RS_DEPTH_TEST)
        {
            _restoreState->depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        }
        if ((_restoreState->bits & RS_DEPTH_WRITE))
        {
            glDepthMask(_restoreState->depthWriteEnabled);
        }

        // Rest restore state bits
        _restoreState->bits = 0;
    }
}

void RenderState::createStateBlock()
{
    if (!_state)
    {
        _state = new StateBlock();
        _restoreState = new StateBlock();
    }
}

RenderState::StateBlock::StateBlock()
    : bits(0L), blendEnabled(false), cullFaceEnabled(false), depthTestEnabled(false), depthWriteEnabled(false),
    srcBlend(RenderState::BLEND_ONE), dstBlend(RenderState::BLEND_ONE)
{
}

}
