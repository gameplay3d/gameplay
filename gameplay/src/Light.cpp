#include "Base.h"
#include "Light.h"

#define LIGHT_COLOR Vector3::one()
#define LIGHT_RANGE 10.0f
#define LIGHT_ANGLE 30.0f
#define LIGHT_INTENSITY 1.0f

namespace gameplay
{

Light::Light() : Component(),
    _type(Type::eDirectional),
    _color(LIGHT_COLOR),
    _intensity(LIGHT_INTENSITY),
    _range(LIGHT_RANGE),
    _angle(LIGHT_ANGLE),
    _angleCos(cos(LIGHT_ANGLE)),
    _lighting(Lighting::eRealtime),
    _shadows(Shadows::eNone)
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
        case Type::eDirectional:
        {

            break;
        }
        case Type::ePoint:
        {

            break;
        }

        case Type::eSpot:
        {

            break;
        }

        default:
            break;
    }
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
            case static_cast<int>(Type::eDirectional):
                return "eDirectional";
            case static_cast<int>(Type::ePoint):
                return "ePoint";
            case static_cast<int>(Type::eSpot):
                return "eSpot";
            default:
                return "eDirectional";
        }
    }
    else if(enumName.compare("gameplay::Light::Lighting") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Lighting::eRealtime):
                return "eRealtime";
            case static_cast<int>(Lighting::eBaked):
                return "eBaked";
            default:
                return "eRealtime";
        }
    }
    else if(enumName.compare("gameplay::Light::Shadows") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Shadows::eNone):
                return "eNone";
            case static_cast<int>(Shadows::eHard):
                return "eHard";
            case static_cast<int>(Shadows::eSoft):
                return "eSoft";
            default:
                return "eNone";
        }
    }
    return "";
}

int Light::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        if (str.compare("eDirectional") == 0)
            return static_cast<int>(Type::eDirectional);
        else if (str.compare("ePoint") == 0)
            return static_cast<int>(Type::ePoint);
        else if (str.compare("eSpot") == 0)
            return static_cast<int>(Type::eSpot);
    }
    else if (enumName.compare("gameplay::Light::Lighting") == 0)
    {
        if (str.compare("eRealtime") == 0)
            return static_cast<int>(Lighting::eRealtime);
        else if (str.compare("eBaked") == 0)
            return static_cast<int>(Lighting::eBaked);
    }
    else if (enumName.compare("gameplay::Light::Shadows") == 0)
    {
        if (str.compare("eNone") == 0)
            return static_cast<int>(Shadows::eNone);
        else if (str.compare("eHard") == 0)
            return static_cast<int>(Shadows::eHard);
        else if (str.compare("eSoft") == 0)
            return static_cast<int>(Shadows::eSoft);
    }
    return static_cast<int>(Shadows::eNone);
}

std::string Light::getClassName()
{
    return "gameplay::Light";
}

void Light::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Light::Type", static_cast<int>(_type), -1);
    serializer->writeColor("color", _color, LIGHT_COLOR);
    serializer->writeFloat("intensity", _intensity, LIGHT_INTENSITY);
    switch (_type)
    {
        case Type::ePoint:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            break;
        }

        case Type::eSpot:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            serializer->writeFloat("angle", _angle, LIGHT_ANGLE);\
            break;
        }

        default:
            break;
    }

    serializer->writeEnum("lighting", "gameplay::Light::Lighting", static_cast<int>(_lighting), static_cast<int>(Lighting::eRealtime));
    serializer->writeEnum("shadows", "gameplay::Light::Shadows", static_cast<int>(_shadows), static_cast<int>(Shadows::eNone));
}

void Light::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Light::Type>(serializer->readEnum("type", "gameplay::Light::Type", -1));
    _color = serializer->readColor("color", LIGHT_COLOR);
    _intensity = serializer->readFloat("intensity", LIGHT_INTENSITY);
    switch(_type)
    {
        case Type::ePoint:
        {
            _range = serializer->readFloat("range", LIGHT_RANGE);
            break;
        }

        case Type::eSpot:
        {
            _range = serializer->readFloat("range", LIGHT_RANGE);
            _angle = serializer->readFloat("angle", LIGHT_ANGLE);
            _angleCos = cos(_angle);
            break;
        }

        default:
            break;
    }
    _lighting = static_cast<Light::Lighting>(serializer->readEnum("lighting", "gameplay::Light::Lighting", static_cast<int>(Lighting::eRealtime)));
    _shadows = static_cast<Light::Shadows>(serializer->readEnum("shadows", "gameplay::Light::Shadows", static_cast<int>(Shadows::eNone)));
}

}
