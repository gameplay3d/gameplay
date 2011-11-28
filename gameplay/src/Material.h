# ifndef MATERIAL_H_
# define MATERIAL_H_

#include "RenderState.h"
#include "Technique.h"
#include "Properties.h"
#include "Mesh.h"

namespace gameplay
{

/**
 * Defines a material for an object to be rendered.
 *
 * This class encapsulates a set of rendering techniques that can be used to render an
 * object. This class facilitates loading of techniques using specified shaders or
 * material files (.material). When multiple techniques are loaded using a material file,
 * the current technique for an object can be set at runtime.
 */
class Material : public RenderState
{
    friend class Technique;
    friend class Pass;
    friend class RenderState;

public:

    /**
     * Creates a material from a specified file path.
     *
     * @param materialPath Path path to the material file.
     * 
     * @return A new Material.
     */
    static Material* create(const char* materialPath);

    /**
     * Creates a material from the specified effect.
     *
     * The returned material has a single technique and a single pass for the
     * given effect.
     *
     * @param effect Effect for the new material.
     * 
     * @return A new Material.
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
     * @param defines New-line delimitted list of preprocessor defines.
     * 
     * @return A new Material.
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
    Technique* getTechnique(unsigned int index) const;

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
     * @param index Index of the technique to set.
     */
    void setTechnique(unsigned int index);

    /**
     * Sets the current material technique. 
     *
     * @param id ID of the technique to set.
     */
    void setTechnique(const char* id);

    /**
     * Stores a binding for the specified mesh onto all techniques and passes in this material.
     *
     * This method creates and stores a VertexAttributeBinding for all techniques and passes
     * onto the  specified Mesh. When a mesh binding is set, the VertexAttributeBinding
     * will be automatically bound when the bind() method is called for a pass.
     *
     * @param mesh The Mesh to create and store a VertexAttributeBinding for (or NULL to remove an existing mesh bindings).
     */
    void setMeshBinding(Mesh* mesh);

private:

    /**
     * Constructor.
     */
    Material();

    /**
     * Destructor.
     */
    ~Material();

    /**
     * Hidden copy constructor.
     */
    Material(const Material& m);

    /**
     * Loads a technique from the given properties object into the specified material.
     */
    static bool loadTechnique(Material* material, Properties* techniqueProperties);

    /**
     * Load a pass from the given properties object into the specified technique.
     */
    static bool loadPass(Technique* technique, Properties* passProperites);

    /**
     * Loads render state from the specified properties object.
     */
    static void loadRenderState(RenderState* renderState, Properties* properties);

    Technique* _currentTechnique;
    std::vector<Technique*> _techniques;
};

}

#endif
