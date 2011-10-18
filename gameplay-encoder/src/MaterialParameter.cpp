#include "MaterialParameter.h"

namespace gameplay
{

MaterialParameter::MaterialParameter(void) :
    type(0)
{
}

MaterialParameter::~MaterialParameter(void)
{
}

unsigned int MaterialParameter::getTypeId(void) const
{
    return MATERIAL_ID;
}
const char* MaterialParameter::getElementName(void) const
{
    return "MaterialParameter";
}

void MaterialParameter::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(value, file);
    write(type, file);
}
void MaterialParameter::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "%f ", "value", value);
    fprintfElement(file, "type", type);
    fprintElementEnd(file);
}

}