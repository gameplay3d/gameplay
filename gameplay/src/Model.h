#ifndef MODEL_H_
#define MODEL_H_

#include "Mesh.h"
#include "MeshSkin.h"
#include "Material.h"
#include "Drawable.h"

namespace gameplay
{

class Bundle;
class MeshSkin;


/**
 * Defines a Model or mesh renderer which is an instance of a Mesh. 
 *
 * A model has a mesh that can be drawn with the specified materials for
 * each of the mesh parts within it.
 */
class Model : public Ref, public Drawable
{
    friend class Node;
    friend class Scene;
    friend class Mesh;
    friend class Bundle;

public:

    /**
     * Creates a new Model.
     * @script{create}
     */
    static Model* create(Mesh* mesh);

    /**
     * Returns the Mesh for this Model.
     *
     * @return The Mesh for this Model.
     */
    Mesh* getMesh() const;

    /**
     * Returns the number of parts in the Mesh for this Model.
     *
     * @return The number of parts in the Mesh for this Model.
     */
    unsigned int getMeshPartCount() const;

    /**
     * Returns the Material currently bound to the specified mesh part.
     *
     * If partIndex is >= 0 and no Material is directly bound to the specified
     * mesh part, the shared Material will be returned.
     *
     * @param partIndex The index of the mesh part whose Material to return (-1 for shared material).
     *
     * @return The requested Material, or NULL if no Material is set.
     */
    Material* getMaterial(int partIndex = -1);

    /**
     * Sets a material to be used for drawing this Model.
     *
     * The specified Material is applied for the MeshPart at the given index in
     * this Model's Mesh. A partIndex of -1 sets a shared Material for
     * all mesh parts, whereas a value of 0 or greater sets the Material for the
     * specified mesh part only.
     *
     * Mesh parts will use an explicitly set part material, if set; otherwise they
     * will use the globally set material.
     *
     * @param material The new material.
     * @param partIndex The index of the mesh part to set the material for (-1 for shared material).
     */
    void setMaterial(Material* material, int partIndex = -1);

    /**
     * Sets a material to be used for drawing this Model.
     *
     * A Material is created from the given vertex and fragment shader source files.
     * The Material is applied for the MeshPart at the given index in this Model's
     * Mesh. A partIndex of -1 sets a shared Material for all mesh parts, whereas a
     * value of 0 or greater sets the Material for the specified mesh part only.
     *
     * Mesh parts will use an explicitly set part material, if set; otherwise they
     * will use the globally set material.
     *
     * @param vshPath The path to the vertex shader file.
     * @param fshPath The path to the fragment shader file.
     * @param defines A new-line delimited list of preprocessor defines. May be NULL.
     * @param partIndex The index of the mesh part to set the material for (-1 for shared material).
     *
     * @return The newly created and bound Material, or NULL if the Material could not be created.
     */
    Material* setMaterial(const char* vshPath, const char* fshPath, const char* defines = NULL, int partIndex = -1);

    /**
     * Sets a material to be used for drawing this Model.
     *
     * A Material is created from the specified material file.
     * The Material is applied for the MeshPart at the given index in this Model's
     * Mesh. A partIndex of -1 sets a shared Material for all mesh parts, whereas a
     * value of 0 or greater sets the Material for the specified mesh part only.
     *
     * Mesh parts will use an explicitly set part material, if set; otherwise they
     * will use the globally set material.
     *
     * @param materialPath The path to the material file.
     * @param partIndex The index of the mesh part to set the material for (-1 for shared material).
     *
     * @return The newly created and bound Material, or NULL if the Material could not be created.
     */
    Material* setMaterial(const char* materialPath, int partIndex = -1);

    /**
     * Determines if a custom (non-shared) material is set for the specified part index.
     *
     * @param partIndex MeshPart index.
     *
     * @return True if a custom MeshPart material is set for the specified index, false otherwise.
     */
    bool hasMaterial(unsigned int partIndex) const;

    /**
     * Returns the MeshSkin.
     *
     * @return The MeshSkin, or NULL if one is not set.
     */
    MeshSkin* getSkin() const;

    /**
     * @see Drawable::draw
     *
     * Binds the vertex buffer and index buffers for the Mesh and
     * all of its MeshPart's and draws the mesh geometry.
     * Any other state necessary to render the Mesh, such as
     * rendering states, shader state, and so on, should be set
     * up before calling this method.
     */
    unsigned int draw(bool wireframe = false);

private:

    /**
     * Constructor.
     */
    Model();

    /**
     * Constructor.
     */
    Model(Mesh* mesh);

    /**
     * Destructor. Hidden use release() instead.
     */
    ~Model();

    /**
     * Hidden copy assignment operator.
     */
    Model& operator=(const Model&);
    
    /**
     * @see Drawable::setNode
     */
    void setNode(Node* node);

    /**
     * @see Drawable::clone
     */
    Drawable* clone(NodeCloneContext& context);

    /**
     * Sets the MeshSkin for this model.
     *
     * @param skin The MeshSkin for this model.
     */
    void setSkin(MeshSkin* skin);

    /**
     * Sets the specified material's node binding to this model's node.
     */
    void setMaterialNodeBinding(Material *m);

    void validatePartCount();

    Mesh* _mesh;
    Material* _material;
    unsigned int _partCount;
    Material** _partMaterials;
    MeshSkin* _skin;
};

}

#endif
