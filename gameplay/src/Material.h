#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "RenderState.h"
#include "Technique.h"
#include "Properties.h"

namespace gameplay
{

class NodeCloneContext;

/**
 * Defines a material for an object to be rendered.
 *
 * This class encapsulates a set of rendering techniques that can be used to render an
 * object. This class facilitates loading of techniques using specified shaders or
 * material files (.material). When multiple techniques are loaded using a material file,
 * the current technique for an object can be set at runtime.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-Materials
 */
class Material : public RenderState
{
    friend class Technique;
    friend class Pass;
    friend class RenderState;
    friend class Node;
    friend class Model;

public:

    /**
     * Pass creation callback function definition.
     */
    typedef std::string(*PassCallback)(Pass*, void*);

    /**
     * Creates a material using the data from the Properties object defined at the specified URL, 
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
     * 
     * @param url The URL pointing to the Properties object defining the material.
     * 
     * @return A new Material or NULL if there was an error.
     * @script{create}
     */
    static Material* create(const char* url);

    /**
     * Creates a material from a Properties file.
     *
     * This overloaded method allows you to pass a function pointer to be called back for each
     * pass that is loaded for the material. The passed in callback receives a pointer to each
     * Pass being created and returns a string of optional defines to append to the shaders
     * being compiled for each Pass. The function is called during Pass creation, prior to
     * compiling the shaders for that Pass. MaterialParameters can be safely modified in this
     * callback even though the shader is not yet compiled.
     *
     * @param url The URL pointing to the Properties object defining the material.
     * @param callback Function pointer to be called during Pass creation.
     * @param cookie Optional custom parameter to be passed to the callback function.
     *
     * @return A new Material or NULL if there was an error.
     * @script{ignore}
     */
    static Material* create(const char* url, PassCallback callback, void* cookie = NULL);

    /**
     * Creates a material from the specified properties object.
     * 
     * @param materialProperties The properties object defining the 
     *      material (must have namespace equal to 'material').
     * @return A new Material.
     * @script{create}
     */
    static Material* create(Properties* materialProperties);

    /**
     * Creates a material from the specified effect.
     *
     * The returned material has a single technique and a single pass for the
     * given effect.
     *
     * @param effect Effect for the new material.
     * 
     * @return A new Material.
     * @script{create}
     */
    static Material* create(Effect* effect);

    /**
     * Creates a material using the specified vertex and fragment shader.
     *
     * The returned material has a single technique and a single pass for the
     * given effect.
     *
     * @param vshPath Path to the vertex shader file.
     * @param fshPath Path to the fragment shader file.
     * @param defines New-line delimited list of preprocessor defines.
     * 
     * @return A new Material.
     * @script{create}
     */
    static Material* create(const char* vshPath, const char* fshPath, const char* defines = NULL);

    /**
     * Returns the number of techniques in the material.
     *
     * @return The technique count.
     */
    unsigned int getTechniqueCount() const;

    /**
     * Returns the technique at the specified index in this material.
     *
     * @param index The index of the technique to return.
     * 
     * @return The specified technique.
     */
    Technique* getTechniqueByIndex(unsigned int index) const;

    /**
     * Returns the technique with the specified ID in this material.
     *
     * @param id The ID of the technique to return.
     * 
     * @return The specified technique.
     */
    Technique* getTechnique(const char* id) const;

    /**
     * Returns this material's current technique.
     *
     * @return The current technique.
     */
    Technique* getTechnique() const;

    /**
     * Sets the current material technique. 
     *
     * @param id ID of the technique to set.
     */
    void setTechnique(const char* id);

    /**
     * @see RenderState::setNodeBinding
     */
    void setNodeBinding(Node* node);

private:

    /**
     * Constructor.
     */
    Material();

    /**
     * Constructor.
     */
    Material(const Material& m);
    
    /**
     * Destructor.
     */
    ~Material();

    /**
     * Clones this material.
     * 
     * @param context The clone context.
     * 
     * @return The newly created material.
     * @script{create}
     */
    Material* clone(NodeCloneContext &context) const;

    /**
     * Creates a new material with optional pass callback function.
     */
    static Material* create(Properties* materialProperties, PassCallback callback, void* cookie);

    /**
     * Loads a technique from the given properties object into the specified material.
     */
    static bool loadTechnique(Material* material, Properties* techniqueProperties, PassCallback callback, void* cookie);

    /**
     * Load a pass from the given properties object into the specified technique.
     */
    static bool loadPass(Technique* technique, Properties* passProperites, PassCallback callback, void* cookie);

    /**
     * Loads render state from the specified properties object.
     */
    static void loadRenderState(RenderState* renderState, Properties* properties);

    Technique* _currentTechnique;
    std::vector<Technique*> _techniques;
};

}

#endif
