#ifndef FBXSCENEEENCODER_H_
#define FBXSCENEEENCODER_H_

#define FBXSDK_NEW_API

#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#ifdef WIN32
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4512 )
#endif
#include <fbxsdk.h>

#include "Base.h"
#include "StringUtil.h"
#include "Object.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "MeshSkin.h"
#include "Model.h"
#include "Scene.h"
#include "Animation.h"
#include "AnimationChannel.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Transform.h"
#include "GPBFile.h"
#include "EncoderArguments.h"

using namespace gameplay;

/**
 * Class for binary encoding an FBX file.
 */
class FBXSceneEncoder
{
public:

    static const unsigned int SCENE_SKIN_VERTEXINFLUENCES_MAX = 4;
    
    /**
     * Constructor.
     */
    FBXSceneEncoder();

    /**
     * Destructor.
     */
    ~FBXSceneEncoder();
    
    /**
     * Writes out encoded FBX file.
     */
    void write(const std::string& filepath, const EncoderArguments& arguments);

    /**
     * Writes a material file.
     * 
     * @param filepath 
     * 
     * @return True if successful; false otherwise.
     */
    bool writeMaterial(const std::string& filepath);

private:

    /**
     * Loads the scene.
     * 
     * @param fbxScene The FBX scene to load.
     */
    void loadScene(FbxScene* fbxScene);

    /**
     * Loads all of the animatiosn from the given FBX scene.
     * 
     * @param fbxScene The scene to load animations from.
     * @param arguments The command line arguments passed to the encoder.
     */
    void loadAnimations(FbxScene* fbxScene, const EncoderArguments& arguments);

    /**
     * Loads the animations from the given FBX animation layer recursively starting from fbxNode.
     * 
     * @param fbxAnimLayer The FBX animation layer to load from.
     * @param fbxNode The node to start loading animations from.
     * @param arguments The command line arguments passed to the encoder.
     */
    void loadAnimationLayer(FbxAnimLayer* fbxAnimLayer, FbxNode* fbxNode, const EncoderArguments& arguments);

    /**
     * Loads animation channels from the given node and adds the channels to the given animation.
     * 
     * @param pAnimLayer The FBX animation layer to load from.
     * @param fbxNode The node to load animation channels from.
     * @param animation The animation to add the channels to.
     */
    void loadAnimationChannels(FbxAnimLayer* pAnimLayer, FbxNode* fbxNode, Animation* animation);

    /**
     * Loads the bind shape for all mesh skins that have be loaded so far.
     * 
     * @param fbxScene The FBX scene to read the bind shapes from.
     */
    void loadBindShapes(FbxScene* fbxScene);

    /**
     * Loads the camera from the given FBX node and adds to it to the given GamePlay node.
     * 
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadCamera(FbxNode* fbxNode, Node* node);

    /**
     * Loads the light from the given FBX node and adds to it to the given GamePlay node.
     * 
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadLight(FbxNode* fbxNode, Node* node);
    
    /**
     * Loads the model from the given FBX node and adds to it to the given GamePlay node.
     *
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadModel(FbxNode* fbxNode, Node* node);

    /**
     * Loads materials for each node in the scene.
     */
    void loadMaterials(FbxScene* fbxScene);

    /**
     * Loads the material from the given node.
     */
    void loadMaterial(FbxNode* fbxNode);

    void loadMaterialTextures(FbxSurfaceMaterial* fbxMaterial, Material* material);

    void loadMaterialFileTexture(FbxFileTexture* fileTexture, Material* material);

    void loadMaterialUniforms(FbxSurfaceMaterial* fbxMaterial, Material* material);

    /**
     * Creates a material from an FbxSurfaceMaterial.
     * 
     * @param name The name of the new material.
     * @param fbxMaterial The FBX material to copy from.
     * @param node The node that the material is being created for.
     * 
     * @return The newly created material.
     */
    Material* createMaterial(const std::string& name, FbxSurfaceMaterial* fbxMaterial, Node* node);

