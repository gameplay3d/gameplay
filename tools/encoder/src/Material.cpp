#include "Material.h"
#include "FileIO.h"
#include "StringUtil.h"

namespace gameplay
{

using std::string;
using std::vector;
using std::map;

Material::Material(const std::string& id) :
    _parent(NULL), _id(id), _lit(false)
{
}

Material::Material(const Material& c) :
    _parent(c._parent),
    _id(c._id), _lit(false),
    _vertexShader(c._vertexShader),
    _fragmentShader(c._fragmentShader),
    _defines(c._defines),
    _uniforms(c._uniforms),
    _renderStates(c._renderStates)
{
    for (vector<Sampler*>::const_iterator it = c._samplers.begin(); it != c._samplers.end(); ++it)
    {
        _samplers.push_back(new Sampler(**it));
    }
}

Material::~Material(void)
{
}

const string& Material::getId() const
{
    return _id;
}

void Material::setId(const char* id)
{
    if (id)
        _id.assign(id);
}

Material* Material::getParent() const
{
    return _parent;
}

void Material::setParent(Material* material)
{
    if (material)
        _parent = material;
}

void Material::addDefine(const string& name)
{
    if (!name.empty())
    {
        _defines[name] = string();
    }
}

bool Material::isDefined(const string& name) const
{
    if (!name.empty())
    {
        return _defines.find(name) != _defines.end();
    }
    return false;
}

const char* Material::getUniform(const char* name) const
{
    map<string, string>::const_iterator it = _uniforms.find(string(name));
    if (it != _uniforms.end())
    {
        return it->second.c_str();
    }
    return NULL;
}

void Material::setUniform(const string& name, const string& value)
{
    _uniforms[name] = value;
}

const char* Material::getRenderState(const char* name) const
{
    map<string, string>::const_iterator it = _renderStates.find(string(name));
    if (it != _renderStates.end())
    {
        return it->second.c_str();
    }
    return NULL;
}

void Material::setRenderState(const string& name, const string& value)
{
    if (!name.empty())
        _renderStates[name] = value;
}

void Material::setVertexShader(const char* path)
{
    if (path)
        _vertexShader.assign(path);
}

void Material::setFragmentShader(const char* path)
{
    if (path)
        _fragmentShader.assign(path);
}

Sampler* Material::createSampler(const string& id)
{
    Sampler* sampler = new Sampler(id.c_str());
    sampler->set("mipmap", "true");
    sampler->set("wrapS", CLAMP);
    sampler->set("wrapT", CLAMP);
    sampler->set(MIN_FILTER, LINEAR_MIPMAP_LINEAR);
    sampler->set(MAG_FILTER, LINEAR);
    _samplers.push_back(sampler);
    return sampler;
}

Sampler* Material::getSampler(const string& id) const
{
    for (vector<Sampler*>::const_iterator it = _samplers.begin(); it != _samplers.end(); ++it)
    {
        Sampler* sampler = *it;
        if (sampler->getId() == id)
        {
            return sampler;
        }
    }
    return NULL;
}

bool Material::isTextured() const
{
    return !_samplers.empty();
}

bool Material::isBumped() const
{
    return getSampler("u_normalmapTexture") != NULL;
}

bool Material::isLit() const
{
    return _lit;
}

bool Material::isSpecular() const
{
    return isDefined(SPECULAR);
}

bool Material::isTextureRepeat() const
{
    return isDefined(TEXTURE_REPEAT);
}

bool Material::isVertexColor() const
{
    return isDefined("VERTEX_COLOR");
}

bool Material::isSkinned() const
{
    return isDefined("SKINNING");
}

bool Material::isModulateAlpha() const
{
    return isDefined("MODULATE_ALPHA");
}

void Material::setLit(bool value)
{
    _lit = value;
}

void Material::writeMaterial(FILE* file)
{
    fprintf(file, "material");
    if (getId().length() > 0)
    {
        fprintf(file, " %s", getId().c_str());
    }
    if (_parent)
    {
         assert(_parent->getId().length() > 0);
        fprintf(file, " : %s", _parent->getId().c_str());
    }
    fprintf(file, "\n");
    fprintf(file, "{\n");
    unsigned int indent = 1;

    writeUniforms(file, indent);
    writeSamplers(file, indent);
    writeRenderStates(file, indent);
    writeTechniqueAndPass(file, indent);

    --indent;
    writeIndent(indent, file);
    fprintf(file, "}\n");
}

void Material::writeDefines(FILE* file, unsigned int& indent)
{
    writeIndent(indent, file);
    fprintf(file, "defines = ");
    for (map<string, string>::const_iterator it = _defines.begin(); it != _defines.end(); ++it)
    {
        if (it != _defines.begin())
        {
            fprintf(file, ";");
        }
        if (it->second.empty())
        {
            fprintf(file, "%s", it->first.c_str());
        }
        else
        {
            fprintf(file, "%s %s", it->first.c_str(), it->second.c_str());
        }
    }
    fprintf(file, "\n");
}

void Material::writeUniforms(FILE* file, unsigned int& indent)
{
    for (map<string, string>::const_iterator it = _uniforms.begin(); it != _uniforms.end(); ++it)
    {
        writeIndent(indent, file);
        fprintf(file, "%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    if (!_uniforms.empty())
    {
        writeIndent(indent, file);
        fprintf(file, "\n");
    }
}

void Material::writeSamplers(FILE* file, unsigned int& indent)
{
    for (vector<Sampler*>::iterator it = _samplers.begin(); it != _samplers.end(); ++it)
    {
        Sampler* sampler = *it;
        Sampler* parentSampler = NULL;
        if (_parent)
        {
            parentSampler = _parent->getSampler(sampler->getId().c_str());
        }
        sampler->writeMaterial(file, indent, parentSampler);
        fprintf(file, "\n");
    }
}

void Material::writeRenderStates(FILE* file, unsigned int& indent)
{
    if (_renderStates.empty())
        return;
    writeIndent(indent, file);
    fprintf(file, "renderState\n");
    writeIndent(indent, file);
    fprintf(file, "{\n");
    ++indent;
    for (map<string, string>::const_iterator it = _renderStates.begin(); it != _renderStates.end(); ++it)
    {
        writeIndent(indent, file);
        fprintf(file, "%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    --indent;
    writeIndent(indent, file);
    fprintf(file, "}\n");
    writeIndent(indent, file);
    fprintf(file, "\n");
}

void Material::writeTechniqueAndPass(FILE* file, unsigned int& indent)
{
    if (!_vertexShader.empty() || !_fragmentShader.empty() || !_defines.empty())
    {
        bool techniqueWritten = false;

        if (!_vertexShader.empty() || 
            !_fragmentShader.empty() || 
            (!_defines.empty() && (!_parent || _parent->_defines != _defines)))
        {
            writeTechniqueOpening(file, indent);
            techniqueWritten = true;
        }
        
        if (!_vertexShader.empty())
        {
            writeIndent(indent, file);
            fprintf(file, "%s = %s\n", "vertexShader", _vertexShader.c_str());
        }
        if (!_fragmentShader.empty())
        {
            writeIndent(indent, file);
            fprintf(file, "%s = %s\n", "fragmentShader", _fragmentShader.c_str());
        }
        if (!_defines.empty())
        {
            if (!_parent || _parent->_defines != _defines)
            {
                writeDefines(file, indent);
            }
        }

        if (techniqueWritten)
        {
            --indent;
            writeIndent(indent, file);
            fprintf(file, "}\n");

            --indent;
            writeIndent(indent, file);
            fprintf(file, "}\n");
        }
    }
}

void Material::writeTechniqueOpening(FILE* file, unsigned int& indent)
{
    // write the techniques
    writeIndent(indent, file);
    fprintf(file, "technique\n");
    writeIndent(indent, file);
    fprintf(file, "{\n");
    ++indent;

    // write the passes
    writeIndent(indent, file);
    fprintf(file, "pass \n");
    writeIndent(indent, file);
    fprintf(file, "{\n");
    ++indent;
}

}
