#include "Base.h"
#include "Effect.h"

namespace gameplay
{

    std::string Effect::wrapStr[] = { "REPEAT", "CLAMP" };
    std::string Effect::filterStr[] = {
        "NEAREST",
        "LINEAR",
        "NEAREST_MIPMAP_NEAREST",
        "LINEAR_MIPMAP_NEAREST",
        "NEAREST_MIPMAP_LINEAR",
        "LINEAR_MIPMAP_LINEAR"
    };

    Effect::Effect(void) :
        ambientColor(Vector4(.2f, .2f, .2f, 1.0f)),
        diffuseColor(Vector4(.8f, .8f, .8f, 1.0f)),
        specularColor(Vector4(.0f, .0f, .0f, .0f)),
        lightColor(Vector4(.8f, .8f, .8f, 1.0f)),
        specularExponent(.0f),
        wrapS(REPEAT),
        wrapT(REPEAT),
        minFilter(NEAREST_MIPMAP_LINEAR),
        magFilter(LINEAR),
        texFilename(""),
        hasTexture(false),
        alpha(1.0f),
        hasLighting(false)
{

}

Effect::~Effect(void)
{
}

unsigned int Effect::getTypeId(void) const
{
    return EFFECT_ID;
}

const char* Effect::getElementName(void) const
{
    return "Effect";
}

void Effect::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_vertexShader, file);
    write(_fragmentShader, file);
}

void Effect::writeText(FILE* file)
{
    fprintf(file, "\t\t\tu_ambientColor = %f, %f, %f, %f\n",
            this->ambientColor.x,
            this->ambientColor.y,
            this->ambientColor.z,
            this->ambientColor.w);
    // TODO: u_lightDirection
    // light color is a fix value at the moment
    fprintf(file, "\t\t\tu_lightColor = %f, %f, %f, %f\n",
            this->lightColor.x,
            this->lightColor.y,
            this->lightColor.z,
            this->lightColor.w);
    fprintf(file, "\t\t\tu_specularExponent = %f\n", this->specularExponent);
    fprintf(file, "\t\t\tu_modulateAlpha = %f\n\n", this->alpha);
    std::string unlit = "";
    if(!this->hasLighting)
    {
       unlit = "-unlit";
    }
    if(this->hasTexture)
    {
        fprintf(file, "\t\t\tvertexShader = res/shaders/textured%s.vert\n", unlit.c_str());
        fprintf(file, "\t\t\tfragmentShader = res/shaders/textured%s.frag\n\n", unlit.c_str());
        fprintf(file, "\t\t\tsampler u_diffuseTexture\n");
        fprintf(file, "\t\t\t{\n");
        // e.g. path = res/duck-diffuse.png
        fprintf(file, "\t\t\t\tpath = %s\n", this->texFilename.c_str());
        fprintf(file, "\t\t\t\twrapS = %s\n", wrapStr[this->wrapS].c_str());
        fprintf(file, "\t\t\t\twrapT = %s\n", wrapStr[this->wrapT].c_str());
        fprintf(file, "\t\t\t\tminFilter = %s\n", filterStr[this->minFilter].c_str());
        fprintf(file, "\t\t\t\tmagFilter = %s\n", filterStr[this->magFilter].c_str());
        fprintf(file, "\t\t\t\tmipmap = false\n");
        fprintf(file, "\t\t\t}\n\n");
    }
    else
    {
        fprintf(file, "\t\t\tvertexShader = res/shaders/colored%s.vert\n", unlit.c_str());
        fprintf(file, "\t\t\tfragmentShader = res/shaders/colored%s.frag\n\n", unlit.c_str());
            fprintf(file, "\t\t\tu_diffuseColor = %f, %f, %f, %f\n",
            this->diffuseColor.x,
            this->diffuseColor.y,
            this->diffuseColor.z,
            this->diffuseColor.w);
    }
    fprintf(file, "\t\t\trenderState\n");
    fprintf(file, "\t\t\t{\n");
    if(this->alpha < 1.0f)
    {
        fprintf(file, "\t\t\t\tcullFace = false\n");
        }
    else
    {
        fprintf(file, "\t\t\t\tcullFace = true\n");
    }
    fprintf(file, "\t\t\t\tdepthTest = true\n");
    fprintf(file, "\t\t\t}\n");
}

void Effect::setAmbient(Vector4 color)
{
    this->ambientColor = color;
}

void Effect::setDiffuse(Vector4 color)
{
    this->diffuseColor = color;
}

void Effect::setSpecular(Vector4 color)
{
    this->specularColor = color;
}

void Effect::setShininess(float shininess)
{
    this->specularExponent = shininess;
}

void Effect::setWrapS(Wrap wrapS)
{
    this->wrapS = wrapS;
}

void Effect::setWrapT(Wrap wrapT)
{
    this->wrapT = wrapT;
}

void Effect::setMinFilter(Filter minFilter)
{
    this->minFilter = minFilter;
}

void Effect::setMagFilter(Filter magFilter)
{
    this->magFilter = magFilter;
}

void Effect::setTextureFilename(std::string path)
{
    int x = path.find_last_of('/');
    this->texFilename = "res/" + path.substr(path.find_last_of('/') + 1);
    this->hasTexture = true;
}

void Effect::setAlpha(float alpha)
{
    this->alpha = alpha;
}

void Effect::setLighting(bool hasLighting)
{
    this->hasLighting = hasLighting;
}

void Effect::setLightColor(Vector4 color)
{
    this->lightColor = color;
}
}