    /**
     * Loads the mesh skin from the given FBX mesh and adds it to the given GamePlay model.
     *
     * @param fbxMesh The FBX mesh to load the skin from.
     * @param model The model to add the skin to.
     */
    void loadSkin(FbxMesh* fbxMesh, Model* model);
    
    /**
     * Loads the FBX Node and creates a GamePlay Node.
     * 
     * @param fbxNode The FBX Node to load.
     * 
     * @return The newly created Node or NULL if the node could not be loaded.
     */
    Node* loadNode(FbxNode* fbxNode);
    
    /**
     * Loads the FbxMesh and returns a GamePlay mesh.
     * If the fbxMesh has already been loaded then the same instance of mesh will be returned.
     * 
     * @param fbxMesh The FBX Mesh to load.
     * 
     * @return The GamePlay mesh that was loaded from the FBX Mesh.
     */
    Mesh* loadMesh(FbxMesh* fbxMesh);

    /**
     * Gets the Mesh that was saved with the given ID. Returns NULL if a match is not found.
     * 
     * @param meshId The ID of the FbxMesh to search for.
     * 
     * @return The mesh that was saved with the ID or NULL if none was found.
     */
    Mesh* getMesh(FbxUInt64 meshId);

    /**
     * Saves the Mesh with the given id.
     * 
     * @param meshId The ID of the FbxMesh to use as a key.
     * @param mesh The mesh to save.
     */
    void saveMesh(FbxUInt64 meshId, Mesh* mesh);
    
    /**
     * Prints a message.
     *
     * @param str The string to print.
     */
    void print(const char* str);

    /**
     * Transforms the GamePlay Node using the transform data from the FBX Node.
     * 
     * @param fbxNode The FBX node to get the transfrom data from
     * @param node The GamePlay Node to copy the transform to.
     */
    void transformNode(FbxNode* fbxNode, Node* node);

    /**
     * Gets the base material that matches the given id. Returns NULL if not found.
     */
    Material* getBaseMaterial(const char* id);

    /**
     * Finds the base material for the given material.
     * This will either return a previously loaded base material or 
     * the base material will be created and returned. (Should never return NULL)
     */
    Material* findBaseMaterial(Material* material);

    /**
     * Finds the gameplay Node that was created from the given FbxNode.
     * Returns NULL if not found.
     */
    Node* findNode(FbxNode* fbxNode);

    /**
     * Creates a base material with the given name from the given child material.
     * 
     * @param baseMaterialName The name of the base material to create.
     * @param childMaterial The child material that the base material is being created for.
     * 
     * @return The newly created base material.
     */
    Material* createBaseMaterial(const std::string& baseMaterialName, Material* childMaterial);

    /**
     * Recursively triangules the meshes starting from the given node.
     * 
     * @param fbxNode The node to start triangulating from.
     */
    static void triangulateRecursive(FbxNode* fbxNode);

private:

    /**
     * The GamePlay file that is populated while reading the FBX file.
     */
    GPBFile _gamePlayFile;

    /**
     * The collection of meshes for the purpose of making sure that the same model is not loaded twice. (Mesh instancing)
     */
    std::map<FbxUInt64, Mesh*> _meshes;

    /**
     * The list of child materials that were loaded.
     */
    std::map<std::string, Material*> _materials;

    /**
     * The list of base materials that the child materials are derived from.
     */
    std::map<std::string, Material*> _baseMaterials;

    /**
     * A map for keeping track of which gameplay Node was created from a given FbxNode.
     */
    std::map<FbxNode*, Node*> _nodeMap;

    /**
     * The animation that channels should be added to if the user is using the -groupAnimation command line argument. May be NULL.
     */
    Animation* _groupAnimation;

    /**
     * Indicates if the animations for mesh skins should be grouped before writing out the GPB file.
     */
    bool _autoGroupAnimations;
};

#endif
