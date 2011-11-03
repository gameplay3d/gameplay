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
     * @param color Color of the light.
     * 
     * @return The new directional light.
     */
    static Light* createDirectional(const Vector3& color);

    /**
     * Creates a point light.
     * 
     * @param color The light's color.
     * @param range The light's range.
     * 
     * @return The new point light.
     */
    static Light* createPoint(const Vector3& color, float range);

    /**
     * Creates a spot light.
     * 
     * @param color The light's color.
     * @param range The light's range.
     * @param innerCosAngle The light's inner angle (in radians).
     * @param outerCosAngle The light's outer angle (in radians).
     * 
     * @return The new spot light.
     */
    static Light* createSpot(const Vector3& color, float range, float innerAngle, float outerAngle);

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

    /**
     * Returns the Range of the point or spot light. 
     *
     * @return The range of the point or spot light.
     */
    float getRange() const;

    /**
     * Sets the range of point or spot light.
     *
     * @param range of point or spot light.
     */
    void setRange(float range);

    /**
     * Returns the inner angle the spot light (in radians).
     *
     * @return The inner angle of the spot light (in radians).
     */
    float getInnerAngle() const;

    /**
     * Sets the inner angle of a spot light (in radians).
     *
     * @param inner angle of spot light (in radians).
     */
    void setInnerAngle(float innerAngle);

    /**
     * Returns the outer angle of the spot light (in radians).
     *
     * @return The outer angle of the spot light (in radians).
     */
    float getOuterAngle() const;

    /**
     * Sets the outer angle of a spot light (in radians).
     *
     * @param outer angle of spot light (in radians).
     */
    void setOuterAngle(float outerAngle);

    /**
     * Returns the cosine of the inner angle of spot light.
     *
     * @return The cosine of the inner angle of spot light.
     */
    float getInnerAngleCos() const;

    /**
     * Returns the cosine of the outer angle of spot light.
     *
     * @return The cosine of the outer angle of spot light.
     */
    float getOuterAngleCos() const;


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
        float range;

        Point(const Vector3& color, float range);
    };

    /**
     * Spot light data.
     */
    class Spot
    {
    public:
        Vector3 color;
        float range;
        float innerAngle;
        float innerAngleCos;
        float outerAngle;
        float outerAngleCos;

        Spot(const Vector3& color, float range, float innerAngle, float outerAngle);
    };

    /**
     * Constructor for the directional light.
     */
    Light(Light::Type type, const Vector3& color);

    /**
     * Constructor for point light.
     */
    Light(Light::Type type, const Vector3& color, float range);

    /**
     * Constructor for spot light.
     */
    Light(Light::Type type, const Vector3& color, float range, float innerAngle, float outerAngle);

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
