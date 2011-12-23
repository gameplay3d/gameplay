#ifndef LIGHT_H_
#define LIGHT_H_

#include "Object.h"

namespace gameplay
{

class Light : public Object
{
public:

    static const int COLOR_SIZE = 3;

    /**
     * Constructor.
     */
    Light(void);

    /**
     * Destructor.
     */
    virtual ~Light(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    float getRed() const;
    float getGreen() const;
    float getBlue() const;

    bool isAmbient() const;

    /**
     * Sets the light type to ambient.
     */
    void setAmbientLight();
    void setDirectionalLight();
    void setPointLight();
    void setSpotLight();

    void setColor(float r, float g, float b);
    void setColor(float r, float g, float b, float a);

    void setConstantAttenuation(float value);
    void setLinearAttenuation(float value);
    void setQuadraticAttenuation(float value);
    void setFalloffAngle(float value);
    void setFalloffExponent(float value);

    enum LightType
    {
        DirectionalLight = 1,
        PointLight = 2,
        SpotLight = 3,
        AmbientLight = 255
    };

private:

    static float computeRange(float constantAttenuation, float linearAttenuation, float quadraticAttenuation);
    static float computeInnerAngle(float outerAngle);
    
    unsigned char _lightType;
    float _color[COLOR_SIZE];

    float _constantAttenuation;
    float _linearAttenuation;
    float _quadraticAttenuation;
    float _falloffAngle;
    float _falloffExponent;

    float _range;
    float _innerAngle;
    float _outerAngle;
};

}

#endif
