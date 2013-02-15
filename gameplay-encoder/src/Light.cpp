#include "Base.h"
#include "Light.h"

namespace gameplay
{

Light::Light(void) :
    _lightType(0),
    _constantAttenuation(0.0f),
    _linearAttenuation(0.0f),
    _quadraticAttenuation(0.0f),
    _falloffAngle(0.0f),
    _falloffExponent(0.0f),
    _range(-1.0f),
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

float Light::computeRange(float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
    if (constantAttenuation == 0.0f && linearAttenuation == 0.0f && quadraticAttenuation == 0.0f)
    {
        return 0.0f;
    }

    // Constant Attenuation is currently not supported.
    if (constantAttenuation == 1.0f)
    {
        return 1.0f;
    }

    const float step = 0.01f;
    float range = 0.01f;
    float att = 1.0f;
    while (att > 0.01f)
    {
        att = 1 / (constantAttenuation + (range * linearAttenuation) + (range * range * quadraticAttenuation));
        range += step;
    }
    return range;
}

float lerpstep( float lower, float upper, float s)
{
    float lerpstep = ( s - lower ) / ( upper - lower );
    
    if (lerpstep < 0.0f)
        lerpstep = 0.0f;
    else if (lerpstep > 1.0f)
        lerpstep = 1.0f;
    
    return lerpstep;
}

float Light::computeInnerAngle(float outerAngle)
{
    const float epsilon = 0.15f;

    // Set inner angle to half of outer angle.
    float innerAngle = outerAngle / 2.0f;

    // Try to find the inner angle by sampling the attenuation with steps of angle.
    for (float angle = 0.0; angle < outerAngle; angle += epsilon)
    {
        float outerCosAngle = cos(MATH_DEG_TO_RAD(outerAngle));
        float innerCosAngle = cos(MATH_DEG_TO_RAD(innerAngle));
            
        float cosAngle = cos(MATH_DEG_TO_RAD(angle));
        float att = lerpstep(outerCosAngle, innerCosAngle, cosAngle);
        
        if (att < 1.0f)
        {
            innerAngle = angle;
            break;
        }
    }

    return innerAngle;
}

void Light::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_lightType, file);
    write(_color, COLOR_SIZE, file);

    // Compute an approximate light range with Collada's attenuation parameters.
    // This facilitates bringing in the light nodes directly from maya to gameplay.
    if (_range == -1.0f)
    {
        _range = computeRange(_constantAttenuation, _linearAttenuation, _quadraticAttenuation);
    }

    if (_lightType == SpotLight)
    {
        // Compute an approximate inner angle of the spot light using Collada's outer angle.
        _outerAngle = _falloffAngle / 2.0f;
        
        if (_innerAngle == -1.0f)
        {
            _innerAngle = computeInnerAngle(_outerAngle);
        }

        write(_range, file);
        write(MATH_DEG_TO_RAD(_innerAngle), file);
        write(MATH_DEG_TO_RAD(_outerAngle), file);
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

    // Compute an approximate light range with Collada's attenuation parameters.
    // This facilitates bringing in the light nodes directly from maya to gameplay.
    if (_range == -1.0f)
    {
        _range = computeRange(_constantAttenuation, _linearAttenuation, _quadraticAttenuation);
    }

    if (_lightType == SpotLight)
    {
        // Compute an approximate inner angle of the spot light using Collada's outer angle.
        _outerAngle = _falloffAngle / 2.0f;
        if (_innerAngle == -1.0f)
        {
            _innerAngle = computeInnerAngle(_outerAngle);
        }

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

void Light::setConstantAttenuation(float value)
{
    _constantAttenuation = value;
}
void Light::setLinearAttenuation(float value)
{
    _linearAttenuation = value;
}
void Light::setQuadraticAttenuation(float value)
{
    _quadraticAttenuation = value;
}
void Light::setFalloffAngle(float value)
{
    _falloffAngle = value;
}
void Light::setFalloffExponent(float value)
{
    _falloffExponent = value;
    if ( value != 1.0)
    {
        LOG(1, "Warning: spot light falloff_exponent must be 1.0. \n");
    }
}

}
