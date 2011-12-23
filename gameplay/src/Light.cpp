#include "Base.h"
#include "Light.h"
#include "Node.h"

namespace gameplay
{

Light::Light(Light::Type type, const Vector3& color) :
    _type(type), _node(NULL)
{
    _directional = new Directional(color);
}

Light::Light(Light::Type type, const Vector3& color, float range) :
    _type(type), _node(NULL)
{
    _point = new Point(color, range);
}

Light::Light(Light::Type type, const Vector3& color, float range, float innerAngle, float outerAngle) :
    _type(type), _node(NULL)
{
    _spot = new Spot(color, range, innerAngle, outerAngle);
}

Light::~Light()
{
    switch (_type)
    {
    case DIRECTIONAL:
        SAFE_DELETE(_directional);
        break;
    case POINT:
        SAFE_DELETE(_point);
        break;
    case SPOT:
        SAFE_DELETE(_spot);
        break;
    }
}

Light* Light::createDirectional(const Vector3& color)
{
    return new Light(DIRECTIONAL, color);
}

Light* Light::createPoint(const Vector3& color, float range)
{
    return new Light(POINT, color, range);
}

Light* Light::createSpot(const Vector3& color, float range, float innerAngle, float outerAngle)
{
    return new Light(SPOT, color, range, innerAngle, outerAngle);
}

Light::Type Light::getLightType() const
{
    return _type;
}

Node* Light::getNode() const
{
    return _node;
}

void Light::setNode(Node* node)
{
    // Connect the new node.
    _node = node;
}

const Vector3& Light::getColor() const
{
    switch (_type)
    {
    case DIRECTIONAL:
        return _directional->color;
    case POINT:
        return _point->color;
    case SPOT:
        return _spot->color;
    default:
        assert(0);
        return Vector3::zero();

    }
}

void Light::setColor(const Vector3& color)
{
    switch (_type)
    {
    case DIRECTIONAL:
        _directional->color = color;
        break;
    case POINT:
        _point->color = color;
        break;
    case SPOT:
        _spot->color = color;
        break;
    }
}

float Light::getRange()  const
{
    assert(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        return _point->range;
    case SPOT:
        return _spot->range;
    default:
        assert(0);
        return 0.0f;
    }
}
    
void Light::setRange(float range)
{
    assert(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        _point->range = range;
        _point->rangeInverse = 1.0f / range;
        break;
    case SPOT:
        _spot->range = range;
        _spot->rangeInverse = 1.0f / range;
        break;
    }
}

float Light::getRangeInverse() const
{
    assert(_type != DIRECTIONAL);

    switch (_type)
    {
    case POINT:
        return _point->rangeInverse;
    case SPOT:
        return _spot->rangeInverse;
    default:
        assert(0);
        return 0.0f;
    }
}
    
float Light::getInnerAngle()  const
{
    assert(_type == SPOT);

    return _spot->innerAngle;
}

void Light::setInnerAngle(float innerAngle)
{
    assert(_type == SPOT);

    _spot->innerAngle = innerAngle;
    _spot->innerAngleCos = cos(innerAngle);
}
    
float Light::getOuterAngle()  const
{
    assert(_type == SPOT);

    return _spot->outerAngle;
}

void Light::setOuterAngle(float outerAngle)
{
    assert(_type == SPOT);

    _spot->outerAngle = outerAngle;
    _spot->outerAngleCos = cos(outerAngle);
}
    
float Light::getInnerAngleCos()  const
{
    assert(_type == SPOT);

    return _spot->innerAngleCos;
}
    
float Light::getOuterAngleCos()  const
{
    assert(_type == SPOT);

    return _spot->outerAngleCos;
}

Light::Directional::Directional(const Vector3& color)
    : color(color)
{
}

Light::Point::Point(const Vector3& color, float range)
    : color(color), range(range)
{
    rangeInverse = 1.0f / range;
}

Light::Spot::Spot(const Vector3& color, float range, float innerAngle, float outerAngle)
    : color(color), range(range), innerAngle(innerAngle), outerAngle(outerAngle)
{
    rangeInverse = 1.0f / range;
    innerAngleCos = cos(innerAngle);
    outerAngleCos = cos(outerAngle);
}

}
