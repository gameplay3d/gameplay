#include "Base.h"
#include "MaterialParameter.h"

namespace gameplay
{

MaterialParameter::MaterialParameter(void) :
    _type(0)
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
    write(_value, file);
    write(_type, file);
}
void MaterialParameter::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "%f ", "value", _value);
    fprintfElement(file, "type", _type);
    fprintElementEnd(file);
}

}
