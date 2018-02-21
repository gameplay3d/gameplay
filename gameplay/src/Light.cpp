#include "Base.h"
#include "Light.h"

#define LIGHT_COLOR Vector3::one()
#define LIGHT_RANGE 10.0f
#define LIGHT_ANGLE 30.0f
#define LIGHT_INTENSITY 1.0f

namespace gameplay
{

Light::Light() : Component(),
    _type(Light::TYPE_DIRECTIONAL),
    _color(LIGHT_COLOR),
    _intensity(1.0f),
    _range(LIGHT_RANGE),
    _angle(LIGHT_ANGLE),
    _angleCos(cos(LIGHT_ANGLE)),
    _lighting(LIGHTING_REALTIME),
    _shadows(SHADOWS_NONE)
{
}

Light::~Light()
{
}

Light::Type Light::getType() const
{
    return _type;
}

void Light::setType(Light::Type type)
{
    _type = type;
}

const Vector3& Light::getColor() const
{
    return _color;
}

void Light::setColor(const Vector3& color)
{
    _color = color;
}

void Light::setIntensity(float intensity)
{
    _intensity = intensity;
}

float Light::getIntensity() const
{
    return _intensity;
}

float Light::getRange()  const
{
    return _range;
}
    
void Light::setRange(float range)
{
    _range = range;
}
    
float Light::getAngle()  const
{
    return _angle;
}

void Light::setAngle(float angle)
{
    _angle = angle;
    _angleCos = cos(angle);
}

float Light::getAngleCos()  const
{
    return _angleCos;
}

Light::Lighting Light::getLighting() const
{
    return _lighting;
}

void Light::setLighting(Light::Lighting lighting)
{
    _lighting = lighting;
}

void Light::setShadows(Light::Shadows shadows)
{
    _shadows = shadows;
}

Light::Shadows Light::getShadows() const
{
    return _shadows;
}

void Light::reset(Light::Type type)
{
    switch (_type)
    {
        case Light::TYPE_DIRECTIONAL:
        {

            break;
        }
        case Light::TYPE_POINT:
        {

            break;
        }

        case Light::TYPE_SPOT:
        {

            break;
        }

        default:
            break;
    }
}

Component::TypeId Light::getTypeId()
{
    return Component::TYPEID_LIGHT;
}

std::string Light::getClassName()
{
    return "gameplay::Light";
}

void Light::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Light::Type", _type, -1);
    serializer->writeColor("color", _color, LIGHT_COLOR);
    serializer->writeFloat("intensity", _intensity, LIGHT_INTENSITY);
    switch (_type)
    {
        case TYPE_POINT:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            break;
        }

        case TYPE_SPOT:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            serializer->writeFloat("angle", _angle, LIGHT_ANGLE);\
            break;
        }

        default:
            break;
    }

    serializer->writeEnum("lighting", "gameplay::Light::Lighting", _lighting, Light::LIGHTING_REALTIME);
    serializer->writeEnum("shadows", "gameplay::Light::Shadows", _shadows, Light::SHADOWS_NONE);
}

void Light::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Light::Type>(serializer->readEnum("type", "gameplay::Light::Type", -1));
    _color = serializer->readColor("color", LIGHT_COLOR);
    _intensity = serializer->readFloat("intensity", LIGHT_INTENSITY);
    switch(_type)
    {
        case Light::TYPE_POINT:
        {
            _range = serializer->readFloat("range", LIGHT_RANGE);
            break;
        }

        case Light::TYPE_SPOT:
        {
            _range = serializer->readFloat("range", LIGHT_RANGE);
            _angle = serializer->readFloat("angle", LIGHT_ANGLE);
            _angleCos = cos(_angle);
            break;
        }

        default:
            break;
    }
    _lighting = static_cast<Light::Lighting>(serializer->readEnum("lighting", "gameplay::Light::Lighting", Light::LIGHTING_REALTIME));
    _shadows = static_cast<Light::Shadows>(serializer->readEnum("shadows", "gameplay::Light::Shadows", Light::SHADOWS_NONE));
}

std::shared_ptr<Serializable> Light::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Light>());
}

std::string Light::enumToString(const std::string& enumName, int value)
{
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        switch (value)
        {
            case Light::TYPE_DIRECTIONAL:
                return "TYPE_DIRECTIONAL";
            case Light::TYPE_POINT:
                return "TYPE_POINT";
            case Light::TYPE_SPOT:
                return "TYPE_SPOT";
            default:
                return "TYPE_DIRECTIONAL";
        }
    }
    else if(enumName.compare("gameplay::Light::Lighting") == 0)
    {
        switch (value)
        {
            case Light::LIGHTING_REALTIME:
                return "LIGHTING_REALTIME";
            case Light::LIGHTING_BAKED:
                return "LIGHTING_BAKED";
            default:
                return "LIGHTING_REALTIME";
        }
    }
    else if(enumName.compare("gameplay::Light::Shadows") == 0)
    {
        switch (value)
        {
            case Light::SHADOWS_NONE:
                return "SHADOWS_NONE";
            case Light::SHADOWS_HARD:
                return "SHADOWS_HARD";
            case Light::SHADOWS_SOFT:
                return "SHADOWS_SOFT";
            default:
                return "SHADOWS_NONE";
        }
    }
    return "";
}

int Light::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        if (str.compare("TYPE_DIRECTIONAL") == 0)
            return Light::TYPE_DIRECTIONAL;
        else if (str.compare("TYPE_POINT") == 0)
            return Light::TYPE_POINT;
        else if (str.compare("TYPE_SPOT") == 0)
            return Light::TYPE_SPOT;
    }
    else if (enumName.compare("gameplay::Light::Lighting") == 0)
    {
        if (str.compare("LIGHTING_REALTIME") == 0)
            return Light::LIGHTING_REALTIME;
        else if (str.compare("LIGHTING_BAKED") == 0)
            return Light::LIGHTING_BAKED;
    }
    else if (enumName.compare("gameplay::Light::Shadows") == 0)
    {
        if (str.compare("SHADOWS_NONE") == 0)
            return Light::SHADOWS_NONE;
        else if (str.compare("SHADOWS_HARD") == 0)
            return Light::SHADOWS_HARD;
        else if (str.compare("SHADOWS_SOFT") == 0)
            return Light::SHADOWS_SOFT;
    }
    return -1;
}

}
