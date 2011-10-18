#include "LightInstance.h"
#include "assert.h"

namespace gameplay
{

LightInstance::LightInstance(void) : ref(NULL)
{

}

LightInstance::~LightInstance(void)
{
}

unsigned int LightInstance::getTypeId(void) const
{
    return LIGHTINSTANCE_ID;
}
const char* LightInstance::getElementName(void) const
{
    return "LightInstance";
}

void LightInstance::writeBinary(FILE* file)
{
    //assert(ref != NULL);
    if (ref != NULL)
        ref->writeBinary(file);
}
void LightInstance::writeText(FILE* file)
{
    if (ref != NULL)
        ref->writeText(file);
}

Light* LightInstance::getLight() const
{
    return ref;
}

void LightInstance::setLight(Light* light)
{
    ref = light;
}

bool LightInstance::isAmbient() const
{
    return ref != NULL && ref->isAmbient();
}

}