#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Sampler.h"
#include "Light.h"
#include "Constants.h"

namespace gameplay
{

class Material
{
public:

    /**
     * Constructor.
     */
    Material(const std::string& id);

    Material(const Material&);

    /**
     * Destructor.
     */
    virtual ~Material(void);

    /**
     * Returns this material's id string.
     */
    const std::string& getId() const;

    /**
     * Sets this material's id string.
     */
    void setId(const char* id);

    Material* getParent() const;
    void setParent(Material* material);

    void addDefine(const std::string& name);
    bool isDefined(const std::string& name) const;

    const char* getUniform(const char* name) const;
    void setUniform(const std::string& name, const std::string& value);

    const char* getRenderState(const char* name) const;
    void setRenderState(const std::string& name, const std::string& value);

    void setVertexShader(const char* path);
    void setFragmentShader(const char* path);

    /**
     * Creates a sampler and adds it to this material.
     */
    Sampler* createSampler(const std::string& id);
    Sampler* getSampler(const std::string& id) const;
    
    bool isTextured() const;
    bool isBumped() const;
    bool isLit() const;
    bool isSpecular() const;
    bool isTextureRepeat() const;
    bool isVertexColor() const;
    bool isSkinned() const;
    bool isModulateAlpha() const;

    void setLit(bool value);

    /**
     * Writes this material to the given file.
     */
    void writeMaterial(FILE* file);

private:

    Material& operator=(const Material&); // Hidden copy assignment operator.

    void writeDefines(FILE* file, unsigned int& indent);
    void writeUniforms(FILE* file, unsigned int& indent);
    void writeSamplers(FILE* file, unsigned int& indent);
    void writeRenderStates(FILE* file, unsigned int& indent);
    void writeTechniqueAndPass(FILE* file, unsigned int& indent);
    void writeTechniqueOpening(FILE* file, unsigned int& indent);

private:
    Material* _parent;
    std::string _id;
    std::string _vertexShader;
    std::string _fragmentShader;
    bool _lit;

    std::map<std::string, std::string> _defines;
    std::map<std::string, std::string> _uniforms;
    std::map<std::string, std::string> _renderStates;
    std::vector<Sampler*> _samplers;
};

}

#endif
