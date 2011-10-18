/**
 * Material.cpp
 */

#include "Base.h"
#include "Material.h"
//#include "MaterialParameter.h"

namespace gameplay
{

Material::Material() :
    _effect(NULL)
{
}

Material::Material(const Material& m)
{
}

Material::~Material()
{
    SAFE_RELEASE(_effect);

    // Destroy parameters.
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        MaterialParameter* p = _parameters[i];
        if (p)
        {
            delete p;
        }
    }
}

Material* Material::createMaterial(Effect* effect)
{
    assert(effect);

    effect->addRef();

    Material* m = new Material();
    m->_effect = effect;

    return m;
}

Material* Material::createMaterial(const char* vshPath, const char* fshPath, const char* defines)
{
    Effect* effect = Effect::createFromFile(vshPath, fshPath, defines);
    if (effect == NULL)
    {
        return NULL;
    }

    Material* m = new Material();
    m->_effect = effect;

    return m;
}

Effect* Material::getEffect()
{
    return _effect;
}

MaterialParameter* Material::getParameter(const char* name)
{
    if (name == NULL)
    {
        return NULL;
    }

    // Search for an existing Parameter with this name.
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        if (strcmp(name, _parameters[i]->_uniform->getName()) == 0)
        {
            return _parameters[i];
        }
    }

    // Look up the uniform with the specified name.
    Uniform* uniform = _effect->getUniform(name);
    if (uniform == NULL)
    {
        // No uniform found with this name.
        return NULL;
    }

    // Create a new parameter and store it in our list.
    MaterialParameter* p = new MaterialParameter(this, uniform);
    _parameters.push_back(p);

    return p;
}

void Material::bind()
{
    // Bind our effect.
    _effect->bind();

    // Bind all parameters in our list (order doesn't matter).
    for (unsigned int i = 0, count = _parameters.size(); i < count; ++i)
    {
        _parameters[i]->bind();
    }
}

}
