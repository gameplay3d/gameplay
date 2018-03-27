#pragma once

#include "Component.h"
#include "Vector3.h"

namespace gameplay
{

/**
 * Defines a light. 
 *
 * There are 3 types of lights that can be created:
 *
 *     directional, point and spot lights.
 */
class Light : public Component
{
    friend class SceneObject;
    friend class Activator;

public:

    /**
     * Defines the supported light types.
     */
    enum Type
    {
        TYPE_DIRECTIONAL,
        TYPE_POINT,
        TYPE_SPOT
    };

    /**
     * Defines the lighting.
     */
    enum Lighting
    {
        LIGHTING_REALTIME,
        LIGHTING_BAKED
    };

    /**
     * Defines the shadows to be applied.
     */
    enum Shadows
    {
        SHADOWS_NONE,
        SHADOWS_HARD,
        SHADOWS_SOFT
    };

    /**
     * Constructor
     */
    Light();

    /**
     * Destructor.
     */
    ~Light();

    /**
     * Sets the type of light.
     *
     * @param type The type of light.
     */
    void setType(Light::Type type);

    /**
     * Gets the type of light.
     *
     * @return The type of light.
     */
    Light::Type getType() const;

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
     * Sets the lighting intensity to be applied.
     *
     * @param intensity The lighting intensity to be applied.
     */
    void setIntensity(float intensity);

    /**
     * Gets the lighting intensity to be applied.
     *
     * @return The lighting intensity to be applied.
     */
    float getIntensity() const;

    /**
     * Returns the Range of the point or spot light. 
     *
     * @return The range of the point or spot light.
     */
    float getRange() const;

    /**
     * Sets the range of point or spot light.
     *
     * @param range The range of point or spot light.
     */
    void setRange(float range);

    /**
     * Returns the inner angle the spot light (in radians).
     *
     * @return The inner angle of the spot light (in radians).
     */
    float getAngle() const;

    /**
     * Sets the angle of a spot light (in degress).
     *
     * @param angle The angle of spot light (in degrees).
     */
    void setAngle(float angle);

    /**
     * Returns the cosine of the inner angle of spot light.
     *
     * @return The cosine of the inner angle of spot light.
     */
    float getAngleCos() const;

    /**
     * Sets the lighting to be applied.
     *
     * @param lighting The lighting to be applied.
     */
    void setLighting(Light::Lighting lighting);

    /**
     * Gets the lighting to be applied.
     *
     * @return The lighting to be applied.
     */
    Light::Lighting getLighting() const;

    /**
     * Sets the shadows to be applied.
     *
     * @param shadows The shadows to be applied.
     */
    void setShadows(Light::Shadows shadows);

    /**
     * Gets the shadows to be applied.
     *
     * @return The shadows to be applied.
     */
    Light::Shadows getShadows() const;

    /**
     * Resets the light to the default values for the specified light type.
     *
     * @param type The type of light to reset for.
     */
    void reset(Light::Type type);

    /**
     * @see Component::getTypeId
     */
    Component::TypeId getTypeId();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();
    
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();
    
    /**
     * @see Activator::enumToString
     */
    static std::string enumToString(const std::string& enumName, int value);

    /**
     * @see Activator::enumParse
     */
    static int enumParse(const std::string& enumName, const std::string& str);
    
private:

    Light::Type _type;
    Vector3 _color;
    float _intensity;
    float _range;
    float _angle;
    float _angleCos;
    Light::Lighting _lighting;
    Light::Shadows _shadows;
};

}
