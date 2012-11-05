#ifndef EFFECT_H_
#define EFFECT_H_

#include "Object.h"

namespace gameplay
{

class Effect : public Object
{
public:
    /**
     * Defines the set of supported texture wrapping modes.
     */
    enum Wrap
    {
        REPEAT = 0,
        CLAMP
    };
    /**
     * Defines the set of supported texture filters.
     */
    enum Filter
    {
        NEAREST = 0,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR
    };
public:
    /**
     * Constructor.
     */
    Effect(void);

    /**
     * Destructor.
     */
    virtual ~Effect(void);

    virtual unsigned int getTypeId(void) const;

    virtual const char* getElementName(void) const;

    virtual void writeBinary(FILE* file);

    virtual void writeText(FILE* file);

    void setAmbient(Vector4 color);
    void setDiffuse(Vector4 color);
    void setSpecular(Vector4 color);
    void setShininess(float shininess); // u_specularExponent
    void setWrapS(Wrap wrapS);
    void setWrapT(Wrap wrapT);
    void setMinFilter(Filter minFilter);
    void setMagFilter(Filter magFilter);
    void setTextureFilename(std::string path);
    void setAlpha(float alpha);
    void setLighting(bool hasLighting);
    void setLightColor(Vector4 color);

public:
    static const std::string wrapStr[2];
    static const std::string filterStr[6];
private:
    Vector4 ambientColor;
    Vector4 diffuseColor;
    Vector4 specularColor;
    Vector4 lightColor;
    float specularExponent;
    Wrap wrapS;
    Wrap wrapT;
    Filter minFilter;
    Filter magFilter;
    std::string texFilename;
    bool hasTexture;
    float alpha;
    bool hasLighting;
    std::string _vertexShader;
    std::string _fragmentShader;
};

}

#endif
