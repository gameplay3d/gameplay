/*
 * Light.cpp
 */

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

Light::Light(Light::Type type, const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation) :
    _type(type), _node(NULL)
{
    _point = new Point(color, constantAttenuation, linearAttenuation, quadraticAttenuation);
}

Light::Light(Light::Type type, const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent) :
    _type(type), _node(NULL)
{
    _spot = new Spot(color, constantAttenuation, linearAttenuation, quadraticAttenuation, falloffAngle, falloffExponent);
}

Light::~Light()
{
    switch (_type)
    {
    case DIRECTIONAL:
        if (_directional)
        {
            delete _directional;
            _directional = NULL;
        }
        break;
    case POINT:
        if (_point)
        {
            delete _point;
            _point = NULL;
        }
        break;
    case SPOT:
        if (_spot)
        {
            delete _spot;
            _spot = NULL;
        }
        break;
    }
}

Light* Light::createDirectional(const Vector3& color)
{
    return new Light(DIRECTIONAL, color);
}

Light* Light::createPoint(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
    return new Light(POINT, color, constantAttenuation, linearAttenuation, quadraticAttenuation);
}

Light* Light::createSpot(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent)
{
    return new Light(SPOT, color, constantAttenuation, linearAttenuation, quadraticAttenuation, falloffAngle, falloffExponent);
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
    if (_node != node)
    {
        // Disconnect our current node.
        SAFE_RELEASE(_node);

        // Connect the new node.
        _node = node;

        if (_node)
        {
            _node->addRef();
        }
    }
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

Light::Directional::Directional(const Vector3& color)
    : color(color)
{
}

Light::Point::Point(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
    : color(color), constantAttenuation(constantAttenuation), linearAttenuation(linearAttenuation), quadraticAttenuation(quadraticAttenuation)
{
}

Light::Spot::Spot(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent)
    : color(color), constantAttenuation(constantAttenuation), linearAttenuation(linearAttenuation), quadraticAttenuation(quadraticAttenuation),
      falloffAngle(falloffAngle), falloffExponent(falloffExponent)
{
}

}
