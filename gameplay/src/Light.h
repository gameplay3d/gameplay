/*
 * Light.h
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Ref.h"
#include "Vector3.h"

namespace gameplay
{

class Node;

/**
 * Defines a light.
 *
 */
class Light : public Ref
{
    friend class Node;

public:

    /**
     * Defines the supported light types.
     */
    enum Type
    {
        DIRECTIONAL = 1,
        POINT = 2,
        SPOT = 3
    };


    /**
     * Creates a directional light.
     * 
     * @param color The light's color.
     * 
     * @return The new directional light.
     */
    static Light* createDirectional(const Vector3& color);

    /**
     * Creates a point light.
     * 
     * @param color The light's color.
     * @param constantAttenuation The light's constant attenuation.
     * @param linearAttenuation The light's linear attenuation.
     * @param quadraticAttenuation The light's quadratic attenuation.
     * 
     * @return The new point light.
     */
    static Light* createPoint(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation);

    /**
     * Creates a spot light.
     * 
     * @param color The light's color.
     * @param constantAttenuation The light's constant attenuation.
     * @param linearAttenuation The light's linear attenuation.
     * @param quadraticAttenuation The light's quadratic attenuation.
     * @param falloffAngle The light's falloff angle.
     * @param falloffExponent The light's falloff exponent.
     * 
     * @return The new spot light.
     */
    static Light* createSpot(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent);

    /**
     * Destructor.
     */
    virtual ~Light();

    /**
     * Returns the light type.
     * 
     * @return The light type.
     */
    Type getLightType() const;

    /**
     * Gets the light color.
     * 
     * @return The light color.
     */
    const Vector3& getColor() const;

    /**
     * Sets the light color.
     * 
     * @param color The light color to set.
     */
    void setColor(const Vector3& color);

    /**
     * Returns the node associated with this light.
     * 
     * @return The node associated with this light.
     */
    Node* getNode() const;

private:

    /**
     * Directional light data.
     */
    class Directional
    {
    public:
        Vector3 color;

        Directional(const Vector3& color);
    };

    /**
     * Point light data.
     */
    class Point
    {
    public:
        Vector3 color;
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;

        Point(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation);
    };

    /**
     * Spot light data.
     */
    class Spot
    {
    public:
        Vector3 color;
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
        float falloffAngle;
        float falloffExponent;

        Spot(const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent);
    };

    /**
     * Constructor.
     */
    Light(Light::Type type, const Vector3& color);

    /**
     * Constructor.
     */
    Light(Light::Type type, const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation);

    /**
     * Constructor.
     */
    Light(Light::Type type, const Vector3& color, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, float falloffAngle, float falloffExponent);

    /**
     * Sets the node associated with this light.
     * 
     * @param node The node to be associated with this light.
     */
    void setNode(Node* node);

    Light::Type _type;
    union
    {
        Directional* _directional;
        Point* _point;
        Spot* _spot;
    };
    Node* _node;
};

}

#endif
