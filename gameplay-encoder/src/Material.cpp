#include "Material.h"

namespace gameplay
{

Material::Material(void) :
    effect(NULL)
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
    //write(parameters, file);
    //write(effect, file);
}
void Material::writeText(FILE* file)
{
    fprintElementStart(file);
    //fprintfElement(file, "parameters", parameters);
    //fprintfElement(file, "effect", effect);
    fprintElementEnd(file);
}

}