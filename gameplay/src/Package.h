#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "Mesh.h"
#include "Font.h"
#include "Node.h"
#include "Game.h"

namespace gameplay
{

/**
 * Represents a gameplay binary package file (.gpb) that contains a
 * collection of game resources that can be loaded.
 */
class Package : public Ref
{
    friend class SceneLoader;

public:

    /**
     * Returns a Package for the given resource path.
     *
     * The specified path must reference a valid gameplay binary file.
     * If the package is already loaded, the existing package is returned
     * with its reference count incremented. When no longer needed, the
     * release() method must be called. Note that calling release() does
     * NOT free any actual game objects created/returned from the Package
     * instance and those objects must be released separately.
     */
    static Package* create(const char* path);

    /**
     * Loads the scene with the specified ID from the package.
     * If id is NULL then the first scene found is loaded.
     * 
     * @param id The ID of the scene to load (NULL to load the first scene).
     * 
     * @return The loaded scene, or NULL if the scene could not be loaded.
     */
    Scene* loadScene(const char* id = NULL);

    /**
     * Loads a node with the specified ID from the package.
     *
     * @param id The ID of the node to load in the package.
     * 
     * @return The loaded node, or NULL if the node could not be loaded.
     */
    Node* loadNode(const char* id);

    /**
     * Loads a mesh with the specified ID from the package.
     *
     * @param id The ID of the mesh to load.
     * 
     * @return The loaded mesh, or NULL if the mesh could not be loaded.
     */
    Mesh* loadMesh(const char* id);

    /**
     * Loads a font with the specified ID from the package.
     *
     * @param id The ID of the font to load.
     * 
     * @return The loaded font, or NULL if the font could not be loaded.
     */
    Font* loadFont(const char* id);

    /**
     * Determines if this package contains a top-level object with the given ID.
     *
     * This method performs a case-sensitive comparison.
     *
     * @param id The ID of the object to search for.
     */
    bool contains(const char* id) const;

    /**
     * Returns the number of top-level objects in this package.
     */
    unsigned int getObjectCount() const;

    /**
     * Returns the unique identifier of the top-level object at the specified index in this package.
     *
     * @param index The index of the object.
     * 
     * @return The ID of the object at the given index, or NULL if index is invalid.
     */
    const char* getObjectID(unsigned int index) const;

private:

    class Reference
    {
    public:
        std::string id;
        unsigned int type;
        unsigned int offset;

        /**
         * Constructor.
         */
        Reference();

        /**
         * Destructor.
         */
        ~Reference();
    };

    struct MeshSkinData
    {
        MeshSkin* skin;
        std::vector<std::string> joints;
        std::vector<Matrix> inverseBindPoseMatrices;
    };

    Package(const char* path);

    /**
     * Destructor.
     */
    ~Package();

    /**
     * Finds a reference by ID.
     */
    Reference* find(const char* id) const;

    /**
     * Resets any load session specific state for the package.
     */
    void clearLoadSession();

    /**
     * Returns the ID of the object at the current file position.
     * Returns NULL if not found.
     * 
     * @return The ID string or NULL if not found.
     */
    const char* getIdFromOffset() const;

    /**
     * Returns the ID of the object at the given file offset by searching through the reference table.
     * Returns NULL if not found.
     *
     * @param offset The file offset.
     * 
     * @return The ID string or NULL if not found.
     */
    const char* getIdFromOffset(unsigned int offset) const;

    /**
     * Seeks the file pointer to the object with the given ID and type
     * and returns the relevant Reference.
     *
     * @param id The ID string to search for.
     * @param type The object type.
     * 
     * @return The reference object or NULL if there was an error.
     */
    Reference* seekTo(const char* id, unsigned int type);

    /**
     * Seeks the file pointer to the first object that matches the given type.
     * 
     * @param type The object type.
     * 
     * @return The reference object or NULL if there was an error.
     */
    Reference* seekToFirstType(unsigned int type);

    /**
     * Loads the scene with the specified ID from the package, and loads the specified nodes with mesh rigid body support.
     * If id is NULL then the first scene found is loaded.
     * 
     * @param id The ID of the scene to load (NULL to load the first scene).
     * @param nodesWithMeshRB A list of the IDs of the nodes within the scene that 
     *      should be loaded with support for triangle mesh rigid bodies.
     * 
     * @return The loaded scene, or NULL if the scene could not be loaded.
     */
    Scene* loadScene(const char* id, const std::vector<std::string>* nodesWithMeshRB);

