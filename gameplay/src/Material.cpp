#include "Base.h"
#include "Material.h"
#include "FileSystem.h"
#include "Effect.h"
#include "Technique.h"
#include "Pass.h"
#include "Properties.h"
#include "Node.h"

namespace gameplay
{

Material::Material() :
    _currentTechnique(NULL)
{
}

Material::Material(const Material& m)
{
}

Material::~Material()
{
    // Destroy all the techniques.
    for (unsigned int i = 0, count = _techniques.size(); i < count; ++i)
    {
        Technique* technique = _techniques[i];
        if (technique)
        {
            SAFE_RELEASE(technique);
        }
    }
}

Material* Material::create(const char* url)
{
    assert(url);

    // Load the material properties from file
    Properties* properties = Properties::create(url);
    assert(properties);
    if (properties == NULL)
    {
        return NULL;
    }

    Material* material = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
    SAFE_DELETE(properties);

    return material;
}

Material* Material::create(Properties* materialProperties)
{
    // Check if the Properties is valid and has a valid namespace.
    assert(materialProperties);
    if (!materialProperties || !(strcmp(materialProperties->getNamespace(), "material") == 0))
    {
        return NULL;
    }

    // Create new material from the file passed in.
    Material* material = new Material();

    // Go through all the material properties and create techniques under this material.
    Properties* techniqueProperties = NULL;
    while ((techniqueProperties = materialProperties->getNextNamespace()))
    {
        if (strcmp(techniqueProperties->getNamespace(), "technique") == 0)
        {
            if (!loadTechnique(material, techniqueProperties))
            {
                SAFE_RELEASE(material);
                return NULL;
            }
        }
    }

    // Load uniform value parameters for this material
    loadRenderState(material, materialProperties);

    // Set the current technique to the first found technique
    if (material->getTechniqueCount() > 0)
    {
        material->setTechnique((unsigned int)0);
    }

    return material;
}

Material* Material::create(Effect* effect)
{
    // Create a new material with a single technique and pass for the given effect
    Material* material = new Material();

    Technique* technique = new Technique(NULL, material);
    material->_techniques.push_back(technique);

    Pass* pass = new Pass(NULL, technique, effect);
    technique->_passes.push_back(pass);
    effect->addRef();

    material->_currentTechnique = technique;

    return material;
}

Material* Material::create(const char* vshPath, const char* fshPath, const char* defines)
{
    // Create a new material with a single technique and pass for the given effect
    Material* material = new Material();

    Technique* technique = new Technique(NULL, material);
    material->_techniques.push_back(technique);

    Pass* pass = Pass::create(NULL, technique, vshPath, fshPath, defines);
    if (!pass)
    {
        SAFE_RELEASE(material);
        return NULL;
    }
    technique->_passes.push_back(pass);

    material->_currentTechnique = technique;

    return material;
}

Material* Material::clone(NodeCloneContext &context) const
{
    Material* material = new Material();
    RenderState::cloneInto(material, context);

    for (std::vector<Technique*>::const_iterator it = _techniques.begin(); it != _techniques.end(); ++it)
    {
        const Technique* technique = *it;
        Technique* techniqueClone = technique->clone(material, context);
        material->_techniques.push_back(techniqueClone);
        if (_currentTechnique == technique)
        {
            material->_currentTechnique = techniqueClone;
        }
    }
    return material;
}

unsigned int Material::getTechniqueCount() const
{
    return _techniques.size();
}

Technique* Material::getTechnique(unsigned int index) const
{
    assert(index < _techniques.size());

    return _techniques[index];
}

Technique* Material::getTechnique(const char* id) const
{
    for (unsigned int i = 0, count = _techniques.size(); i < count; ++i)
    {
        Technique* t = _techniques[i];
        if (strcmp(t->getId(), id) == 0)
        {
            return t;
        }
    }

    return NULL;
}

Technique* Material::getTechnique() const
{
    return _currentTechnique;
}

void Material::setTechnique(const char* id)
{
    Technique* t = getTechnique(id);
    if (t)
    {
        _currentTechnique = t;
    }
}

void Material::setTechnique(unsigned int index)
{
    Technique* t = getTechnique(index);
    if (t)
    {
        _currentTechnique = t;
    }
}

bool Material::loadTechnique(Material* material, Properties* techniqueProperties)
{
    // Create a new technique
    Technique* technique = new Technique(techniqueProperties->getId(), material);

    // Go through all the properties and create passes under this technique.
    techniqueProperties->rewind();
    Properties* passProperties = NULL;
    while ((passProperties = techniqueProperties->getNextNamespace()))
    {
        if (strcmp(passProperties->getNamespace(), "pass") == 0)
        {
            // Create and load passes.
            if (!loadPass(technique, passProperties))
            {
                SAFE_RELEASE(technique);
                return false;
            }
        }
    }

    // Load uniform value parameters for this technique
    loadRenderState(technique, techniqueProperties);

    // Add the new technique to the material
    material->_techniques.push_back(technique);

    return true;
}

bool Material::loadPass(Technique* technique, Properties* passProperties)
{
    // Fetch shader info required to create the effect of this technique.
    const char* vertexShaderPath = passProperties->getString("vertexShader");
    assert(vertexShaderPath);
    const char* fragmentShaderPath = passProperties->getString("fragmentShader");
    assert(fragmentShaderPath);
    const char* defines = passProperties->getString("defines");
    std::string define;
    if (defines != NULL)
    {
        define = defines;
        define.insert(0, "#define ");
        unsigned int pos;
        while ((pos = define.find(';')) != std::string::npos)
        {
            define.replace(pos, 1, "\n#define ");
        }
        define += "\n";
    }

    // Create the pass
    Pass* pass = Pass::create(passProperties->getId(), technique, vertexShaderPath, fragmentShaderPath, define.c_str());
    if (!pass)
    {
        return false;
    }

    // Load render state
    loadRenderState(pass, passProperties);

    // Add the new pass to the technique
    technique->_passes.push_back(pass);

    return true;
}

bool isMaterialKeyword(const char* str)
{
    #define MATERIAL_KEYWORD_COUNT 3
    static const char* reservedKeywords[MATERIAL_KEYWORD_COUNT] =
    {
        "vertexShader",
        "fragmentShader",
        "defines"
    };
    for (unsigned int i = 0; i < MATERIAL_KEYWORD_COUNT; ++i)
    {
        if (strcmp(reservedKeywords[i], str) == 0)
        {
            return true;
        }
    }
    return false;
}

Texture::Filter parseTextureFilterMode(const char* str, Texture::Filter defaultValue)
{
    if (str == NULL || strlen(str) == 0)
    {
        return defaultValue;
    }
    else if (strcmp(str, "NEAREST") == 0)
    {
        return Texture::NEAREST;
    }
    else if (strcmp(str, "LINEAR") == 0)
    {
        return Texture::LINEAR;
    }
    else if (strcmp(str, "NEAREST_MIPMAP_NEAREST") == 0)
    {
        return Texture::NEAREST_MIPMAP_NEAREST;
    }
    else if (strcmp(str, "LINEAR_MIPMAP_NEAREST") == 0)
    {
        return Texture::LINEAR_MIPMAP_NEAREST;
    }
    else if (strcmp(str, "NEAREST_MIPMAP_LINEAR") == 0)
    {
        return Texture::NEAREST_MIPMAP_LINEAR;
    }
    else if (strcmp(str, "LINEAR_MIPMAP_LINEAR") == 0)
    {
        return Texture::LINEAR_MIPMAP_LINEAR;
    }
    return defaultValue;
}

Texture::Wrap parseTextureWrapMode(const char* str, Texture::Wrap defaultValue)
{
    if (str == NULL || strlen(str) == 0)
    {
        return defaultValue;
    }
    else if (strcmp(str, "REPEAT") == 0)
    {
        return Texture::REPEAT;
    }
    else if (strcmp(str, "CLAMP") == 0)
    {
        return Texture::CLAMP;
    }
    return defaultValue;
}

void Material::loadRenderState(RenderState* renderState, Properties* properties)
{
    // Rewind the properties to start reading from the start
    properties->rewind();

    const char* name;
    while (name = properties->getNextProperty())
    {
        if (isMaterialKeyword(name))
            continue; // keyword - skip

        switch (properties->getType())
        {
        case Properties::NUMBER:
            renderState->getParameter(name)->setValue(properties->getFloat());
            break;
        case Properties::VECTOR2:
            {
                Vector2 vector2;
                if (properties->getVector2(NULL, &vector2))
                {
                    renderState->getParameter(name)->setValue(vector2);
                }
            }
            break;
        case Properties::VECTOR3:
            {
                Vector3 vector3;
                if (properties->getVector3(NULL, &vector3))
                {
                    renderState->getParameter(name)->setValue(vector3);
                }
            }
            break;
        case Properties::VECTOR4:
            {
                Vector4 vector4;
                if (properties->getVector4(NULL, &vector4))
                {
                    renderState->getParameter(name)->setValue(vector4);
                }
            }
            break;
        case Properties::MATRIX:
            {
                Matrix matrix;
                if (properties->getMatrix(NULL, &matrix))
                {
                    renderState->getParameter(name)->setValue(matrix);
                }
            }
            break;
        default:
            {
                // Assume this is a parameter auto-binding
                renderState->setParameterAutoBinding(name, properties->getString());
            }
            break;
        }
    }

    // Iterate through all child namespaces searching for samplers and render state blocks
    Properties* ns;
    while (ns = properties->getNextNamespace())
    {
        if (strcmp(ns->getNamespace(), "sampler") == 0)
        {
            // Read the texture uniform name
            name = ns->getId();
            if (strlen(name) == 0)
                continue; // missing texture uniform name

            // Get the texture path
            const char* path = ns->getString("path");
            if (path == NULL || strlen(path) == 0)
                continue; // missing texture path

            // Read texture state (booleans default to 'false' if not present)
            bool mipmap = ns->getBool("mipmap");
            Texture::Wrap wrapS = parseTextureWrapMode(ns->getString("wrapS"), Texture::REPEAT);
            Texture::Wrap wrapT = parseTextureWrapMode(ns->getString("wrapT"), Texture::REPEAT);
            Texture::Filter minFilter = parseTextureFilterMode(ns->getString("minFilter"), mipmap ? Texture::NEAREST_MIPMAP_LINEAR : Texture::LINEAR);
            Texture::Filter magFilter = parseTextureFilterMode(ns->getString("magFilter"), Texture::LINEAR);

            // Set the sampler parameter
            Texture::Sampler* sampler = renderState->getParameter(name)->setValue(path, mipmap);
            if (sampler)
            {
                sampler->setWrapMode(wrapS, wrapT);
                sampler->setFilterMode(minFilter, magFilter);
            }
        }
        else if (strcmp(ns->getNamespace(), "renderState") == 0)
        {
            while (name = ns->getNextProperty())
            {
                renderState->getStateBlock()->setState(name, ns->getString());
            }
        }
    }
}

}
