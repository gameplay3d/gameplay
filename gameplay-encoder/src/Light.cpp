#include "Light.h"

namespace gameplay
{

Light::Light(void) :
    lightType(0),
    constantAttenuation(0.0f),
    linearAttenuation(0.0f),
    quadraticAttenuation(0.0f),
    falloffAngle(0.0f),
    falloffExponent(0.0f)
{
    fillArray(color, 0.0f, COLOR_SIZE);
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
    write(lightType, file);
    write(color, COLOR_SIZE, file);

    if (lightType == SpotLight)
    {
        write(constantAttenuation, file);
        write(linearAttenuation, file);
        write(quadraticAttenuation, file);
        write(falloffAngle, file);
        write(falloffExponent, file);
    }
    else if (lightType == PointLight)
    {
        write(constantAttenuation, file);
        write(linearAttenuation, file);
        write(quadraticAttenuation, file);
    }

}
void Light::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "lightType", lightType);
    fprintfElement(file, "color", color, COLOR_SIZE);

    if (lightType == SpotLight)
    {
        fprintfElement(file, "constantAttenuation", constantAttenuation);
        fprintfElement(file, "linearAttenuation", linearAttenuation);
        fprintfElement(file, "quadraticAttenuation", quadraticAttenuation);
        fprintfElement(file, "falloffAngle", falloffAngle);
        fprintfElement(file, "falloffExponent", falloffExponent);
    }
    else if (lightType == PointLight)
    {
        fprintfElement(file, "constantAttenuation", constantAttenuation);
        fprintfElement(file, "linearAttenuation", linearAttenuation);
        fprintfElement(file, "quadraticAttenuation", quadraticAttenuation);
    }
    fprintElementEnd(file);
}

float Light::getRed() const
{
    return color[0];
}
float Light::getGreen() const
{
    return color[1];
}
float Light::getBlue() const
{
    return color[2];
}

bool Light::isAmbient() const
{
    return lightType == AmbientLight;
}

void Light::setAmbientLight()
{
    lightType = AmbientLight;
}
void Light::setDirectionalLight()
{
    lightType = DirectionalLight;
}
void Light::setPointLight()
{
    lightType = PointLight;
}
void Light::setSpotLight()
{
    lightType = SpotLight;
}

void Light::setColor(float r, float g, float b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Light::setConstantAttenuation(float value)
{
    constantAttenuation = value;
}
void Light::setLinearAttenuation(float value)
{
    linearAttenuation = value;
}
void Light::setQuadraticAttenuation(float value)
{
    quadraticAttenuation = value;
}
void Light::setFalloffAngle(float value)
{
    falloffAngle = value;
}
void Light::setFalloffExponent(float value)
{
    falloffExponent = value;
}

}