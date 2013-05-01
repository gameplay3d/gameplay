#include "Base.h"
#include "Pass.h"
#include "Technique.h"
#include "Material.h"
#include "Node.h"

namespace gameplay
{

Pass::Pass(const char* id, Technique* technique, Effect* effect) :
    _id(id ? id : ""), _technique(technique), _effect(effect), _vaBinding(NULL)
{
    RenderState::_parent = _technique;
}

Pass::~Pass()
{
    SAFE_RELEASE(_effect);
    SAFE_RELEASE(_vaBinding);
}

Pass* Pass::create(const char* id, Technique* technique, const char* vshPath, const char* fshPath, const char* defines)
{
    // Attempt to create/load the effect.
    Effect* effect = Effect::createFromFile(vshPath, fshPath, defines);
    if (effect == NULL)
    {
        GP_ERROR("Failed to create effect for pass.");
        return NULL;
    }

    // Return the new pass.
    return new Pass(id, technique, effect);
}

const char* Pass::getId() const
{
    return _id.c_str();
}

Effect* Pass::getEffect() const
{
    return _effect;
}

void Pass::setVertexAttributeBinding(VertexAttributeBinding* binding)
{
    SAFE_RELEASE(_vaBinding);

    if (binding)
    {
        _vaBinding = binding;
        binding->addRef();
    }
}

VertexAttributeBinding* Pass::getVertexAttributeBinding() const
{
    return _vaBinding;
}

void Pass::bind()
{
    GP_ASSERT(_effect);

    // Bind our effect.
    _effect->bind();

    // Bind our render state
    RenderState::bind(this);

    // If we have a vertex attribute binding, bind it
    if (_vaBinding)
    {
        _vaBinding->bind();
    }
}

void Pass::unbind()
{
    // If we have a vertex attribute binding, unbind it
    if (_vaBinding)
    {
        _vaBinding->unbind();
    }
}

Pass* Pass::clone(Technique* technique, NodeCloneContext &context) const
{
    Effect* effect = getEffect();
    GP_ASSERT(effect);
    effect->addRef();
    Pass* pass = new Pass(getId(), technique, effect);
    RenderState::cloneInto(pass, context);
    pass->_parent = technique;
    return pass;
}

}
