#include "Base.h"
#include "Material.h"

namespace gameplay
{

Material::Material(void) :
    _effect(NULL)
{
}

Material::~Material(void)
{
}

unsigned int Material::getTypeId(void) const
{
    return MATERIAL_ID;
}
const char* Material::getElementName(void) const
{
    return "Material";
}

void Material::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    //write(_parameters, file);
    //write(_effect, file);
}
void Material::writeText(FILE* file)
{
    fprintElementStart(file);
    //fprintfElement(file, "parameters", _parameters);
    //fprintfElement(file, "effect", _effect);
    fprintElementEnd(file);
}

}
