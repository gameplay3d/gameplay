#include "Base.h"
#include "Effect.h"

namespace gameplay
{

Effect::Effect(void)
{
}

Effect::~Effect(void)
{
}

unsigned int Effect::getTypeId(void) const
{
    return EFFECT_ID;
}

const char* Effect::getElementName(void) const
{
    return "Effect";
}

void Effect::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_vertexShader, file);
    write(_fragmentShader, file);
}

void Effect::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "vertexShader", _vertexShader);
    fprintfElement(file, "fragmentShader", _fragmentShader);
    fprintElementEnd(file);
}

}
