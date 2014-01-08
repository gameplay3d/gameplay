#include "Base.h"
#include "Light.h"

namespace gameplay
{

Light::Light(void) :
    _lightType(0),
    _range(1.0f),
    _innerAngle(-1.0f),
    _outerAngle(0.0f)
{
    fillArray(_color, 0.0f, COLOR_SIZE);
}

Light::~Light(void)
{
}

unsigned int Light::getTypeId(void) const
{
    return LIGHT_ID;
}
const char* Light::getElementName(void) const
{
    return "Light";
}

void Light::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_lightType, file);
    write(_color, COLOR_SIZE, file);

    if (_lightType == SpotLight)
    {
        write(_range, file);
        write(_innerAngle, file);
        write(_outerAngle, file);
    }
    else if (_lightType == PointLight)
    {
        write(_range, file);
    }
}

void Light::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "lightType", _lightType);
    fprintfElement(file, "color", _color, COLOR_SIZE);

    if (_lightType == SpotLight)
    {
        fprintfElement(file, "range", _range);
        fprintfElement(file, "innerAngle", MATH_DEG_TO_RAD(_innerAngle));
        fprintfElement(file, "outerAngle", MATH_DEG_TO_RAD(_outerAngle));
    }
    else if (_lightType == PointLight)
    {
        fprintfElement(file, "range", _range);
    }

    fprintElementEnd(file);
}

float Light::getRed() const
{
    return _color[0];
}
float Light::getGreen() const
{
    return _color[1];
}
float Light::getBlue() const
{
    return _color[2];
}

bool Light::isAmbient() const
{
    return _lightType == AmbientLight;
}

void Light::setAmbientLight()
{
    _lightType = AmbientLight;
}
void Light::setDirectionalLight()
{
    _lightType = DirectionalLight;
}
void Light::setPointLight()
{
    _lightType = PointLight;
}
void Light::setSpotLight()
{
    _lightType = SpotLight;
}

void Light::setColor(float r, float g, float b)
{
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
}
    
void Light::setRange(float value)
{
    _range = value;
}
void Light::setInnerAngle(float value)
{
    _innerAngle = value;
}
void Light::setOuterAngle(float value)
{
    _outerAngle = value;
}

}
