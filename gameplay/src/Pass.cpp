#include "Base.h"
#include "Pass.h"
#include "Technique.h"
#include "Material.h"

namespace gameplay
{

Pass::Pass(const char* id, Technique* technique, Effect* effect) :
    _id(id ? id : ""), _technique(technique), _effect(effect), _vaBinding(NULL)
{
    assert(technique);

    RenderState::_parent = _technique;
}

Pass::Pass(const Pass& copy)
{
}

Pass::~Pass()
{
    SAFE_RELEASE(_effect);
    SAFE_RELEASE(_vaBinding);
}

Pass* Pass::create(const char* id, Technique* technique, const char* vshPath, const char* fshPath, const char* defines)
{
    // Attempt to create/load the effect
    Effect* effect = Effect::createFromFile(vshPath, fshPath, defines);
    assert(effect);
    if (effect == NULL)
    {
        return NULL;
    }

    // Return the new pass
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

void Pass::bind()
{
    // Bind our effect
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

}
