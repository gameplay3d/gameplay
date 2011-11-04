/**
 * Technique.cpp
 */

#include "Base.h"
#include "Technique.h"
#include "Material.h"

namespace gameplay
{

Technique::Technique(const char* id, Material* material)
	: _id(id ? id : ""), _material(material)
{
}

Technique::Technique(const Technique& m)
{
}

Technique::~Technique()
{
	// Destroy all the passes.
    for (unsigned int i = 0, count = _passes.size(); i < count; ++i)
    {
        SAFE_RELEASE(_passes[i]);
    }
}

const char* Technique::getId() const
{
    return _id.c_str();
}

unsigned int Technique::getPassCount() const
{
    return _passes.size();
}

Pass* Technique::getPass(unsigned int index) const
{
    assert(index < _passes.size());

    return _passes[index];
}

Pass* Technique::getPass(const char* id) const
{
    for (unsigned int i = 0, count = _passes.size(); i < count; ++i)
    {
        Pass* pass = _passes[i];
        if (strcmp(pass->getId(), id) == 0)
        {
            return pass;
        }
    }

    return NULL;
}

}
