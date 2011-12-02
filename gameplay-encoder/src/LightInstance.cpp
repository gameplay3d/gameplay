#include "Base.h"
#include "LightInstance.h"

namespace gameplay
{

LightInstance::LightInstance(void) : _ref(NULL)
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
    if (_ref != NULL)
    {
        _ref->writeBinary(file);
    }
}
void LightInstance::writeText(FILE* file)
{
    if (_ref != NULL)
    {
        _ref->writeText(file);
    }
}

Light* LightInstance::getLight() const
{
    return _ref;
}

void LightInstance::setLight(Light* light)
{
    _ref = light;
}

bool LightInstance::isAmbient() const
{
    return _ref != NULL && _ref->isAmbient();
}

}
