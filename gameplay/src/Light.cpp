#include "Base.h"
#include "Light.h"

#define LIGHT_COLOR Vector3::one()
#define LIGHT_RANGE 10.0f
#define LIGHT_INNER_ANGLE 30.0f
#define LIGHT_OUTER_ANGLE 45.0f

namespace gameplay
{
Light::Light() : Component(),
    _type(TYPE_DIRECTIONAL),
    _color(LIGHT_COLOR),
    _range(LIGHT_RANGE),
    _innerAngle(LIGHT_INNER_ANGLE),
    _innerAngleCos(cos(LIGHT_INNER_ANGLE)),
    _outerAngle(LIGHT_OUTER_ANGLE),
    _outerAngleCos(cos(LIGHT_OUTER_ANGLE))
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

float Light::getRange()  const
{
    return _range;
}
    
void Light::setRange(float range)
{
    _range = range;
}

float Light::getRangeInverse() const
{
    return _rangeInverse;
}

void Light::setRangeInverse(float rangeInverse)
{
    _rangeInverse = rangeInverse;
}
    
float Light::getInnerAngle()  const
{
    return _innerAngle;
}

void Light::setInnerAngle(float innerAngle)
{
    _innerAngle = innerAngle;
    _innerAngleCos = cos(innerAngle);
}
    
float Light::getOuterAngle()  const
{
    return _outerAngle;
}

void Light::setOuterAngle(float outerAngle)
{
    _outerAngle = outerAngle;
    _outerAngleCos = cos(outerAngle);
}
    
float Light::getInnerAngleCos()  const
{
    return _innerAngleCos;
}
    
float Light::getOuterAngleCos()  const
{
    return _outerAngleCos;
}

std::string Light::getClassName()
{
    return "gameplay::Light";
}

void Light::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Light::Type", _type, -1);
    serializer->writeColor("color", _color, LIGHT_COLOR);
    switch(_type)
    {
        case TYPE_POINT:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            break;
        }
        case TYPE_SPOT:
        {
            serializer->writeFloat("range", _range, LIGHT_RANGE);
            serializer->writeFloat("innerAngle", _innerAngle, LIGHT_INNER_ANGLE);
            serializer->writeFloat("outerAngle", _outerAngle, LIGHT_OUTER_ANGLE);
            break;
        }
        default:
            break;
    }
}

void Light::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Light::Type>(serializer->readEnum("type", "gameplay::Light::Type", -1));
    Vector3 color = serializer->readColor("color", LIGHT_COLOR);
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
            _innerAngle = serializer->readFloat("innerAngle", LIGHT_INNER_ANGLE);
            _innerAngleCos = cos(_innerAngle);
            _outerAngle = serializer->readFloat("outerAngle", LIGHT_OUTER_ANGLE);
            _outerAngleCos = cos(_outerAngle);
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
            case Light::TYPE_DIRECTIONAL:
                return "TYPE_DIRECTIONAL";
            case Light::TYPE_POINT:
                return "TYPE_POINT";
            case Light::TYPE_SPOT:
                return "TYPE_SPOT";
            default:
                break;
        }
    }
    return "TYPE_DIRECTIONAL";
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
    return Light::TYPE_DIRECTIONAL;
}

}