    /**
     * Loads a node with the specified ID from the package, optionally with mesh rigid body support.
     *
     * @param id The ID of the node to load in the package.
     * @param loadWithMeshRBSupport Whether or not to load the node with mesh rigid body support.
     * 
     * @return The loaded node, or NULL if the node could not be loaded.
     */
    Node* loadNode(const char* id, bool loadWithMeshRBSupport);

    /**
     * Internal method to load a node.
     *
     * Only one of node or scene should be passed as non-NULL (or neither).
     */
    Node* loadNode(const char* id, Scene* sceneContext, Node* nodeContext, bool loadWithMeshRBSupport);

    /**
     * Loads a mesh with the specified ID from the package.
     *
     * @param id The ID of the mesh to load.
     * @param loadWithMeshRBSupport Whether to load the mesh with 
     *      support for triangle mesh rigid bodies or not.
     * @param nodeId The id of the mesh's model's parent node.
     * 
     * @return The loaded mesh, or NULL if the mesh could not be loaded.
     */
    Mesh* loadMesh(const char* id, bool loadWithMeshRBSupport, const char* nodeId);

    /**
     * Reads an unsigned int from the current file position.
     *
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(unsigned int* ptr);

    /**
     * Reads an unsigned char from the current file position.
     * 
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(unsigned char* ptr);

    /**
     * Reads a float from the current file position.
     * 
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(float* ptr);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param ptr A pointer to the array where the data will be copied to.
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, T** ptr);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param values A pointer to the vector to copy the values to. The vector will be resized if it is smaller than length.
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, std::vector<T>* values);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param values A pointer to the vector to copy the values to. The vector will be resized if it is smaller than length.
     * @param readSize The size that reads will be preformed at, size must be the same as or smaller then the sizeof(T)
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, std::vector<T>* values, unsigned int readSize);
    
    /**
     * Reads 16 floats from the current file position.
     *
     * @param m A pointer to float array of size 16.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool readMatrix(float* m);

    /**
     * Reads an xref string from the current file position.
     * 
     * @param id The string to load the ID string into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool readXref(std::string& id);

    /**
     * Recursively reads nodes from the current file position.
     * This method will load cameras, lights and models in the nodes.
     * 
     * @return A pointer to new node or NULL if there was an error.
     */
    Node* readNode(Scene* sceneContext, Node* nodeContext, const std::vector<std::string>* nodesWithMeshRB);

    /**
     * Reads a camera from the current file position.
     *
     * @return A pointer to a new camera or NULL if there was an error.
     */
    Camera* readCamera();

    /**
     * Reads a light from the current file position.
     *
     * @return A pointer to a new light or NULL if there was an error.
     */
    Light* readLight();

    /**
     * Reads a model from the current file position.
     * 
     * @return A pointer to a new model or NULL if there was an error.
     */
    Model* readModel(Scene* sceneContext, Node* nodeContext, bool loadWithMeshRBSupport, const char* nodeId);

    /**
     * Reads a mesh skin from the current file position.
     *
     * @return A pointer to a new mesh skin or NULL if there was an error.
     */
    MeshSkin* readMeshSkin(Scene* sceneContext, Node* nodeContext);

    /**
     * Reads an animation from the current file position.
     * 
     * @param scene The scene to load the animations into.
     */
    void readAnimation(Scene* scene);

    /**
     * Reads an "animations" object from the current file position and all of the animations contained in it.
     * 
     * @param scene The scene to load the animations into.
     */
    void readAnimations(Scene* scene);

    /**
     * Reads an animation channel at the current file position into the given animation.
     * 
     * @param scene The scene that the animation is in.
     * @param animation The animation to the load channel into.
     * @param animationId The ID of the animation that this channel is loaded into.
     * 
     * @return The animation that the channel was loaded into.
     */
    Animation* readAnimationChannel(Scene* scene, Animation* animation, const char* animationId);

    /**
     * Sets the transformation matrix.
     *
     * @param values A pointer to array of 16 floats.
     * @param transform The transform to set the values in.
     */
    void setTransform(const float* values, Transform* transform);

    /**
     * Resolves joint references for all pending mesh skins.
     */
    void resolveJointReferences(Scene* sceneContext, Node* nodeContext);

private:
    std::string _path;
    unsigned int _referenceCount;
    Reference* _references;
    FILE* _file;

    std::vector<MeshSkinData*> _meshSkins;
};

}

#endif
