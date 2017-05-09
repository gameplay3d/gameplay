#pragma once

#include "Component.h"
#include "Vector3.h"

namespace gameplay
{

/**
 * Defines a light. 
 *
 * There are 3 types of lights that can be created:
 * directional, point and spot lights.
 */
class Light : public Component
{
    friend class SceneObject;
    friend class Serializer::Activator;

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
     *
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
     * Gets the light type.
     * 
     * @return The light type.
     */
    Type getType() const;

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
     * Gets the inverse of the range of point or spot light.
     *
     * @return The range of the point or spot light.
     */
    float getRangeInverse() const;

   /**
     * Sets the range of point or spot light.
     *
     * @param range The range of point or spot light.
     */
    void setRangeInverse(float rangeInverse);

    /**
     * Returns the inner angle the spot light (in radians).
     *
     * @return The inner angle of the spot light (in radians).
     */
    float getInnerAngle() const;

    /**
     * Sets the inner angle of a spot light (in radians).
     *
     * @param innerAngle The angle of spot light (in radians).
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
     * @param outerAngle The angle of spot light (in radians).
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

    /**
     * @see Serializable::getSerializedClassName
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
     * @see Serializer::Activator::CreateObjectCallback
	 */
    static std::shared_ptr<Serializable> createObject();
    
	/**
     * @see Serializer::Activator::EnumToStringCallback
	 */
    static std::string enumToString(const std::string& enumName, int value);

	/**
     * @see Serializer::Activator::EnumParseCallback
	 */
    static int enumParse(const std::string& enumName, const std::string& str);
    
private:

    Light::Type _type;
    Vector3 _color;
    float _range;
    float _rangeInverse;
    float _innerAngle;
    float _innerAngleCos;
    float _outerAngle;
    float _outerAngleCos;
};

}
